# fp16
16 bit fixed point math library  

## Implemented :
### Fixed point conversions  
|Function|Brief documentation|
|:-|:-|
|fp16_flt2fp|Converts a float to a fixed point type|
|fp16_fp2flt|Converts a a fixed point type to float|  
|fp16_int2fp|Converts an integer type to fixed point|
|fp16_fp2int|Converts a fixed point type to integer|
|fp16_rshift|Right shifts fixed point number with rounding LSB|
|fp16_lshift|Left shifts fixed point number|
|fp16_shift_m|Macro to shift fixed point number in any direction|
|fp16_lshift_m|Macro to left shift fixed point number|
|fp16_rshift_m|Macro to right shift fixed point number|
|fp16_intcast_m|Macro to perform an integer cast on a fixed point number|
|fp16_fp2fp|Converts between fixed point number formats (Qx.y)|

### Basic math operations 
|Function|Brief documentation|
|:-|:-|
fp16_add|Adds two fixed point numbers|
fp16_sub|Subtracts two fixed point numbers|
fp16_mult|Multiplies two fixed point numbers|
fp16_div|Divides two fixed point numbers|

### Rounding and remainder functions
|Function|Brief documentation|
|:-|:-|
|fp16_ceil|Round up value|
|fp16_floor|Round down value|
|fp16_trunc|Truncate value|
|fp16_round|Round to nearest|
|fp16_fmod|Compute remainder of division|

### Trigonometric functions
|Function|Brief documentation|
|:-|:-|
|fp16_cos|Compute cosine|
|fp16_sin|Compute sine|
|fp16_tan|Compute tangent|
|fp16_acos|Compute arc cosine|
|fp16_asin|Compute arc sine|
|fp16_atan|Compute arc tangent|

### Hyperbolic functions
|Function|Brief documentation|
|:-|:-|
|fp16_cosh|Compute hyperbolic cosine|
|fp16_sinh|Compute hyperbolic sine|
|fp16_tanh|Compute hyperbolic tangent|

### Exponential and logarithmic functions
|Function|Brief documentation|
|:-|:-|
|fp16_exp|Compute exponential function|
|fp16_log|Compute natural logarithm|
|fp16_log10|Compute common logarithm|
|fp16_log2|Compute binary logarithm|


### Power functions
|Function|Brief documentation|
|:-|:-|
|fp16_pow|Raise to power|
|fp16_sqrt|Compute square root|
|fp16_cbrt|Compute cubic root|
|fp16_hypot|Compute hypotenuse|

### Fixed-point manipulation functions
|Function|Brief documentation|
|:-|:-|
|fp16_copysign|Copy sign|
|fp16_nextafter| Next representable value|

### Minimum, maximum, difference functions
|Function|Brief documentation|
|:-|:-|
|fp16_fdim|Positive difference|
|fp16_fmax|Maximum value|
|fp16_fmin|Minimum value|

### Comparison macro / functions 
|Function|Brief documentation|
|:-|:-|
|fp16_isgreater|Is greater|
|fp16_isgreaterequal|Is greater or equal|
|fp16_isless|Is less|
|fp16_islessequal|Is less or equal|
|fp16_islessgreater|Is less or greater|

### Common mathematical operations and transformations 
|Function|Brief documentation|
|:-|:-|
|fp16_fabs|Compute absolute value|
|fp16_abs|Compute absolute value (operates on int values)|
|fp16_fma|Multiply-add|

### Classification macro / functions 
|Function|Brief documentation|
|:-|:-|
|fp16_signbit|Sign bit|


## Unitests completed so far :

```
Keys :

<UTTCF> : Unittest Tescase Failure 
<UTTCB> : Unittest Tescase Begin 
<UTTSB> : Unittest Testsuite Begin 
<UTTCE> : Unittest Testscase End 
<UTTSE> : Unittest Testsuite Begin     
Trailing 0 indicates no failed testcases    

Testrun :

<UTTSB> fp16 "../unittest_fp16.c" "Jun  3 2022" "18:45:24"
<UTTCB> fp16 fp16_check_shiftops
<UTTCE> fp16 fp16_check_shiftops 0
<UTTCB> fp16 fp16_precision
<UTTCE> fp16 fp16_precision 0
<UTTCB> fp16 fp16_ftofp_fptof
<UTTCE> fp16 fp16_ftofp_fptof 0
<UTTCB> fp16 fp16_floor
<UTTCE> fp16 fp16_floor 0
<UTTCB> fp16 fp16_ceil
<UTTCE> fp16 fp16_ceil 0
<UTTCB> fp16 fp16_trunc
<UTTCE> fp16 fp16_trunc 0
<UTTCB> fp16 fp16_fabs
<UTTCE> fp16 fp16_fabs 0
<UTTCB> fp16 fp16_abs
<UTTCE> fp16 fp16_abs 0
<UTTCB> fp16 fp16_fmod
<UTTCE> fp16 fp16_fmod 0
<UTTCB> fp16 fp16_fp2int
<UTTCE> fp16 fp16_fp2int 0
<UTTCB> fp16 fp16_int2fp
<UTTCE> fp16 fp16_int2fp 0
<UTTCB> fp16 fp16_round
<UTTCE> fp16 fp16_round 0
<UTTCB> fp16 fp16_lround
<UTTCE> fp16 fp16_lround 0
<UTTCB> fp16 fp16_sqrt
<UTTCE> fp16 fp16_sqrt 0
<UTTCB> fp16 fp16_cbrt
<UTTCE> fp16 fp16_cbrt 0
<UTTCB> fp16 fp16_hypot
<UTTCE> fp16 fp16_hypot 0
<UTTCB> fp16 fp16_sin
<UTTCE> fp16 fp16_sin 0
<UTTCB> fp16 fp16_cos
<UTTCE> fp16 fp16_cos 0
<UTTCB> fp16 fp16_asin
<UTTCE> fp16 fp16_asin 0
<UTTCB> fp16 fp16_acos
<UTTCE> fp16 fp16_acos 0
<UTTCB> fp16 fp16_atan
<UTTCE> fp16 fp16_atan 0
<UTTCB> fp16 fp16_sinh
<UTTCE> fp16 fp16_sinh 0
<UTTCB> fp16 fp16_cosh
<UTTCE> fp16 fp16_cosh 0
<UTTCB> fp16 fp16_tanh
<UTTCE> fp16 fp16_tanh 0
<UTTCB> fp16 fp16_exp
<UTTCE> fp16 fp16_exp 0
<UTTCB> fp16 fp16_log
<UTTCE> fp16 fp16_log 0
<UTTCB> fp16 fp16_log10
<UTTCE> fp16 fp16_log10 0
<UTTCB> fp16 fp16_log2
<UTTCE> fp16 fp16_log2 0
<UTTCB> fp16 fp16_pow
<UTTCE> fp16 fp16_pow 0
<UTTSE> fp16 0


```
