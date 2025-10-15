# FXP16 - 16 Bit Fixed Point Math Library
FXP16 is a lightweight and flexible fixed-point math library designed for 16-bit fixed-point arithmetic with support for different fractional/integral bit splits (Q0 … Q15). Its goal is to provide a robust set of mathematical functions comparable to those defined in the C standard library’s math.h, but optimized for environments where floating-point support is limited or undesirable - such as embedded systems.

The library is currently a work in progress. Many core trigonometric and rounding functions are already implemented, while others are under active development.

## Implementation Details

### CORDIC-Based Sine and Cosine Functions

https://de.wikipedia.org/wiki/CORDIC

The functions `fxp16_cos`, `fxp16_sin`, and `cordic_sin_cos_q15_pi` compute sine and cosine values using the **CORDIC algorithm** (COordinate Rotation DIgital Computer). CORDIC is an efficient iterative method for evaluating trigonometric functions through successive vector rotations. It eliminates the need for multiplications and relies solely on additions, subtractions, bit shifts, and table lookups, making it particularly suitable for embedded systems without floating-point hardware.

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
| `fxp16_sin`              | Q15          | Q15           | -π to +π    | Computes sine using the CORDIC algorithm   |
| `fxp16_cos`              | Q15          | Q15           | -π to +π    | Computes cosine using the CORDIC algorithm |


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

The function `fxp16_tan` computes the tangent of a normalized angle using sine and cosine values obtained from the CORDIC-based function `cordic_sin_cos_q15_pi`. Instead of using a separate tangent-specific algorithm, the function first evaluates sine and cosine in Q15 fixed-point format and then derives the tangent as the quotient:

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

When the cosine value approaches zero, the division may exceed the representable range of the fixed-point output format. In such cases, the result is **saturated** to the maximum or minimum representable value, depending on the sign of the numerator. This behavior is intentional and ensures that the function never wraps around or produces undefined results. Users should take this into account when applying `fxp16_tan` in control loops or signal processing algorithms near ±π/2, where the tangent becomes numerically ill-conditioned.

#### Interpretation of the Tangent Graph
<img width="866" height="577" alt="tan" src="https://github.com/user-attachments/assets/3bc6d54b-7506-4651-9863-47dc7e1f4653" />

The figure above shows the behavior of the tangent function implemented via `fxp16_tan` for three different output formats: **Q0** (blue dotted), **Q8** (red dashed), and **Q15** (yellow solid).

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

The function `fxp16_atan2` computes the angle of a vector `(x, y)` using the **CORDIC vectoring mode**. It returns the angle in **π-normalized Q15 format**, i.e. the interval `-1.0` to `+1.0 - LSB` corresponds to the real angle range `-π` to `+π - LSB`. The implementation follows the behavior of the standard `atan2(y, x)` function known from floating-point math libraries, but uses fixed-point arithmetic throughout.

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


#### Interpretation of the Two-Argument Arctangent Graph
<img width="866" height="577" alt="atan2" src="https://github.com/user-attachments/assets/cfc11c78-a5c3-45e6-a6e4-15e8ea4eeeb5" />

The figure above illustrates the output of the `fxp16_atan2` function for different quadrants, plotted against the **ratio `y/x`** on the horizontal axis. The vertical axis represents the **output angle in π-normalized Q15 format**, ranging from `-1.0` (−π) to `+1.0 - LSB` (+π − LSB). Each curve corresponds to one of the four quadrants:

* **Quadrant I (x > 0, y > 0)** – blue
* **Quadrant II (x < 0, y > 0)** – red
* **Quadrant III (x < 0, y < 0)** – yellow
* **Quadrant IV (x > 0, y < 0)** – green

#### General Behavior

* For **x > 0**, the function behaves like the standard `atan(y/x)` curve, smoothly mapping ratios from negative to positive into the range `(-0.5, +0.5)` (corresponding to `(-π/2, +π/2)`).

* For **x < 0**, a **quadrant offset** of ±π is added to the basic `atan(y/x)` value:

  * In Quadrant II (x < 0, y > 0), the output equals `atan(y/x) + π`, resulting in a vertical shift upward by +1.0 (π).
  * In Quadrant III (x < 0, y < 0), the output equals `atan(y/x) − π`, resulting in a vertical shift downward by −1.0 (−π).
    This matches the standard mathematical definition of `atan2` and ensures a full angular coverage of (−π, +π).

* As `y/x` → ±∞ (corresponding to x → 0), the curves approach ±0.5 (±π/2) or their shifted equivalents, depending on the quadrant. The function transitions smoothly and continuously across all quadrants.

#### Special Cases

The implementation also explicitly handles singular input combinations to match the conventional behavior of `atan2`:

| Condition    | Result (π-normalized Q15) | Angle |
| ------------ | ------------------------- | ----- |
| x = 0, y = 0 | 0.0                       | 0     |
| x = 0, y > 0 | +0.5                      | +π/2  |
| x = 0, y < 0 | −0.5                      | −π/2  |
| x > 0, y = 0 | 0.0                       | 0     |
| x < 0, y = 0 | +1.0                      | +π    |

This ensures well-defined outputs even at the axes and origin.

#### Additional Observations

* **Symmetry:** The output exhibits the expected symmetry: Quadrants II and III are vertical translations of Quadrants I and IV by ±π, as defined by the mathematical `atan2`.
* **Range Coverage:** The function covers the entire normalized output range from `−1.0` to `+1.0 - LSB`, corresponding to angles from `−π` to `+π - LSB`.
* **Smooth Transition:** The graph confirms that the implementation provides smooth transitions between quadrants and maintains continuity at quadrant boundaries (excluding the singular points at x = 0).
* **Numerical Stability:** Since the function internally uses CORDIC vectoring mode, it remains stable for both large and small values of `y/x` without divisions.

### CORDIC-Based Arcus Tangent Function

The function `fxp16_atan()` computes the arctangent of a fixed-point input using the **CORDIC vectoring algorithm** via the helper function `fxp16_atan2()`. This implementation operates entirely in fixed-point arithmetic and is suitable for embedded systems without floating-point hardware.

#### Algorithmic Principle

1. The input value `y` is given in an arbitrary fixed-point format with `frac` fractional bits.
2. It is converted to Q15 format by shifting to match the internal CORDIC representation.
3. A reference value `x = 1.0` (in Q15) is paired with the scaled `y` to form the vector `(x, y)`.
4. If either `x` or `y` lies outside the representable Q15 range `[-1.0, +1.0]`, both are **iteratively scaled down** until they fit within bounds.
5. The angle is then computed using `fxp16_atan2(y, x)`, returning the arctangent in **π-normalized Q15**.

Because `x` is always positive in this construction, the output range is limited to `[-0.5, +0.5]`, corresponding to `[-π/2, +π/2]`, matching the mathematical definition of `atan`.

#### Input and Output Ranges

* **Input:**

  * `y`: fixed-point number in Q`frac` format
  * `frac`: number of fractional bits of `y`

* **Output:**

  * π-normalized Q15, representing an angle in the range `[-0.5, +0.5]` (equivalent to `[-π/2, +π/2]`)

---

#### Iterative Scaling Loop

Before the CORDIC calculation, the implementation ensures that both `x` and `y` remain within the valid Q15 domain:

```c
while (Y > FXP32_Q15_ONE || Y < FXP16_Q15_MINUS_ONE) {
    Y = fxp32_arshift(Y, 1);
    x = fxp32_arshift(x, 1);
}
```

This **iterative scaling loop** halves both `x` and `y` repeatedly until their magnitudes are ≤ 1.0 in Q15. This step is crucial because:

* The CORDIC vectoring algorithm assumes inputs are within the unit circle in Q15 format.
* Scaling both values by the same factor preserves the **y/x ratio**, ensuring that the computed angle remains mathematically correct.
* It prevents numerical overflow and ensures convergence of the iterative vectoring process.

This mechanism allows the function to handle a wide dynamic range of input values, independent of their original fixed-point format, while maintaining deterministic and numerically stable behavior.

#### Interpretation of the Arcus Tangent Graph
<img width="866" height="577" alt="atan" src="https://github.com/user-attachments/assets/f8e9cefa-d63c-43a4-9c28-a2b4a4ff033d" />

The figure above shows the output of the `fxp16_atan` function for input values `x` in different **fixed-point formats (Q11–Q15)**. The horizontal axis represents the real input value, while the vertical axis shows the π-normalized Q15 output of the arctangent function in the range `[-0.5, +0.5]` (equivalent to `[-π/2, +π/2]`).

Each colored curve corresponds to a different input format:

* **Q15** – green
* **Q14** – yellow
* **Q13** – red
* **Q12** – blue
* **Q11** – purple

#### General Behavior

* All curves follow the characteristic shape of the arctangent function:

  * For **small input magnitudes**, the function is nearly linear, reflecting `atan(x) ≈ x` for `|x| ≪ 1`.
  * For **large positive inputs**, the function approaches `+0.5` (i.e., `+π/2`) asymptotically.
  * For **large negative inputs**, the function approaches `−0.5` (i.e., `−π/2`) asymptotically.

* Regardless of the input Q format, the **output range** is always confined to the π-normalized interval `[-0.5, +0.5]`, because `fxp16_atan` internally uses a fixed `x = 1` reference and applies `atan2(y, 1)`.

#### Effect of Input Fixed-Point Formats

The different segments of the graph show how the function handles inputs with varying fixed-point resolution:

* **Q15 (green)** covers the region around `[-1, +1]` with high resolution. Within this domain, no iterative scaling is needed, and the output corresponds directly to the Q15 input precision.

* For **Q14, Q13, Q12, and Q11** (yellow, red, blue, purple), the curves extend over increasingly larger input magnitudes. This behavior is a result of the **iterative scaling loop** in the implementation:

  * Inputs larger than ±1.0 (in Q15 terms) are repeatedly shifted right (divided by 2), together with the reference value, until they fit within the Q15 domain.
  * This preserves the ratio `y/x` and allows the algorithm to handle very large inputs without overflow.

* As a consequence, inputs in lower Q formats can represent **larger real values**, but the resolution decreases correspondingly. The arctangent function remains correct, as it depends only on the ratio, not on the absolute magnitude.

#### Additional Observations

* **Saturation Behavior:** For very large positive or negative inputs, all curves flatten out at ±0.5, demonstrating the correct asymptotic behavior of `atan(x) → ±π/2`.
* **Smooth Transition Across Scales:** The transitions between different Q formats are smooth and continuous. This shows that the scaling mechanism preserves functional shape without introducing discontinuities.
* **Dynamic Range vs. Precision Trade-off:**

  * Higher Q formats (e.g., Q15) provide finer angular resolution for moderate input values.
  * Lower Q formats extend the dynamic range, enabling correct handling of larger inputs at the cost of reduced precision.

### Arcus Sine/Cosine Functions

The functions `fxp16_asin()` and `fxp16_acos()` compute the **arcsine** and **arccosine** of a Q15 fixed-point input, returning the result in **π-normalized Q15 format**. Both functions are implemented using algebraic transformations and the CORDIC-based `fxp16_atan2()` function, ensuring high numerical stability and full quadrant coverage without requiring floating-point arithmetic.

#### Algorithmic Principle

For a given input `x` (Q15, range `[-1.0, +1.0]`):

* **Arcsine:**
  The arcsine is computed using the identity

  ```
  asin(x) = atan2(x, sqrt(1 - x²))
  ```

  First, the value `t = 1 - x²` is computed in extended 32-bit precision to avoid overflow. Its square root is then obtained in Q15 using `fxp16_sqrt()`. Finally, `fxp16_atan2(x, c)` is called with `c = sqrt(1 - x²)` to obtain the result in π-normalized Q15 format.
  The output range of `fxp16_asin()` is `[-0.5, +0.5]`, corresponding to `[-π/2, +π/2]`.

* **Arccosine:**
  The arccosine is computed using the identity

  ```
  acos(x) = atan2(sqrt(1 - x²), x)
  ```

  The same intermediate value `c = sqrt(1 - x²)` is reused, but the arguments are passed in reversed order to `fxp16_atan2()`.
  The output range of `fxp16_acos()` is `[0.0, +1.0]`, corresponding to `[0, π]`.

These formulations reduce both functions to a single square root and an `atan2` evaluation, leveraging the existing CORDIC-based arctangent implementation.

#### Input and Output Ranges

| Function    | Input Format | Input Range      | Output Format | Output Range (π-normalized Q15) |
| ----------- | ------------ | ---------------- | ------------- | ------------------------------- |
| `fxp16_asin` | Q15          | `-1.0` to `+1.0` | Q15           | `-0.5` to `+0.5`  (−π/2 … +π/2) |
| `fxp16_acos` | Q15          | `-1.0` to `+1.0` | Q15           | `0.0` to `+1.0`   (0 … π)       |

#### Numerical Characteristics

* **Precision:**
  Both functions use 32-bit intermediate computations for `1 - x²` to avoid rounding errors and negative intermediate values at the domain boundaries.

* **Domain Handling:**
  Input values outside the interval `[-1.0, +1.0]` are not expected. For `|x| = 1.0`, the square root term becomes exactly zero, yielding well-defined outputs at the domain boundaries (±π/2 for asin, 0 and π for acos).

* **CORDIC Consistency:**
  Since both functions rely on `fxp16_atan2`, they inherit its quadrant correctness, stability, and π-normalized output range.




#### Interpretation of the Arcus Sine/Arcus Cosine Graph
<img width="866" height="577" alt="asinacos" src="https://github.com/user-attachments/assets/3e549fcc-b639-43f7-8f57-0e9162f23b8c" />

The figure above shows the output of the `fxp16_asin` (blue) and `fxp16_acos` (red) functions for inputs `x` in the range `[-1.0, +1.0]` (Q15). The vertical axis represents the **π-normalized Q15 output**, with `-0.5` corresponding to `−π/2`, `0.0` to `0`, `0.5` to `+π/2`, and `1.0` to `+π`.

#### General Behavior

* **Arcsine (blue):**

  * The curve starts at `−0.5` for `x = −1`, passes through `0.0` at `x = 0`, and reaches `+0.5` at `x = +1`.
  * This matches the mathematical definition of asin(x), which maps the interval `[-1, +1]` onto `[-π/2, +π/2]`.
  * The curve is **monotonically increasing**, symmetric about the origin, and reflects the odd function property `asin(−x) = −asin(x)`.

* **Arccosine (red):**

  * The curve starts at `+1.0` for `x = −1`, decreases to `0.5` at `x = 0`, and reaches `0.0` at `x = +1`.
  * This corresponds to the mathematical acos(x), mapping `[-1, +1]` to `[π, 0]` (π-normalized `[1.0, 0.0]`).
  * The function is **monotonically decreasing** over the entire input range.

#### Relationship Between `asin` and `acos`

The graph clearly shows the fundamental identity:

```
asin(x) + acos(x) = π/2
```

In π-normalized Q15, this corresponds to:

```
asin(x) + acos(x) = 0.5
```

At every point along the x-axis, the sum of the two curves equals `0.5`. This confirms that both implementations are numerically consistent and adhere to the standard trigonometric relationship.

#### Additional Observations

* **Domain Handling:**
  Both functions operate only within the mathematically valid domain `x ∈ [−1.0, +1.0]`. Outside this range, the square root in `sqrt(1 − x²)` would be undefined. The implementation clamps intermediate values to ensure stable behavior at the domain boundaries.

* **Boundary Values:**

  * For x = +1:
    `asin(+1)` = +0.5 (π/2), `acos(+1)` = 0.0 (0).
  * For x = −1:
    `asin(−1)` = −0.5 (−π/2), `acos(−1)` = 1.0 (π).
  * For x = 0:
    `asin(0)` = 0.0, `acos(0)` = 0.5 (π/2).

* **Numerical Smoothness:**
  The curves are smooth and continuous over the entire domain, demonstrating that the combination of the square root and CORDIC-based `atan2` yields numerically stable results even near the boundaries x = ±1, where rounding errors are most critical.

* **Monotonicity and Symmetry:**
  `asin(x)` is strictly increasing and odd-symmetric, whereas `acos(x)` is strictly decreasing and symmetric about x = 0. These properties match their analytical definitions exactly.

### CORDIC-Based Hyperbolic Sine, Cosine, and Tangent

`fxp16_sinh`, `fxp16_cosh`, and `fxp16_tanh` use a hyperbolic CORDIC core that replaces multiplications with additions, subtractions, shifts, and lookups—ideal for MCUs without FPUs. The kernel runs in Q15 and is wrapped by fxp16 helpers that convert to and from arbitrary Q-formats.

#### How it works

The algorithm operates in rotation mode on a vector that already includes the hyperbolic CORDIC gain (`FXP32_Q15_K_HYP`). It iteratively adjusts the vector using `atanh(2^-i)` micro-angles (Q15 table entries for `i = 1..16`). For radix-2 hyperbolic CORDIC, iterations `i = 4` and `i = 13` are repeated to ensure convergence; this is handled internally.

To cover the full range reliably, inputs are reduced with respect to `ln(2)`. The value `x` is written as `x ≈ n·ln(2) + r` with small residual `r`. The kernel computes `(cosh(r), sinh(r))`, and the final result is recomposed exactly using powers of two (`2^n` and `2^-n`) formed by shifts. Rounding is applied where right shifts model scaling; left shifts saturate. Multiplies use a 64-bit intermediate and saturate on overflow; adds are saturating as well.

For very large magnitudes detected during range reduction (|n| ≥ 16), `cosh` saturates to positive max and `sinh` saturates by the sign of `x`. For `tanh`, extreme inputs short-circuit to the largest magnitude below 1 with the correct sign; otherwise `tanh(x)` is computed as `sinh(x)/cosh(x)` via a rounded Q15 division bounded to (−1, 1).

#### Formats

Internally everything runs in Q15. The fxp16 wrappers accept any fxp16 Q-format via `x_frac`, convert to Q15, run the core, then convert the result to the requested `y_frac` with fxp16-range saturation.

## Functions

| Function                       | Input          | Output         | Range   | Summary                                                                                            |   |                                                                       |
| ------------------------------ | -------------- | -------------- | ------- | -------------------------------------------------------------------------------------------------- | - | --------------------------------------------------------------------- |
| `fxp32_cordic_cosh_sinh_q15`    | Q15            | Q15            | any `x` | ln(2) reduction → CORDIC kernel → exact recomposition with `2^±n`; early saturation for very large | n | .                                                                     |
| `fxp32_cordic_tanh_q15`         | Q15            | Q15            | any `x` | Early ±1 saturation for large                                                                      | x | ; otherwise `sinh/ cosh` with rounded Q15 divide, clamped to (−1, 1). |
| `fxp16_sinh(y_frac, x, x_frac)` | fxp16 Q`x_frac` | fxp16 Q`y_frac` | any `x` | Format convert → Q15 core → convert back with fxp16 saturation.                                     |   |                                                                       |
| `fxp16_cosh(y_frac, x, x_frac)` | fxp16 Q`x_frac` | fxp16 Q`y_frac` | any `x` | As above, returning `cosh`.                                                                        |   |                                                                       |
| `fxp16_tanh(y_frac, x, x_frac)` | fxp16 Q`x_frac` | fxp16 Q`y_frac` | any `x` | Early saturation or Q15 `tanh` path → convert back with fxp16 saturation.                           |   |                                                                       |

#### Notes

Micro-angles are stored in `fxp32_q15_atanh_tab[]`. The gain constant `FXP32_Q15_K_HYP` pre-compensates the kernel so no post-scaling is needed. The only division appears in `tanh` and is rounded with bounds that enforce the open interval (−1, 1). Range reduction keeps intermediates safe and improves numerical behavior without resorting to floating point.

#### Interpretation of the Hyperbolic Sine / Hyperbolic Cosine / Hyperbolic Tangent Graph
<img width="866" height="577" alt="sinhcoshtanh" src="https://github.com/user-attachments/assets/5b12825c-bf2c-48f4-80f3-b97892caf97b" />

The figure above shows the output of the hyperbolic functions `fxp16_sinh` (blue, dashed), `fxp16_cosh` (red), and `fxp16_tanh` (yellow) for inputs `x` in Q8 format. The horizontal axis represents the input `x / Q8`, while the vertical axis shows the evaluated hyperbolic functions in the same scaling.

##### General Behavior

**Hyperbolic Sine (blue, dashed):**
The `sinh(x)` curve is odd-symmetric. It passes through the origin and grows exponentially for large |x|, becoming negative for negative inputs and positive for positive inputs. Its slope at the origin equals 1, matching the mathematical derivative `d/dx sinh(0) = 1`.

**Hyperbolic Cosine (red):**
The `cosh(x)` curve is even-symmetric, with a minimum value of 1 at `x = 0`. It rises rapidly and dominates for large |x|, reflecting the exponential definition `cosh(x) = (e^x + e^−x)/2`. Unlike sine, it never falls below 1 and grows without bound in both directions.

**Hyperbolic Tangent (yellow):**
The `tanh(x)` curve starts at 0 and smoothly approaches ±1 as `x` increases or decreases. It has a steep slope around the origin and quickly saturates, illustrating the characteristic sigmoidal shape. This behavior mirrors the analytic `tanh(x) = sinh(x)/cosh(x)`, where the ratio tends to ±1 as |x| becomes large.

##### Functional Relationships

The plotted curves show the expected identity:

```
tanh(x) = sinh(x) / cosh(x)
```

This is especially visible near large |x|, where `sinh(x)` and `cosh(x)` both grow exponentially and their ratio tends to ±1, resulting in the horizontal asymptotes of `tanh(x)`.

##### Symmetry and Growth

`sinh(x)` is strictly increasing and odd, `cosh(x)` is strictly increasing for x > 0 and even, and `tanh(x)` is strictly increasing and odd with horizontal asymptotes at ±1. The exponential growth of `cosh` and `sinh` is evident from the curvature of their plots. In contrast, `tanh` remains bounded, which makes it particularly useful for applications such as numerical normalization or activation functions in neural computations.

##### Numerical Behavior

The plotted curves are smooth, continuous, and free of visible discontinuities or oscillations. This confirms that the fixed-point hyperbolic CORDIC implementation handles scaling, range reduction, and saturation correctly across both small and large input ranges. Around the origin, all functions behave linearly or quadratically as expected, and for larger arguments, `cosh` and `sinh` grow exponentially while `tanh` stabilizes toward its asymptotes.






## Implementation Checklist (in comparison to math.h floating point implementation)

### Trigonometric functions

| Name       | Description                                        | Implemented?   |
| ---------- | -------------------------------------------------- | -------------- |
| fxp16_cos   | Compute cosine                                     | [X]            |
| fxp16_sin   | Compute sine                                       | [X]            |
| fxp16_tan   | Compute tangent                                    | [X]            |
| fxp16_acos  | Compute arc cosine                                 | [X]            |
| fxp16_asin  | Compute arc sine                                   | [X]            |
| fxp16_atan  | Compute arc tangent                                | [X]            |
| fxp16_atan2 | Compute arc tangent with two parameters            | [X]            |

### Hyperbolic functions

| Name       | Description                                | Implemented?   |
| ---------- | ------------------------------------------ | -------------- |
| fxp16_cosh  | Compute hyperbolic cosine                  | [X]            |
| fxp16_sinh  | Compute hyperbolic sine                    | [X]            |
| fxp16_tanh  | Compute hyperbolic tangent                 | [X]            |
| fxp16_acosh | Compute area hyperbolic cosine             | [ ]            |
| fxp16_asinh | Compute area hyperbolic sine               | [ ]            |
| fxp16_atanh | Compute area hyperbolic tangent            | [ ]            |

### Exponential and logarithmic functions

| Name         | Description                                                            | Implemented?   |
| ------------ | ---------------------------------------------------------------------- | -------------- |
| fxp16_exp     | Compute exponential function                                           | [ ]            |
| fxp16_frexp   | Get significand and exponent                                           | [ ]            |
| fxp16_ldexp   | Generate value from significand and exponent                           | [ ]            |
| fxp16_log     | Compute natural logarithm                                              | [ ]            |
| fxp16_log10   | Compute common logarithm                                               | [ ]            |
| fxp16_modf    | Break into fractional and integral parts                               | [ ]            |
| fxp16_exp2    | Compute binary exponential function                                    | [ ]            |
| fxp16_expm1   | Compute exponential minus one                                          | [ ]            |
| fxp16_ilogb   | Integer binary logarithm                                               | [ ]            |
| fxp16_log1p   | Compute logarithm plus one                                             | [ ]            |
| fxp16_log2    | Compute binary logarithm                                               | [ ]            |
| fxp16_logb    | Compute floating-point base logarithm                                  | [ ]            |
| fxp16_scalbn  | Scale significand using floating-point base exponent                   | [ ]            |
| fxp16_scalbln | Scale significand using floating-point base exponent (long)            | [ ]            |

### Power functions

| Name       | Description                    | Implemented?   |
| ---------- | ------------------------------ | -------------- |
| fxp16_pow   | Raise to power                 | [ ]            |
| fxp16_sqrt  | Compute square root            | [X]            |
| fxp16_cbrt  | Compute cubic root             | [ ]            |
| fxp16_hypot | Compute hypotenuse             | [ ]            |

### Error and gamma functions

| Name        | Description                                     | Implemented?   |
| ----------- | ----------------------------------------------- | -------------- |
| fxp16_erf    | Compute error function                          | [ ]            |
| fxp16_erfc   | Compute complementary error function            | [ ]            |
| fxp16_tgamma | Compute gamma function                          | [ ]            |
| fxp16_lgamma | Compute log-gamma function                      | [ ]            |

### Rounding and remainder functions

| Name           | Description                                               | Implemented?   |
| -------------- | --------------------------------------------------------- | -------------- |
| fxp16_ceil      | Round up value                                            | [X]            |
| fxp16_floor     | Round down value                                          | [X]            |
| fxp16_fmod      | Compute remainder of division                             | [X]            |
| fxp16_trunc     | Truncate value                                            | [X]            |
| fxp16_round     | Round to nearest                                          | [X]            |
| fxp16_lround    | Round to nearest and cast to long integer                 | [X]            |
| fxp16_llround   | Round to nearest and cast to long long integer            | [ ]            |
| fxp16_rint      | Round to integral value                                   | [ ]            |
| fxp16_lrint     | Round and cast to long integer                            | [ ]            |
| fxp16_llrint    | Round and cast to long long integer                       | [ ]            |
| fxp16_nearbyint | Round to nearby integral value                            | [ ]            |
| fxp16_remainder | Compute remainder (IEC 60559)                             | [ ]            |
| fxp16_remquo    | Compute remainder and quotient                            | [ ]            |

### Floating-point manipulation functions

| Name            | Description                                              | Implemented?   |
| --------------- | -------------------------------------------------------- | -------------- |
| fxp16_copysign   | Copy sign                                                | [ ]            |
| fxp16_nan        | Generate quiet NaN                                       | [ ]            |
| fxp16_nextafter  | Next representable value                                 | [ ]            |
| fxp16_nexttoward | Next representable value toward precise value            | [ ]            |

### Minimum, maximum, difference functions

| Name      | Description                    | Implemented? |
| ---- | ------------------------------ | -------------- |
| fxp16_fdim | Positive difference            | [ ]            |
| fxp16_fmax | Maximum value                  | [ ]            |
| fxp16_fmin | Minimum value                  | [ ]            |

### Other functions

| Name      | Description                       | Implemented?   |
| --------- | --------------------------------- | -------------- |
| fxp16_fabs | Compute absolute value            | [ ]            |
| fxp16_abs  | Compute absolute value            | [ ]            |
| fxp16_fma  | Multiply-add                      | [ ]            |

### Classification macro / functions (C: Macros, C++: Functions)

| Name            | Description                                    | Implemented?   |
| --------------- | ---------------------------------------------- | -------------- |
| fxp16_fpclassify | Classify floating-point value                  | [ ]            |
| fxp16_isfinite   | Is finite value                                | [ ]            |
| fxp16_isinf      | Is infinity                                    | [ ]            |
| fxp16_isnan      | Is Not-A-Number                                | [ ]            |
| fxp16_isnormal   | Is normal                                      | [ ]            |
| fxp16_signbit    | Sign bit                                       | [ ]            |

### Comparison macro / functions

| Name                | Description                 | Implemented?   |
| ------------------- | --------------------------- | -------------- |
| fxp16_isgreater      | Is greater                  | [ ]            |
| fxp16_isgreaterequal | Is greater or equal         | [ ]            |
| fxp16_isless         | Is less                     | [ ]            |
| fxp16_islessequal    | Is less or equal            | [ ]            |
| fxp16_islessgreater  | Is less or greater          | [ ]            |
| fxp16_isunordered    | Is unordered                | [ ]            |

### Macro constants

| Name                  | Description                       | Implemented? |
| --------------------- | --------------------------------- | -------------- |
| fxp16_math_errhandling | Error handling                    | [ ]            |
| FXP16_INFINITY         | Infinity                          | [ ]            |
| FXP16_NAN              | Not-A-Number                      | [ ]            |
| FXP16_HUGE_VAL         | Huge value                        | [ ]            |
| FXP16_HUGE_VALF        | Huge float value                  | [ ]            |
| FXP16_HUGE_VALL        | Huge long double value            | [ ]            |
