/*! \copyright
    Copyright (c) 2017, marco@bacchi.at
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. The name of the author may not be used to endorse or promote
       products derived from this software without specific prior
       written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*!
    \file   fp.h

    \brief  Signed fixed point implementation for small embedded project

    \details
*/

#ifndef _FP16_H_
#define _FP16_H_

#include <stdint.h>


#define FP16_Q0             (0)
#define FP16_Q0_PREC        (1)
#define FP16_Q0_MAX     (32767)
#define FP16_Q0_MIN     (-32768)

#define FP16_Q0_M_E     (3)          /* 3.0 */
#define FP16_Q0_M_PI        (3)          /* 3.0 */
#define FP16_Q0_M_PI_2      (2)          /* 2.0 */
#define FP16_Q0_M_PI_4      (1)          /* 1.0 */
#define FP16_Q0_M_1_PI      (0)          /* 0.0 */
#define FP16_Q0_M_2_PI      (1)          /* 1.0 */
#define FP16_Q0_M_2_SQRTPI      (1)          /* 1.0 */
#define FP16_Q0_M_SQRT2     (1)          /* 1.0 */
#define FP16_Q0_M_SQRT1_2       (1)          /* 1.0 */
#define FP16_Q0_M_LOG2E     (1)          /* 1.0 */
#define FP16_Q0_M_LOG10E        (0)          /* 0.0 */
#define FP16_Q0_M_LN2       (1)          /* 1.0 */
#define FP16_Q0_M_LN10      (2)          /* 2.0 */


#define FP16_Q1             (1)
#define FP16_Q1_PREC        (0.5)
#define FP16_Q1_MAX     (16383.5)
#define FP16_Q1_MIN     (-16384.0)

#define FP16_Q1_M_E     (5)          /* 2.5 */
#define FP16_Q1_M_PI        (6)          /* 3.0 */
#define FP16_Q1_M_PI_2      (3)          /* 1.5 */
#define FP16_Q1_M_PI_4      (2)          /* 1.0 */
#define FP16_Q1_M_1_PI      (1)          /* 0.5 */
#define FP16_Q1_M_2_PI      (1)          /* 0.5 */
#define FP16_Q1_M_2_SQRTPI      (2)          /* 1.0 */
#define FP16_Q1_M_SQRT2     (3)          /* 1.5 */
#define FP16_Q1_M_SQRT1_2       (1)          /* 0.5 */
#define FP16_Q1_M_LOG2E     (3)          /* 1.5 */
#define FP16_Q1_M_LOG10E        (1)          /* 0.5 */
#define FP16_Q1_M_LN2       (1)          /* 0.5 */
#define FP16_Q1_M_LN10      (5)          /* 2.5 */


#define FP16_Q2             (2)
#define FP16_Q2_PREC        (0.25)
#define FP16_Q2_MAX     (8191.75)
#define FP16_Q2_MIN     (-8192.0)

#define FP16_Q2_M_E     (11)             /* 2.75 */
#define FP16_Q2_M_PI        (13)             /* 3.25 */
#define FP16_Q2_M_PI_2      (6)          /* 1.5 */
#define FP16_Q2_M_PI_4      (3)          /* 0.75 */
#define FP16_Q2_M_1_PI      (1)          /* 0.25 */
#define FP16_Q2_M_2_PI      (3)          /* 0.75 */
#define FP16_Q2_M_2_SQRTPI      (5)          /* 1.25 */
#define FP16_Q2_M_SQRT2     (6)          /* 1.5 */
#define FP16_Q2_M_SQRT1_2       (3)          /* 0.75 */
#define FP16_Q2_M_LOG2E     (6)          /* 1.5 */
#define FP16_Q2_M_LOG10E        (2)          /* 0.5 */
#define FP16_Q2_M_LN2       (3)          /* 0.75 */
#define FP16_Q2_M_LN10      (9)          /* 2.25 */


#define FP16_Q3             (3)
#define FP16_Q3_PREC        (0.125)
#define FP16_Q3_MAX     (4095.875)
#define FP16_Q3_MIN     (-4096.0)

#define FP16_Q3_M_E     (22)             /* 2.75 */
#define FP16_Q3_M_PI        (25)             /* 3.125 */
#define FP16_Q3_M_PI_2      (13)             /* 1.625 */
#define FP16_Q3_M_PI_4      (6)          /* 0.75 */
#define FP16_Q3_M_1_PI      (3)          /* 0.375 */
#define FP16_Q3_M_2_PI      (5)          /* 0.625 */
#define FP16_Q3_M_2_SQRTPI      (9)          /* 1.125 */
#define FP16_Q3_M_SQRT2     (11)             /* 1.375 */
#define FP16_Q3_M_SQRT1_2       (6)          /* 0.75 */
#define FP16_Q3_M_LOG2E     (12)             /* 1.5 */
#define FP16_Q3_M_LOG10E        (3)          /* 0.375 */
#define FP16_Q3_M_LN2       (6)          /* 0.75 */
#define FP16_Q3_M_LN10      (18)             /* 2.25 */


#define FP16_Q4             (4)
#define FP16_Q4_PREC        (0.0625)
#define FP16_Q4_MAX     (2047.9375)
#define FP16_Q4_MIN     (-2048.0)

#define FP16_Q4_M_E     (43)             /* 2.6875 */
#define FP16_Q4_M_PI        (50)             /* 3.125 */
#define FP16_Q4_M_PI_2      (25)             /* 1.5625 */
#define FP16_Q4_M_PI_4      (13)             /* 0.8125 */
#define FP16_Q4_M_1_PI      (5)          /* 0.3125 */
#define FP16_Q4_M_2_PI      (10)             /* 0.625 */
#define FP16_Q4_M_2_SQRTPI      (18)             /* 1.125 */
#define FP16_Q4_M_SQRT2     (23)             /* 1.4375 */
#define FP16_Q4_M_SQRT1_2       (11)             /* 0.6875 */
#define FP16_Q4_M_LOG2E     (23)             /* 1.4375 */
#define FP16_Q4_M_LOG10E        (7)          /* 0.4375 */
#define FP16_Q4_M_LN2       (11)             /* 0.6875 */
#define FP16_Q4_M_LN10      (37)             /* 2.3125 */


#define FP16_Q5             (5)
#define FP16_Q5_PREC        (0.03125)
#define FP16_Q5_MAX     (1023.96875)
#define FP16_Q5_MIN     (-1024.0)

#define FP16_Q5_M_E     (87)             /* 2.71875 */
#define FP16_Q5_M_PI        (101)            /* 3.15625 */
#define FP16_Q5_M_PI_2      (50)             /* 1.5625 */
#define FP16_Q5_M_PI_4      (25)             /* 0.78125 */
#define FP16_Q5_M_1_PI      (10)             /* 0.3125 */
#define FP16_Q5_M_2_PI      (20)             /* 0.625 */
#define FP16_Q5_M_2_SQRTPI      (36)             /* 1.125 */
#define FP16_Q5_M_SQRT2     (45)             /* 1.40625 */
#define FP16_Q5_M_SQRT1_2       (23)             /* 0.71875 */
#define FP16_Q5_M_LOG2E     (46)             /* 1.4375 */
#define FP16_Q5_M_LOG10E        (14)             /* 0.4375 */
#define FP16_Q5_M_LN2       (22)             /* 0.6875 */
#define FP16_Q5_M_LN10      (74)             /* 2.3125 */


#define FP16_Q6             (6)
#define FP16_Q6_PREC        (0.015625)
#define FP16_Q6_MAX     (511.984375)
#define FP16_Q6_MIN     (-512.0)

#define FP16_Q6_M_E     (174)            /* 2.71875 */
#define FP16_Q6_M_PI        (201)            /* 3.140625 */
#define FP16_Q6_M_PI_2      (101)            /* 1.578125 */
#define FP16_Q6_M_PI_4      (50)             /* 0.78125 */
#define FP16_Q6_M_1_PI      (20)             /* 0.3125 */
#define FP16_Q6_M_2_PI      (41)             /* 0.640625 */
#define FP16_Q6_M_2_SQRTPI      (72)             /* 1.125 */
#define FP16_Q6_M_SQRT2     (91)             /* 1.421875 */
#define FP16_Q6_M_SQRT1_2       (45)             /* 0.703125 */
#define FP16_Q6_M_LOG2E     (92)             /* 1.4375 */
#define FP16_Q6_M_LOG10E        (28)             /* 0.4375 */
#define FP16_Q6_M_LN2       (44)             /* 0.6875 */
#define FP16_Q6_M_LN10      (147)            /* 2.296875 */


#define FP16_Q7             (7)
#define FP16_Q7_PREC        (0.0078125)
#define FP16_Q7_MAX     (255.9921875)
#define FP16_Q7_MIN     (-256.0)

#define FP16_Q7_M_E     (348)            /* 2.71875 */
#define FP16_Q7_M_PI        (402)            /* 3.140625 */
#define FP16_Q7_M_PI_2      (201)            /* 1.5703125 */
#define FP16_Q7_M_PI_4      (101)            /* 0.7890625 */
#define FP16_Q7_M_1_PI      (41)             /* 0.3203125 */
#define FP16_Q7_M_2_PI      (81)             /* 0.6328125 */
#define FP16_Q7_M_2_SQRTPI      (144)            /* 1.125 */
#define FP16_Q7_M_SQRT2     (181)            /* 1.4140625 */
#define FP16_Q7_M_SQRT1_2       (91)             /* 0.7109375 */
#define FP16_Q7_M_LOG2E     (185)            /* 1.4453125 */
#define FP16_Q7_M_LOG10E        (56)             /* 0.4375 */
#define FP16_Q7_M_LN2       (89)             /* 0.6953125 */
#define FP16_Q7_M_LN10      (295)            /* 2.3046875 */


#define FP16_Q8             (8)
#define FP16_Q8_PREC        (0.00390625)
#define FP16_Q8_MAX     (127.99609375)
#define FP16_Q8_MIN     (-128.0)

#define FP16_Q8_M_E     (696)            /* 2.71875 */
#define FP16_Q8_M_PI        (804)            /* 3.140625 */
#define FP16_Q8_M_PI_2      (402)            /* 1.5703125 */
#define FP16_Q8_M_PI_4      (201)            /* 0.78515625 */
#define FP16_Q8_M_1_PI      (81)             /* 0.31640625 */
#define FP16_Q8_M_2_PI      (163)            /* 0.63671875 */
#define FP16_Q8_M_2_SQRTPI      (289)            /* 1.12890625 */
#define FP16_Q8_M_SQRT2     (362)            /* 1.4140625 */
#define FP16_Q8_M_SQRT1_2       (181)            /* 0.70703125 */
#define FP16_Q8_M_LOG2E     (369)            /* 1.44140625 */
#define FP16_Q8_M_LOG10E        (111)            /* 0.43359375 */
#define FP16_Q8_M_LN2       (177)            /* 0.69140625 */
#define FP16_Q8_M_LN10      (589)            /* 2.30078125 */


#define FP16_Q9             (9)
#define FP16_Q9_PREC        (0.001953125)
#define FP16_Q9_MAX     (63.998046875)
#define FP16_Q9_MIN     (-64.0)

#define FP16_Q9_M_E     (1392)           /* 2.71875 */
#define FP16_Q9_M_PI        (1608)           /* 3.140625 */
#define FP16_Q9_M_PI_2      (804)            /* 1.5703125 */
#define FP16_Q9_M_PI_4      (402)            /* 0.78515625 */
#define FP16_Q9_M_1_PI      (163)            /* 0.318359375 */
#define FP16_Q9_M_2_PI      (326)            /* 0.63671875 */
#define FP16_Q9_M_2_SQRTPI      (578)            /* 1.12890625 */
#define FP16_Q9_M_SQRT2     (724)            /* 1.4140625 */
#define FP16_Q9_M_SQRT1_2       (362)            /* 0.70703125 */
#define FP16_Q9_M_LOG2E     (739)            /* 1.443359375 */
#define FP16_Q9_M_LOG10E        (222)            /* 0.43359375 */
#define FP16_Q9_M_LN2       (355)            /* 0.693359375 */
#define FP16_Q9_M_LN10      (1179)           /* 2.302734375 */


#define FP16_Q10            (10)
#define FP16_Q10_PREC       (0.0009765625)
#define FP16_Q10_MAX        (31.9990234375)
#define FP16_Q10_MIN        (-32.0)

#define FP16_Q10_M_E        (2784)           /* 2.71875 */
#define FP16_Q10_M_PI       (3217)           /* 3.1416015625 */
#define FP16_Q10_M_PI_2     (1608)           /* 1.5703125 */
#define FP16_Q10_M_PI_4     (804)            /* 0.78515625 */
#define FP16_Q10_M_1_PI     (326)            /* 0.318359375 */
#define FP16_Q10_M_2_PI     (652)            /* 0.63671875 */
#define FP16_Q10_M_2_SQRTPI     (1155)           /* 1.1279296875 */
#define FP16_Q10_M_SQRT2        (1448)           /* 1.4140625 */
#define FP16_Q10_M_SQRT1_2      (724)            /* 0.70703125 */
#define FP16_Q10_M_LOG2E        (1477)           /* 1.4423828125 */
#define FP16_Q10_M_LOG10E       (445)            /* 0.4345703125 */
#define FP16_Q10_M_LN2      (710)            /* 0.693359375 */
#define FP16_Q10_M_LN10     (2358)           /* 2.302734375 */


#define FP16_Q11            (11)
#define FP16_Q11_PREC       (0.00048828125)
#define FP16_Q11_MAX        (15.99951171875)
#define FP16_Q11_MIN        (-16.0)

#define FP16_Q11_M_E        (5567)           /* 2.71826171875 */
#define FP16_Q11_M_PI       (6434)           /* 3.1416015625 */
#define FP16_Q11_M_PI_2     (3217)           /* 1.57080078125 */
#define FP16_Q11_M_PI_4     (1608)           /* 0.78515625 */
#define FP16_Q11_M_1_PI     (652)            /* 0.318359375 */
#define FP16_Q11_M_2_PI     (1304)           /* 0.63671875 */
#define FP16_Q11_M_2_SQRTPI     (2311)           /* 1.12841796875 */
#define FP16_Q11_M_SQRT2        (2896)           /* 1.4140625 */
#define FP16_Q11_M_SQRT1_2      (1448)           /* 0.70703125 */
#define FP16_Q11_M_LOG2E        (2955)           /* 1.44287109375 */
#define FP16_Q11_M_LOG10E       (889)            /* 0.43408203125 */
#define FP16_Q11_M_LN2      (1420)           /* 0.693359375 */
#define FP16_Q11_M_LN10     (4716)           /* 2.302734375 */


#define FP16_Q12            (12)
#define FP16_Q12_PREC       (0.000244140625)
#define FP16_Q12_MAX        (7.999755859375)
#define FP16_Q12_MIN        (-8.0)

#define FP16_Q12_M_E        (11134)          /* 2.71826171875 */
#define FP16_Q12_M_PI       (12868)          /* 3.1416015625 */
#define FP16_Q12_M_PI_2     (6434)           /* 1.57080078125 */
#define FP16_Q12_M_PI_4     (3217)           /* 0.785400390625 */
#define FP16_Q12_M_1_PI     (1304)           /* 0.318359375 */
#define FP16_Q12_M_2_PI     (2608)           /* 0.63671875 */
#define FP16_Q12_M_2_SQRTPI     (4622)           /* 1.12841796875 */
#define FP16_Q12_M_SQRT2        (5793)           /* 1.414306640625 */
#define FP16_Q12_M_SQRT1_2      (2896)           /* 0.70703125 */
#define FP16_Q12_M_LOG2E        (5909)           /* 1.442626953125 */
#define FP16_Q12_M_LOG10E       (1779)           /* 0.434326171875 */
#define FP16_Q12_M_LN2      (2839)           /* 0.693115234375 */
#define FP16_Q12_M_LN10     (9431)           /* 2.302490234375 */


#define FP16_Q13            (13)
#define FP16_Q13_PREC       (0.0001220703125)
#define FP16_Q13_MAX        (3.9998779296875)
#define FP16_Q13_MIN        (-4.0)

#define FP16_Q13_M_E        (22268)          /* 2.71826171875 */
#define FP16_Q13_M_PI       (25736)          /* 3.1416015625 */
#define FP16_Q13_M_PI_2     (12868)          /* 1.57080078125 */
#define FP16_Q13_M_PI_4     (6434)           /* 0.785400390625 */
#define FP16_Q13_M_1_PI     (2608)           /* 0.318359375 */
#define FP16_Q13_M_2_PI     (5215)           /* 0.6365966796875 */
#define FP16_Q13_M_2_SQRTPI     (9244)           /* 1.12841796875 */
#define FP16_Q13_M_SQRT2        (11585)          /* 1.4141845703125 */
#define FP16_Q13_M_SQRT1_2      (5793)           /* 0.7071533203125 */
#define FP16_Q13_M_LOG2E        (11819)          /* 1.4427490234375 */
#define FP16_Q13_M_LOG10E       (3558)           /* 0.434326171875 */
#define FP16_Q13_M_LN2      (5678)           /* 0.693115234375 */
#define FP16_Q13_M_LN10     (18863)          /* 2.3026123046875 */


#define FP16_Q14            (14)
#define FP16_Q14_PREC       (6.103515625e-05)
#define FP16_Q14_MAX        (1.99993896484375)
#define FP16_Q14_MIN        (-2.0)

#define FP16_Q14_M_E        (32767)          /* 1.99993896484375 */
#define FP16_Q14_M_PI       (32767)          /* 1.99993896484375 */
#define FP16_Q14_M_PI_2     (25736)          /* 1.57080078125 */
#define FP16_Q14_M_PI_4     (12868)          /* 0.785400390625 */
#define FP16_Q14_M_1_PI     (5215)           /* 0.31829833984375 */
#define FP16_Q14_M_2_PI     (10430)          /* 0.6365966796875 */
#define FP16_Q14_M_2_SQRTPI     (18487)          /* 1.12835693359375 */
#define FP16_Q14_M_SQRT2        (23170)          /* 1.4141845703125 */
#define FP16_Q14_M_SQRT1_2      (11585)          /* 0.70709228515625 */
#define FP16_Q14_M_LOG2E        (23637)          /* 1.44268798828125 */
#define FP16_Q14_M_LOG10E       (7115)           /* 0.43426513671875 */
#define FP16_Q14_M_LN2      (11357)          /* 0.69317626953125 */
#define FP16_Q14_M_LN10     (32767)          /* 1.99993896484375 */


#define FP16_Q15            (15)
#define FP16_Q15_PREC       (3.0517578125e-05)
#define FP16_Q15_MAX        (0.999969482421875)
#define FP16_Q15_MIN        (-1.0)

#define FP16_Q15_M_E        (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_PI       (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_PI_2     (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_PI_4     (25736)          /* 0.785400390625 */
#define FP16_Q15_M_1_PI     (10430)          /* 0.31829833984375 */
#define FP16_Q15_M_2_PI     (20861)          /* 0.636627197265625 */
#define FP16_Q15_M_2_SQRTPI     (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_SQRT2        (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_SQRT1_2      (23170)          /* 0.70709228515625 */
#define FP16_Q15_M_LOG2E        (32767)          /* 0.999969482421875 */
#define FP16_Q15_M_LOG10E       (14231)          /* 0.434295654296875 */
#define FP16_Q15_M_LN2      (22713)          /* 0.693145751953125 */
#define FP16_Q15_M_LN10     (32767)          /* 0.999969482421875 */




#define FP16_TRIG_TAB_SIZE           (64) /* 64,32 or 16 */
#define FP16_TRIG_SIN_TAB_RES        (65536/FP16_TRIG_TAB_SIZE)
#define FP16_TRIG_ASIN_TAB_RES       (32768/FP16_TRIG_TAB_SIZE)

#define FP16_TRIG_Q15_ONE_HALF           (16384)
#define FP16_TRIG_Q15_MINUS_ONE_HALF     (-16384)

#define FP16_TRIG_Q14_ONE               (16384)
#define FP16_TRIG_Q14_MINUS_ONE         (-16384)

#define FP16_EXP_TAYLOR_ORDER           10

typedef int16_t fp16_t; /*!< Type can be used for all Q configurations */


/*!
    \brief      Macro that saturates a result to fixed point limits
    \details    This macro checks if the int32_t result of a previously performed calculation
                results in a fixed point overflow. If so the result is saturated to the
                extends of the fixed point type.
    \param[in]  var     Fixed point int32_t result to be saturated
*/
#define fp16_sat_m(var) \
    do{ \
        if (var > INT16_MAX) { var = INT16_MAX; } \
        if (var < INT16_MIN) { var = INT16_MIN; } \
    }while(0)


/*!
  \brief      Macro to right shift fixed point number
  \details    Macro to right shift fixed point number
  \param      result   Fixed point number to right shift (int32_t format)
  \param      shift    Number of bits to right shift


*/
#define fp16_rshift_m(result,shift)               \
 do{                                              \
    if (shift > 0 )                               \
    {                                             \
       if ( fp16_signbit(result) )                \
       {                                          \
          result = (-result)>>(shift-1);          \
          result = -((result>>1)+(result&1));     \
       }                                          \
       else                                       \
       {                                          \
          result >>= (shift-1);                   \
          result = (result>>1) + (result&1);      \
       }                                          \
    }                                             \
 }while(0)


/*!
  \brief      Macro to left shift fixed point number
  \details    Macro to left shift fixed point number
  \param      result   Fixed point number to left shift (int32_t format)
  \param      shift    Number of bits to left shift

*/
#define fp16_lshift_m(result,shift) do{ result<<=shift; }while(0)


/*!
  \brief      Macro to shift fixed point number in any direction
  \details    Macro to shift fixed point number in any direction
  \param      result   Fixed point number to left shift (int32_t format)
  \param      shift    Number of bits to shift (+ = rshift, - = lshift)
*/
#define fp16_shift_m(result,shift)                \
 do {                                             \
    if ( shift > 0 )                              \
    {                                             \
       if ( fp16_signbit(result) )                \
       {                                          \
          result = (-result)>>(shift-1);          \
          result = -((result>>1)+(result&1));     \
       }                                          \
       else                                       \
       {                                          \
          result >>= (shift-1);                   \
          result = (result>>1) + (result&1);      \
       }                                          \
    }                                             \
    else if( shift < 0 )                          \
    {                                             \
       fp16_lshift_m(result,shift);               \
    }                                             \
 }while(0)


/*!
   \brief      Macro to perform an integer cast on a fixed point number
   \details    Macro to perform an integer cast on a fixed point number
   \param      result   Fixed point number to left cast to int
   \param      frac     Number of fractional bits
*/
#define fp16_intcast_m(result,frac)               \
 do {                                             \
   if ( fp16_signbit(result) )                    \
   {                                              \
      result = -((-result)&~((1<<frac)-1));       \
   }                                              \
   else                                           \
   {                                              \
      result&=~((1<<frac)-1);                     \
   }                                              \
 }while(0)


/* fixed point conversions */
fp16_t fp16_flt2fp(float var, uint8_t frac);
float fp16_fp2flt(fp16_t var, uint8_t frac);
fp16_t fp16_int2fp(int16_t intpart, uint8_t frac);
fp16_t fp16_fp2fp(fp16_t fp, uint8_t fracold, uint8_t fracnew);
fp16_t fp16_rshift(fp16_t fp, uint8_t shift);
fp16_t fp16_lshift(fp16_t fp, uint8_t shift);


/*!
    \brief      Compute square root
    \details    Returns the square root of x.
                If a domain error occurs, the global variable errno is set to EDOM.
    \param      fp  Fixed point variable to be taken square root of
    \param      frac     Number of fracional bits

    \returns     Returns the square root of x, with the same number of fractional bits as x.
*/
fp16_t fp16_sqrt(fp16_t fp, uint8_t frac);


/*!
    \brief      Converts a fixed point type to integer
    \details    Converts a fixed point variable to an integer variable.
    \param[in]  fp  Fixed point variable to be converted
    \param[in]  frac     Number of fracional bits

    \returns Integer interpretation of provided fixed point number
*/
#define fp16_fp2int(fp,frac) fp16_lround(fp,frac)



/* Basic math operations */
fp16_t fp16_add(fp16_t summand1, fp16_t summand2);
fp16_t fp16_sub(fp16_t minuend, fp16_t subtrahend);
fp16_t fp16_mult(fp16_t mult1, uint8_t frac1, fp16_t mult2, uint8_t frac2);
fp16_t fp16_div(fp16_t divident, uint8_t frac1, fp16_t divisor, uint8_t frac2);



/* Trigonometric functions */

extern const int16_t *fp16_cos_tab;
extern const int16_t fp16_sin_tab[];

fp16_t fp16_sin_cos_helper(fp16_t fp, const int16_t *tab);

/*!
    \brief      Computes cosine of a provided angle
    \details    Computes cosine of a provided angle. The angle must be fixed point number in Q1.15 format.
                The allowed range is from -1.0 to 0.999969482421875, representing an angle from
                -PI to +PI in radians minus the fraction caused by the last LSB up to +PI.

                The result returned is of fixed point type Q14 [-1.0,+1.0]

    \param[in]  fp        The angle must be fixed point number in Q1.15 format [-1.0,+1.0-LSB]

    \returns The result returned is of fixed point type Q2.14 [-1.0,+1.0]
*/
#define fp16_cos(fp) fp16_sin_cos_helper(fp,fp16_cos_tab)

/*!
    \brief      Computes sine of a provided angle
    \details    Computes sine of a provided angle. The angle must be fixed point number in Q1.15 format.
                The allowed range is from -1.0 to 0.999969482421875, representing an angle from
                -PI to +PI in radians minus the fraction caused by the last LSB up to +PI.

                The result returned is of fixed point type Q14 [-1.0,+1.0]

    \param[in]  fp        The angle must be fixed point number in Q1.15 format [-1.0,+1.0-LSB]

    \returns The result returned is of fixed point type Q2.14 [-1.0,+1.0]
*/
#define fp16_sin(fp) fp16_sin_cos_helper(fp,fp16_sin_tab)





fp16_t fp16_tan(fp16_t fp, uint8_t frac);

fp16_t fp16_asin(fp16_t fp);
#define fp16_acos(fp) (FP16_Q13_M_PI_2 - fp16_fp2fp(fp16_asin(fp),FP16_Q14,FP16_Q13))
fp16_t fp16_atan(fp16_t fp, uint8_t frac);



fp16_t fp16_exp(fp16_t fp, uint8_t frac);
fp16_t fp16_log(fp16_t fp, uint8_t frac);



/* Rounding and remainder functions */


/*!
    \brief      Round up value
    \details    Rounds x upward, returning the smallest integral value that is not less than x.


    \param      x       Value to round up.
    \param      xfrac   Number of fractional bits for x.
    \return     The smallest integral value that is not less than x (as a fixed-point value).
*/

fp16_t fp16_ceil(fp16_t x, uint8_t xfrac);


/*!
    \brief      Round down value
    \details    Rounds x downward, returning the largest integral value that is not greater than x.

    \param      x       Value to round down.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded downward (as a fixed-point value).
*/
#define fp16_floor(x,xfrac) (x&~((1<<xfrac)-1))

/*!
    \brief      Truncate value
    \details    Rounds x toward zero, returning the nearest integral value that is not larger in magnitude than x.


    \param      x       Value to truncate.
    \param      xfrac   Number of fractional bits for x.
    \return     The nearest integral value that is not larger in magnitude than x (as a fixed-point value).
*/
#define fp16_trunc(x,xfrac) ((x>0)?fp16_floor(x,xfrac):fp16_ceil(x,xfrac))

/*!
    \brief      Round to nearest
    \details    Returns the integral value that is nearest to x, with halfway cases rounded away from zero.

    \param      x       Value to round.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded to the nearest integral (as a fixed-point value).
*/
fp16_t fp16_round(fp16_t x, uint8_t xfrac);

/*!
    \brief      Compute remainder of division
    \details    Returns the floating-point remainder of numer/denom (rounded towards zero):

                fmod = numer - tquot * denom

                Where tquot is the truncated (i.e., rounded towards zero) result of: numer/denom.

                 A similar function, remainder, returns the same but with the quotient rounded to the nearest integer (instead of truncated).

                 In the C Programming Language, the fmod function returns the remainder when x is divided by y.



    \param      x       Value of the quotient numerator.
    \param      xfrac   Number of fractional bits for x.
    \param      y       Value of the quotient denominator.
    \param      yfrac   Number of fractional bits for y.


    \return     The remainder of dividing the arguments. If denom is zero, the function may either
                return zero or cause a domain error (depending on the library implementation).
*/
fp16_t fp16_fmod(fp16_t divident, uint8_t frac1, fp16_t divisor, uint8_t frac2);


/*!
    \brief      Round to nearest and cast to integer
    \details    Returns the integer value that is nearest in value to x, with halfway cases rounded away from zero.

    \param      x       Value to round.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded to the nearest integral, casted to a value of type int.
*/
int fp16_lround(fp16_t fp, uint8_t frac);





/* Fixed-point manipulation functions */

/*!
    \brief      Copy sign
    \details    Returns a value with the magnitude of x and the sign of y.
                x and y must not have the same amount of fractional bits.

    \param      x   Value with the magnitude of the resulting value.
    \param      y   Value with the sign of the resulting value.
    \return     The value with a magnitude of x and the sign of y.
*/
fp16_t fp16_copysign(fp16_t x, fp16_t y);

/*!
    \brief      Next representable value
    \details    Returns the next representable value after x in the direction of y.
                x and y must have the same amount of fractional bits.

    \param      x   Base value.
    \param      y   Value toward which the return value is approximated.
    \return     The next representable value after x in the direction of y.
*/

#define fp16_nextafter(x,y) ((fp16_isless(x,y))?(x+1):((fp16_isgreater(x,y))?(x-1):(y)))




/* Minimum, maximum, difference functions */


/*!
    \brief      Positive difference
    \details    The function returns x-y if x>y, and zero otherwise.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The positive difference between x and y.
*/
#define fp16_fdim(x,y) ((fp16_isgreater(x,y))?(fp16_sub(x,y)):(0))




/*!
    \brief      Maximum value
    \details    Returns the larger of its arguments: either x or y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The maximum numeric value of its arguments.
*/
#define fp16_fmax(x,y) (fp16_isgreaterequal(x,y)?(x):(y))


/*!
    \brief      Minimum value
    \details    Returns the smaller of its arguments: either x or y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The minimum numeric value of its arguments.
*/
#define fp16_fmin(x,y) (fp16_islessequal(x,y)?(x):(y))



/* functions to compute common mathematical operations and transformations */
fp16_t fp16_fabs (fp16_t x);
fp16_t fp16_abs (fp16_t x, uint8_t frac);
fp16_t fp16_fma (fp16_t x, uint8_t xfrac, fp16_t y, uint8_t yfrac, fp16_t z, uint8_t zfrac);


/* Comparison macro / functions */

/*!
    \brief      Is greater
    \details    Returns whether x is greater than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is greater than y. false (0) otherwise.
*/
#define fp16_isgreater(x,y)  ((x)>(y))

/*!
    \brief      Is greater or equal
    \details    Returns whether x is greater than or equal to y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is greater than or equal to y. false (0) otherwise.
*/
#define fp16_isgreaterequal(x,y)  ((x)>=(y))

/*!
    \brief      Is less
    \details    Returns whether x is less than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than y. false (0) otherwise.
*/
#define fp16_isless(x,y)  ((x)<(y))


/*!
    \brief      Is less or equal
    \details    Returns whether x is less than or equal to y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than or equal to y. false (0) otherwise.
*/
#define fp16_islessequal(x,y)  ((x)<=(y))


/*!
    \brief      Is less or greater
    \details    Returns whether x is less than or greater than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than or greater than y. false (0) otherwise.
*/
#define fp16_islessgreater(x,y)   (x!=y)



/* Classification macro / functions */


/*!
    \brief      Sign bit
    \details    Returns whether the sign of x is negative.

    \param      x   fixed point numer to get the sign from
    \return     true (1) if negative, false (0) otherwise
*/
#define fp16_signbit(x) (x<0)

#endif /* _FP16_H_ */
