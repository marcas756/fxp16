### CORDIC-Based Sine and Cosine Functions

https://de.wikipedia.org/wiki/CORDIC

The functions `fp16_cos`, `fp16_sin`, and `cordic_sin_cos_q15_pi` compute sine and cosine values using the **CORDIC algorithm** (COordinate Rotation DIgital Computer). CORDIC is an efficient iterative method for evaluating trigonometric functions through successive vector rotations. It eliminates the need for multiplications and relies solely on additions, subtractions, bit shifts, and table lookups, making it particularly suitable for embedded systems without floating-point hardware.

#### Algorithmic Principle

The CORDIC algorithm determines sine and cosine by iteratively rotating an initial unit vector `(x0, y0) = (1, 0)` toward the target angle. In each iteration, the vector is rotated by a predefined micro-angle `atan(2^-i)`, with the rotation direction chosen based on the residual angle. After a sufficient number of iterations, the resulting vector `(xn, yn)` converges to `(cos(theta), sin(theta))`, scaled by a constant gain factor `K`, which is compensated within the algorithm.

#### Input and Output Ranges

All functions use **Q15 fixed-point format** for both input and output:

* **Input range:**
  `-1.0` to `+1.0` represents angles from `-π` to `+π`.

* **Output range:**
  Sine and cosine values are returned in Q15 format, spanning `-1.0` to `+1.0`.

#### Function Overview

| Function                | Input Format | Output Format | Angle Range | Description                                |
| ----------------------- | ------------ | ------------- | ----------- | ------------------------------------------ |
| `cordic_sin_cos_q15_pi` | Q15          | Q15           | -π to +π    | Computes sine and cosine simultaneously    |
| `fp16_sin`              | Q15          | Q15           | -π to +π    | Computes sine using the CORDIC algorithm   |
| `fp16_cos`              | Q15          | Q15           | -π to +π    | Computes cosine using the CORDIC algorithm |


#### Interpretation of the Sine/Cosine Graph

<img width="866" height="577" alt="sincos" src="https://github.com/user-attachments/assets/ac9306cd-a352-4a4b-95b1-345c925cc03e" />

The figure above shows the sine (blue) and cosine (red) functions computed using the CORDIC implementation. The horizontal axis represents the **normalized input angle** in Q15 format, ranging from `-1.0` (corresponding to `-π`) to `+1.0 - LSB` (corresponding to `+π - LSB`). The vertical axis shows the computed sine and cosine values, also in Q15 format, ranging from `-1.0` to `+1.0 - LSB`.

* **Input normalization:**
  Angles are normalized to the interval `[-1.0, +1.0)` instead of `[-π, +π]`. The upper bound is deliberately limited to `+1.0 - LSB` (least significant bit), which corresponds to `+π - LSB` in the original angle domain. This avoids the singularity at `+π`, where sine and cosine wrap around. 

* **Output range:**
  The sine and cosine outputs are strictly confined to `[-1.0, +1.0 - LSB]` in Q15 format. This guarantees that the functions never overflow the representable fixed-point range, ensuring well-defined and bounded behavior across the entire input domain.

* **Function behavior:**

  * The **sine curve** crosses zero at the normalized input `0.0`, reaches its maximum of approximately `+1.0` at `+0.5` (corresponding to `+π/2`), and its minimum of approximately `-1.0` at `-0.5` (`-π/2`).
  * The **cosine curve** reaches its maximum at `0.0` (angle 0), crosses zero at approximately `±0.5` (±π/2), and reaches its minimum at the normalized input `±1.0` (±π). This matches the expected phase shift of 90° between sine and cosine.

* **Symmetry and periodicity:**
  Both functions show the expected even/odd symmetry: cosine is even (`cos(-x) = cos(x)`), sine is odd (`sin(-x) = -sin(x)`). The plot confirms that the implementation preserves these properties within the Q15 resolution.

* **Sampling granularity:**
  The smoothness of both curves indicates that the resolution of the Q15 input is sufficient to represent the functions without visible quantization artifacts. Any residual stair-stepping is below visual resolution, meaning that the angular step size is significantly finer than the display grid.

### Tangent Function Based on CORDIC

The function `fp16_tan` computes the tangent of a normalized angle using sine and cosine values obtained from the CORDIC-based function `cordic_sin_cos_q15_pi`. Instead of using a separate tangent-specific algorithm, the function first evaluates sine and cosine in Q15 fixed-point format and then derives the tangent as the quotient:

```
tan(x) = sin(x) / cos(x)
```

This design allows the reuse of the same efficient CORDIC implementation, ensuring consistent numerical characteristics across trigonometric functions.

#### Input and Output Ranges

* **Input range:**
  The input angle is represented in Q15 fixed-point format in the interval `-1.0` to `+1.0 - LSB`, corresponding to the real angle range `-π` to `+π - LSB`. The upper bound is intentionally limited by one least significant bit (LSB) to avoid discontinuities at `+π`.

* **Output range:**
  The output is provided in fixed-point format, either Q0 (integer) or Q15, depending on configuration. Since the tangent function is unbounded, the result may saturate when the cosine approaches zero near `±π/2`. The exact saturation limit depends on the chosen output format.

#### Numerical Characteristics

* **Singularities:**
  Near normalized input values of approximately `±0.5` (corresponding to `±π/2`), the cosine value approaches zero. Consequently, the computed tangent grows rapidly in magnitude, eventually reaching the saturation limits of the fixed-point representation.
* **Accuracy:**
  For input angles sufficiently far from ±π/2, the computed tangent closely follows the analytical tangent function. Numerical deviations are mainly determined by the resolution of the Q15 format and the precision of the fixed-point division.
* **Consistency:**
  Since sine and cosine are derived from the same CORDIC iteration, the phase relationship remains coherent, and the tangent function inherits the stability and determinism of the underlying algorithm.

#### Warning on Overflow and Saturation

When the cosine value approaches zero, the division may exceed the representable range of the fixed-point output format. In such cases, the result is **saturated** to the maximum or minimum representable value, depending on the sign of the numerator. This behavior is intentional and ensures that the function never wraps around or produces undefined results. Users should take this into account when applying `fp16_tan` in control loops or signal processing algorithms near ±π/2, where the tangent becomes numerically ill-conditioned.

### Interpretation of the Tangent Graph
<img width="866" height="577" alt="tan" src="https://github.com/user-attachments/assets/3bc6d54b-7506-4651-9863-47dc7e1f4653" />

The figure above shows the behavior of the tangent function implemented via `fp16_tan` for three different output formats: **Q0** (blue dotted), **Q8** (red dashed), and **Q15** (yellow solid).

The horizontal axis represents the **normalized input angle** in Q15 format, ranging from `-1.0` (equivalent to `-π`) to `+1.0 - LSB` (equivalent to `+π - LSB`). The vertical axis shows the computed tangent values in the respective fixed-point output formats.

#### General Behavior

* For all three formats, the function follows the characteristic tangent curve, with singularities near normalized inputs of approximately `±0.5` (corresponding to `±π/2`).
* Around these points, the cosine denominator approaches zero, causing the tangent to grow rapidly in magnitude. This results in vertical asymptotes in the mathematical function and saturation in the fixed-point implementation.

#### Q15 Output (Yellow Solid Line)

* The Q15 format provides the highest resolution but is **explicitly limited to the range `-1.0` to `+1.0 - LSB`**.
* As the input approaches `±π/2`, the output saturates at these fixed limits, as clearly visible near the vertical asymptotes at ±0.5.
* This design choice prevents overflows and ensures stable behavior even in ill-conditioned regions, but it also means that very large tangent values cannot be represented in Q15.

#### Q8 Output (Red Dashed Line)

* Q8 provides a coarser resolution but a **larger dynamic range** than Q15, allowing tangent values to grow significantly before saturation.
* The graph shows that Q8 tracks the analytical tangent function well up to a point, after which the output rapidly saturates near the format’s maximum representable value.
* The curve near ±0.5 shows a steep, continuous transition, reflecting the trade-off between resolution and range typical of mid-range fixed-point formats.

#### Q0 Output (Blue Dotted Line)

* Q0 represents the extreme case of minimal resolution and **integer-only output**.
* The graph shows clear step-wise behavior, with tangent values quantized to integer levels.
* This coarse representation is useful in applications where only rough directional information is needed or where numerical resources are limited, but it cannot capture smooth function transitions.

#### Additional Observations

* **Saturation Behavior:** All three formats exhibit predictable saturation as the cosine approaches zero. Q15 saturates earliest due to its strict ±1.0 limit, whereas Q8 and Q0 continue to grow until reaching their respective fixed-point limits.
* **Quantization Effects:** As the resolution decreases (from Q15 to Q0), quantization steps become clearly visible. Q15 appears smooth, Q8 shows minor stepping, and Q0 exhibits strong stair-stepping.
* **Symmetry:** The curves are antisymmetric with respect to the origin, reflecting the mathematical property tan(-x) = -tan(x), confirming consistent behavior across formats.
* **Practical Implication:** The choice of output format directly impacts both **accuracy** and **dynamic range**. Applications that require fine angular discrimination should prefer Q15, while those prioritizing range or computational simplicity may choose Q8 or Q0 depending on the use case.


### Two-Argument Arctangent Computation Using CORDIC

The function `fp16_atan2` computes the angle of a vector `(x, y)` using the **CORDIC vectoring mode**. It returns the angle in **π-normalized Q15 format**, i.e. the interval `-1.0` to `+1.0 - LSB` corresponds to the real angle range `-π` to `+π - LSB`. The implementation follows the behavior of the standard `atan2(y, x)` function known from floating-point math libraries, but uses fixed-point arithmetic throughout.

#### Algorithmic Principle

The CORDIC algorithm in vectoring mode rotates the input vector `(x, y)` step by step toward the x-axis. In each iteration, a predefined micro-angle `atan(2^-i)` is either added or subtracted depending on the sign of `y`. After a sufficient number of iterations, `y` converges to zero and the accumulated rotation angle corresponds to the arctangent of the original vector. This method avoids divisions and relies exclusively on additions, subtractions, bit shifts, and table lookups, making it highly efficient for fixed-point embedded implementations.

#### Input and Output Ranges

* **Input:**

  * Both `x` and `y` must be given in **Q15 fixed-point format**, representing values in the range `-1.0` to `+1.0 - LSB`.
  * If other fixed-point formats are used, both `x` and `y` must first be scaled to Q15 using the **same scaling factor**, to preserve the correct ratio `y/x`.
  * Special cases are handled explicitly: `(0, 0)` returns 0, `(x < 0, y = 0)` returns `+π`, and vertical vectors `(x = 0)` return `±π/2` depending on the sign of `y`.

* **Output:**

  * The return value is the angle in **π-normalized Q15 format**, ranging from `-1.0` (−π) to `+1.0 - LSB` (+π − LSB).
  * The function covers the entire angular range, including all four quadrants, by internally mirroring vectors in the left half-plane and applying a quadrant correction at the end.



 


#### Interpretation of the Arcus Sine/Arcus Cosine Graph
<img width="866" height="577" alt="asinacos" src="https://github.com/user-attachments/assets/3e549fcc-b639-43f7-8f57-0e9162f23b8c" />




## Implementation Checklist (in comparison to math.h floating point implementation)

### Trigonometric functions

| Name       | Description                                        | Implemented?   |
| ---------- | -------------------------------------------------- | -------------- |
| fp16_cos   | Compute cosine                                     | [X]            |
| fp16_sin   | Compute sine                                       | [X]            |
| fp16_tan   | Compute tangent                                    | [X]            |
| fp16_acos  | Compute arc cosine                                 | [X]            |
| fp16_asin  | Compute arc sine                                   | [X]            |
| fp16_atan  | Compute arc tangent                                | [X]            |
| fp16_atan2 | Compute arc tangent with two parameters            | [X]            |

### Hyperbolic functions

| Name       | Description                                | Implemented?   |
| ---------- | ------------------------------------------ | -------------- |
| fp16_cosh  | Compute hyperbolic cosine                  | [ ]            |
| fp16_sinh  | Compute hyperbolic sine                    | [ ]            |
| fp16_tanh  | Compute hyperbolic tangent                 | [ ]            |
| fp16_acosh | Compute area hyperbolic cosine             | [ ]            |
| fp16_asinh | Compute area hyperbolic sine               | [ ]            |
| fp16_atanh | Compute area hyperbolic tangent            | [ ]            |

### Exponential and logarithmic functions

| Name         | Description                                                            | Implemented?   |
| ------------ | ---------------------------------------------------------------------- | -------------- |
| fp16_exp     | Compute exponential function                                           | [ ]            |
| fp16_frexp   | Get significand and exponent                                           | [ ]            |
| fp16_ldexp   | Generate value from significand and exponent                           | [ ]            |
| fp16_log     | Compute natural logarithm                                              | [ ]            |
| fp16_log10   | Compute common logarithm                                               | [ ]            |
| fp16_modf    | Break into fractional and integral parts                               | [ ]            |
| fp16_exp2    | Compute binary exponential function                                    | [ ]            |
| fp16_expm1   | Compute exponential minus one                                          | [ ]            |
| fp16_ilogb   | Integer binary logarithm                                               | [ ]            |
| fp16_log1p   | Compute logarithm plus one                                             | [ ]            |
| fp16_log2    | Compute binary logarithm                                               | [ ]            |
| fp16_logb    | Compute floating-point base logarithm                                  | [ ]            |
| fp16_scalbn  | Scale significand using floating-point base exponent                   | [ ]            |
| fp16_scalbln | Scale significand using floating-point base exponent (long)            | [ ]            |

### Power functions

| Name       | Description                    | Implemented?   |
| ---------- | ------------------------------ | -------------- |
| fp16_pow   | Raise to power                 | [ ]            |
| fp16_sqrt  | Compute square root            | [X]            |
| fp16_cbrt  | Compute cubic root             | [ ]            |
| fp16_hypot | Compute hypotenuse             | [ ]            |

### Error and gamma functions

| Name        | Description                                     | Implemented?   |
| ----------- | ----------------------------------------------- | -------------- |
| fp16_erf    | Compute error function                          | [ ]            |
| fp16_erfc   | Compute complementary error function            | [ ]            |
| fp16_tgamma | Compute gamma function                          | [ ]            |
| fp16_lgamma | Compute log-gamma function                      | [ ]            |

### Rounding and remainder functions

| Name           | Description                                               | Implemented?   |
| -------------- | --------------------------------------------------------- | -------------- |
| fp16_ceil      | Round up value                                            | [X]            |
| fp16_floor     | Round down value                                          | [X]            |
| fp16_fmod      | Compute remainder of division                             | [X]            |
| fp16_trunc     | Truncate value                                            | [X]            |
| fp16_round     | Round to nearest                                          | [X]            |
| fp16_lround    | Round to nearest and cast to long integer                 | [X]            |
| fp16_llround   | Round to nearest and cast to long long integer            | [ ]            |
| fp16_rint      | Round to integral value                                   | [ ]            |
| fp16_lrint     | Round and cast to long integer                            | [ ]            |
| fp16_llrint    | Round and cast to long long integer                       | [ ]            |
| fp16_nearbyint | Round to nearby integral value                            | [ ]            |
| fp16_remainder | Compute remainder (IEC 60559)                             | [ ]            |
| fp16_remquo    | Compute remainder and quotient                            | [ ]            |

### Floating-point manipulation functions

| Name            | Description                                              | Implemented?   |
| --------------- | -------------------------------------------------------- | -------------- |
| fp16_copysign   | Copy sign                                                | [ ]            |
| fp16_nan        | Generate quiet NaN                                       | [ ]            |
| fp16_nextafter  | Next representable value                                 | [ ]            |
| fp16_nexttoward | Next representable value toward precise value            | [ ]            |

### Minimum, maximum, difference functions

| Name      | Description                    | Implemented? |
| ---- | ------------------------------ | -------------- |
| fp16_fdim | Positive difference            | [ ]            |
| fp16_fmax | Maximum value                  | [ ]            |
| fp16_fmin | Minimum value                  | [ ]            |

### Other functions

| Name      | Description                       | Implemented?   |
| --------- | --------------------------------- | -------------- |
| fp16_fabs | Compute absolute value            | [ ]            |
| fp16_abs  | Compute absolute value            | [ ]            |
| fp16_fma  | Multiply-add                      | [ ]            |

### Classification macro / functions (C: Macros, C++: Functions)

| Name            | Description                                    | Implemented?   |
| --------------- | ---------------------------------------------- | -------------- |
| fp16_fpclassify | Classify floating-point value                  | [ ]            |
| fp16_isfinite   | Is finite value                                | [ ]            |
| fp16_isinf      | Is infinity                                    | [ ]            |
| fp16_isnan      | Is Not-A-Number                                | [ ]            |
| fp16_isnormal   | Is normal                                      | [ ]            |
| fp16_signbit    | Sign bit                                       | [ ]            |

### Comparison macro / functions

| Name                | Description                 | Implemented?   |
| ------------------- | --------------------------- | -------------- |
| fp16_isgreater      | Is greater                  | [ ]            |
| fp16_isgreaterequal | Is greater or equal         | [ ]            |
| fp16_isless         | Is less                     | [ ]            |
| fp16_islessequal    | Is less or equal            | [ ]            |
| fp16_islessgreater  | Is less or greater          | [ ]            |
| fp16_isunordered    | Is unordered                | [ ]            |

### Macro constants

| Name                  | Description                       | Implemented? |
| --------------------- | --------------------------------- | -------------- |
| fp16_math_errhandling | Error handling                    | [ ]            |
| FP16_INFINITY         | Infinity                          | [ ]            |
| FP16_NAN              | Not-A-Number                      | [ ]            |
| FP16_HUGE_VAL         | Huge value                        | [ ]            |
| FP16_HUGE_VALF        | Huge float value                  | [ ]            |
| FP16_HUGE_VALL        | Huge long double value            | [ ]            |
