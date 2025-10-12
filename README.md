### CORDIC-Based Sine and Cosine Functions

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



<img width="866" height="577" alt="sincos" src="https://github.com/user-attachments/assets/ac9306cd-a352-4a4b-95b1-345c925cc03e" />



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
