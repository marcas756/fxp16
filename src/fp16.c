/*! \copyright
    Copyright (c) 2017-2022, marco@bacchi.at
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
    \file   fp.c

    \brief  Signed fixed point implementation for small embedded project

    \details
*/

#include "fp16.h"
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>



/*!
    \brief      Converts a float to a fixed point type
    \details    Converts a float variable to a fixed point variable. Result gets
                saturated if float exceeds fixed point limits.
    \param[in]  var     Float variable to be converted
    \param[in]  frac    Number of fracional bits

    \returns    Fixed point interpretation of provided floating point number
*/
fp16_t fp16_flt2fp(float var, uint8_t frac)
{
   /*

   To convert from floating-point to fixed-point, we follow this algorithm:

   Calculate x = floating_input * 2^(fractional_bits)
   Round x to the nearest whole number (e.g. round(x))
   Store the rounded x in an integer container

   */

    var = round(var * (1 << frac));
    fp16_sat_m(var);
    return (fp16_t)var;
}

/*!
    \brief      Converts a a fixed point type to float
    \details    Converts a fixed point variable to a float variable.
    \param[in]  var     Fixed point variable to be converted
    \param[in]  frac    Number of fracional bits

    \returns Floating point interpretation of provided fixed point number
*/
float fp16_fp2flt(fp16_t var, uint8_t frac)
{
    return ((float)(var)) / (1 << frac);


}


/*!
    \brief      Converts an integer type to fixed point
    \details    Converts an integer variable to a fixed point variable.
    \param[in]  intpart  Integer variable to be converted
    \param[in]  frac     Number of fracional bits

    \returns Fixed point interpretation of provided integer number
*/
fp16_t fp16_int2fp(int16_t intpart, uint8_t frac)
{

    int32_t result = intpart;
    result <<= frac;
    fp16_sat_m(result);
    return(fp16_t)result;
}


#if FP16CONF_ARSHIFT_W_ROUNDING

fp16_t fp16_arshift(fp16_t fp, uint8_t shift)
{
   fpxx_arshift_m(fp,shift);
   return fp;
}

fp32_t fp32_arshift(fp32_t var,uint8_t rshift)
{
    fpxx_arshift_m(var,rshift);
    return var;
}

#endif

/*!
    \brief      Left shifts fixed point number
    \details    Left shifts fixed point number

                A Left Arithmetic Shift of one position moves each bit to the left by one.
                The vacant least significant bit (LSB) is filled with zero and the most
                significant bit (MSB) is discarded. It is identical to Left Logical Shift.

    \param      fp       Fixed point number to left shift
    \param      shift     Number of bits to left shift

    \returns Left shifted fixed point number
*/
fp16_t fp16_alshift(fp16_t fp, uint8_t shift)
{
   fp32_t result = fp;
   result<<=shift;
   fp16_sat_m(result);
   return(fp16_t)result;
}



/*!
    \brief      Converts between fixed point number formats (Qx.y)
    \details    Converts between fixed point number formats (Qx.y)
    \param      fp       fixed point number to convert
    \param      fracold     Old number of fracional bits
    \param      fracnew     New number of fracional bits
    \returns    Fixed point number in new fixed point format
*/

fp16_t fp16_fp2fp(fp16_t fp, uint8_t fracold, uint8_t fracnew)
{
    fp32_t result = fp;
    fpxx_ashift_m(result, fracold - fracnew);
    fp16_sat_m(result);
    return (fp16_t) result;
}


fp16_t fp16_sat(fp32_t fp32)
{
    fp16_sat_m(fp32);
    return (fp16_t) fp32;
}



/*!
    \brief      Adds two fixed point numbers
    \details    Adds two fixed point numbers. Result gets saturated if it exceeds fixed point limits.

                result = a + b

                Be aware of that both fixed point numbers must be of same type (same number of fractional bits)!
                Otherwise the result may be undefined.

    \param[in]  summand1       First fixed point summand
    \param[in]  summand2       Second fixed point summand


    \returns Sum of first summand and second summand in fixed point format
*/
fp16_t fp16_add(fp16_t summand1, fp16_t summand2)
{
    int32_t result;
    result = summand1+summand2;
    fp16_sat_m(result);
    return(fp16_t)result;
}

/*!
    \brief     Subtracts two fixed point numbers
    \details    Adds two fixed point number. Result gets saturated if it exceeds fixed point limits.

                result = a - b

                Be aware of that both fixed point numbers must be of same type (same number of fractional bits)!
                Otherwise the result may be undefined.

    \param[in]  minuend     Minuend
    \param[in]  subtrahend  Subtrahend


    \returns Difference of minuend and subtrahend in fixed point format
*/

fp16_t fp16_sub(fp16_t minuend, fp16_t subtrahend)
{
   int32_t result;
   result = minuend-subtrahend;
   fp16_sat_m(result);
   return(fp16_t)result;
}





/*!
    \brief      Multiplies two fixed point numbers
    \details    Multiplies two fixed point numbers. The numbers may be of different fixed point format.
                The result is in the fixed point format of the multiplicand (1st fixed point parameter).
                Result gets saturated if it exceeds fixed point limits.

                When performing an integer multiplication the product is 2xWL if both the multiplier and
                multiplicand are WL long. If the integer multiplication is on fixed-point variables, the number of
                integer and fractional bits in the product is the sum of the corresponding multiplier and
                multiplicand Q-points.

                result = a*b


    \param[in]  mult1     multiplicator
    \param[in]  mult2     multiplicant


    \returns Product of multiplicator and multiplicant in fixed point format of multiplicator
*/
fp16_t fp16_mult(fp16_t mult1, uint8_t frac1, fp16_t mult2, uint8_t frac2)
{
    fp32_t result = fp32_arshift((fp32_t)mult1*(fp32_t)mult2,frac2);
    fp16_sat_m(result);
    return (fp16_t)result;
}


/*!
    \brief      Divides two fixed point numbers
    \details    Divides two fixed point numbers

                result = a/b


    \param      divident     divident
    \param      divisor      divisor


    \returns divident/divisor with fractional bits of divident
*/
fp16_t fp16_div(fp16_t divident, uint8_t frac1, fp16_t divisor, uint8_t frac2)
{
  fp32_t result = (divident<<frac2)/divisor;
  fp16_sat_m(result);
  return (fp16_t)result;
}



fp16_t fp16_ceil(fp16_t x, uint8_t xfrac)
{

   fp32_t result = (fp32_t)x&~((1<<xfrac)-1);

   if(result == x)
   {
      return (fp16_t)result;
   }

   result+=(1<<(xfrac));
   fp16_sat_m(result);

   return (fp16_t)result;

}


fp16_t fp16_round(fp16_t x, uint8_t xfrac)
{
   int32_t result = x;

   if (xfrac == 0 )
   {
      return x;
   }

   if( x < 0 )
   {
      result = -result;
      result += (1<<(xfrac-1));
      result &= ~((1<<xfrac)-1);
      result = -result;
   }
   else
   {
      result += (1<<(xfrac-1));
      result &= ~((1<<xfrac)-1);
   }


   fp16_sat_m(result);
   return (fp16_t)result;
}


fp16_t fp16_fmod(fp16_t x, uint8_t xfrac, fp16_t y, uint8_t yfrac)
{
   if ( y == 0 )
   {
      return 0;
   }

   int32_t result = (x<<yfrac)/y;
   fp16_intcast_m(result,xfrac);

   result*=y;
   fpxx_arshift_m(result,yfrac);

   result = x - result;
   fp16_sat_m(result);
   return (fp16_t)result;
}



int fp16_lround(fp16_t x, uint8_t xfrac)
{
   int32_t result = x;

   if (xfrac == 0 )
   {
      return x;
   }

   if( x < 0 )
   {
      result = -result;
      result += (1<<(xfrac-1));
      result>>=xfrac;
      result = -result;
   }
   else
   {
      result += (1<<(xfrac-1));
      result>>=xfrac;
   }


   fp16_sat_m(result);
   return (fp16_t)result;

}




/*
 * sqrt_fixed16:
 *  - x:        16-Bit Fixed-Point-Wert im Qn-Format
 *  - frac_bits: Anzahl Nachkommabits n (z. B. 15 für Q1.15, 8 für Q8.8)
 *  -> Rückgabe: sqrt(x) im selben Qn-Format
 *
 * Implementiert die ganzzahlige "restoring" Wurzel direkt (ohne Helper).
 * Nutzt 32-Bit-Zwischenwerte und ist deterministisch (~16 Iterationen).
 */
fp16_t fp16_sqrt(fp16_t  x, uint8_t frac_bits)
{
    if (frac_bits > 15) frac_bits = 15;     // Safety für 32-Bit-Zwischenwerte

    // Wir brauchen y_fixed = sqrt(x * 2^n)
    uint32_t a = (uint32_t)x << frac_bits;

    // isqrt32(a): restoring square root, ohne Mul/Div
    uint32_t r = 0;
    uint32_t bit = 1u << 30;                // höchstes gerades Bit (<= 2^30)
    while (bit > a) bit >>= 2;

    while (bit != 0) {
        if (a >= r + bit) {
            a -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }

    if (r > 0xFFFFu) r = 0xFFFFu;           // Sättigung in 16 Bit
    return (uint16_t)r;
}






fp16_t fp16_cbrt(fp16_t a, uint8_t afrac)
{

    /*
        Newton's method
        x[n+1] = (a/x[n]^2 + 2*x[n])/3
    */


    int32_t x = 1<<afrac;


    if ( a == 0 )
    {
        return 0;
    }

    for(int tmp = 0; tmp < FP16_CBRT_ITERATIONS; tmp++)
    {
        int32_t xx = x*x;
        fpxx_arshift_m(xx,afrac);
        x = (x<<1)+(a<<afrac)/xx;
        x = x/3;
    }

    fp16_sat_m(x);
    return (fp16_t)x;
}

// CORDIC-Skalierungsfaktor K ≈ 0.607252935 in Q1.15 (unverändert)
#define CORDIC_K_Q15   ((int16_t)0x4DBA)  // round(0.607252935 * 2^15) = 19898

// Nützliche Konstanten im "π-normalisierten" Q1.15
#define FP16_Q15_NORM_ONE_PI            FP16_Q15_ALMOST_ONE      // +π  (≈ +1.0)
#define FP16_Q15_NORM_MINUS_PI          FP16_Q15_MINUS_ONE       // -π  (exakt -1.0)
#define FP16_Q15_NORM_HALF_PI           FP16_Q15_ONE_HALF        // +π/2 (= +0.5)
#define FP16_Q15_NORM_MINUS_HALF_PI     (-FP16_Q15_ONE_HALF)     // -π/2 (= -0.5)

// atan(2^-i) Tabelle in Q1.15 *relativ zu π*, also: round(atan(2^-i)/π * 2^15)
// (i = 0..13); danach wird's 0 in dieser Q-Skalierung.
static const int16_t atan_table_q15_pi[14] = {
    0x2000, // atan(2^-0)/π
    0x12E4, // atan(2^-1)/π
    0x09FB, // atan(2^-2)/π
    0x0511, // atan(2^-3)/π
    0x028B, // atan(2^-4)/π
    0x0146, // atan(2^-5)/π
    0x00A3, // atan(2^-6)/π
    0x0051, // atan(2^-7)/π
    0x0029, // atan(2^-8)/π
    0x0014, // atan(2^-9)/π
    0x000A, // atan(2^-10)/π
    0x0005, // atan(2^-11)/π
    0x0003, // atan(2^-12)/π
    0x0001  // atan(2^-13)/π
};






// Eingabe:  angle_q15 in Q1.15, normiert auf π: [-1.000*π, +(1.000-LSB)*π]  -> z ∈ [-1.0, +0.999969...]
// Ausgabe: *sin_q15, *cos_q15 in Q1.15.
void cordic_sin_cos_q15_pi(fp16_t angle_q15, fp16_t* sin_q15, fp16_t* cos_q15)
{
    // Hinweis: Die Eingabe ist bereits auf [-π, π) begrenzt (π-normalisierte Q1.15).
    // Optionales "Sicherheits-Clamping" könnte man hinzufügen, ist hier nicht nötig.



     // 1) In den Bereich [-π/2, π/2] spiegeln + Vorzeichen für Quadranten merken
    fp32_t z = angle_q15;   // z ist jetzt "π-normalisierter" Winkel in Q1.15
    int16_t sign_s = 1, sign_c = 1;

    if (z > FP16_Q15_NORM_HALF_PI)
    {
        // θ in (π/2, π): z' = π - θ  -> sin positiv, cos negativ
        // In normierter Darstellung: z' = 1.0 - z  (mit 1.0 ≈ 0x7FFF)
        z = FP16_Q15_NORM_ONE_PI - z;
        sign_c = -1;
    }
    else if (z < FP16_Q15_NORM_MINUS_HALF_PI)
    {
        // z' = -π - z  (in π-normalisiertem Q1.15: -1.0 - z)
        z = FP16_Q15_NORM_MINUS_PI - z;
        sign_c = -1;   // bleibt -1
    }
    else
    {

    }

    // 2) CORDIC-Startvektor in Q1.15 (K vorab multipliziert)
    int16_t x = CORDIC_K_Q15;
    int16_t y = 0;

    // 3) Iterationen (14 Schritte für diese Winkelauflösung genügen)
    for (int i = 0; i < 14; ++i)
    {
        int16_t x_shift = (int16_t)(x >> i);
        int16_t y_shift = (int16_t)(y >> i);
        int16_t a = atan_table_q15_pi[i];  // Winkelinkrement in "π-normalisiertem" Q1.15

        if (z >= 0)
        {
            fp32_t xn = (int32_t)x - y_shift;
            fp32_t yn = (int32_t)y + x_shift;
            fp16_sat_m(xn);
            fp16_sat_m(yn);
            x = (int16_t)xn;
            y = (int16_t)yn;
            z = (z - a);
        }
        else
        {
            fp32_t xn = (int32_t)x + y_shift;
            fp32_t yn = (int32_t)y - x_shift;
            fp16_sat_m(xn);
            fp16_sat_m(yn);
            x = (int16_t)xn;
            y = (int16_t)yn;
            z = (z + a);
        }
    }

    // 4) Quadranten-Vorzeichen anwenden
    *cos_q15 = (sign_c > 0) ? x : (int16_t)(-x);
    *sin_q15 = (sign_s > 0) ? y : (int16_t)(-y);
}

fp16_t fp16_sin(fp16_t rad)
{
    fp16_t sin_q15, cos_q15;
    cordic_sin_cos_q15_pi(rad, &sin_q15, &cos_q15);
    return sin_q15;
}

fp16_t fp16_cos(fp16_t rad)
{
    fp16_t sin_q15,cos_q15;
    cordic_sin_cos_q15_pi(rad , &sin_q15, &cos_q15);
    return cos_q15;
}


fp16_t fp16_tan(fp16_t fp, uint8_t frac)
{
    fp32_t x;
    fp16_t sin_q15, cos_q15;

    switch(fp)
    {
        case FP16_Q15_NORM_MINUS_HALF_PI:
            errno = EDOM;
            return INT16_MAX;
        case FP16_Q15_NORM_HALF_PI:
            errno = EDOM;
            return INT16_MIN;
    }

    cordic_sin_cos_q15_pi(fp, &sin_q15, &cos_q15);

    x = (sin_q15<<FP16_Q15)/cos_q15;

    x = fp32_arshift(x,FP16_Q15-frac);
    fp16_sat_m(x);
    return (fp16_t)x;
}




fp16_t fp16_atan2(fp16_t y_in, fp16_t x_in)
{
    // Sonderfälle wie bei double atan2
    if (y_in == 0) {
        if (x_in > 0) return (fp16_t)0;
        if (x_in < 0) return (fp16_t)FP16_Q15_NORM_ONE_PI;      // +π
        return (fp16_t)0;                             // atan2(0,0) -> 0 (Konvention)
    }
    if (x_in == 0) {
        fp16_t half_pi = (fp16_t)(FP16_Q15_NORM_ONE_PI >> 1);   // ±π/2
        return (y_in > 0) ? half_pi : (fp16_t)(-half_pi);
    }

    // Merke Original-Vorzeichen für Quadrantenkorrektur
    const int y_orig_nonneg = (y_in >= 0);
    const int x_orig_neg    = (x_in < 0);

    // In rechte Halbebene spiegeln, damit Vectoring stabil konvergiert
    // (atan(y/x) wird dadurch nicht geändert; nur Quadranten-Offset verschiebt sich)
    int32_t Xi = x_in;
    int32_t Yi = y_in;
    if (x_orig_neg) { Xi = -Xi; Yi = -Yi; }  // jetzt Xi >= 0

    int32_t Z = 0;

    // CORDIC-Vectoring: z += sign(y) * atan(2^-i)
    for (int i = 0; i < 14; ++i) {
        int32_t x_shift = (Xi >> i);
        int32_t y_shift = (Yi >> i);
        int32_t a = (int32_t)atan_table_q15_pi[i]; // Q1.15, in Einheiten von π

        if (Yi > 0) {
            // rotate clockwise (vectoring)
            int32_t Xn = Xi + y_shift;
            int32_t Yn = Yi - x_shift;
            Xi = Xn; Yi = Yn; Z += a;
        } else {
            // rotate counter-clockwise (vectoring)
            int32_t Xn = Xi - y_shift;
            int32_t Yn = Yi + x_shift;
            Xi = Xn; Yi = Yn; Z -= a;
        }
        if (Yi == 0) break; // frühzeitiger Stopp, wenn genau getroffen
    }

    // Quadrantenkorrektur anhand der ORIGINALEN Vorzeichen
    if (x_orig_neg) {
        Z += y_orig_nonneg ? (int32_t)FP16_Q15_NORM_ONE_PI      // +π
                           : (int32_t)(-FP16_Q15_NORM_ONE_PI);  // −π
    }

    // End-Sättigung und Rückgabe
    fp16_sat_m(Z);
    return (fp16_t)(int16_t)Z;
}


fp16_t fp16_atan(fp16_t x, uint8_t frac)
{
    fp16_t y = INT16_MAX;
    fp32_t X = x;



    fpxx_ashift_m(X, frac-FP16_Q15);


    while(X > INT16_MAX || X < INT16_MIN)
    {
       X = fp32_arshift(X,1);
       y = fp32_arshift(y,1);

    }

    fp16_sat_m(X);

    return fp16_atan2((fp16_t)X, y);
}



fp16_t fp16_asin(fp16_t x)
{

    int32_t xi = x;

    // t = 1 − x^2  in Q1.15
    // Achtung: Für die "1" verwenden wir intermediär 1<<15 (=32768), damit bei |x|=1 exakt 0 herauskommt.
    uint32_t prod      = (uint32_t)(xi * xi);   // bis 1,073,741,824, passt in 32 Bit
    uint32_t prod_q15  = prod >> 15;            // zurück in Q1.15
    int32_t  t_ext     = (int32_t)(1u << 15) - (int32_t)prod_q15; // kann negativ werden
    if (t_ext < 0)      t_ext = 0;
    if (t_ext > 0x7FFF) t_ext = 0x7FFF;         // in darstellbaren Q1.15-Bereich klemmen
    fp16_t   t_q15      = (fp16_t)t_ext;

    // c = sqrt(1 - x^2) in Q1.15
    fp16_t c = fp16_sqrt(t_q15, 15);

    // asin(x) = atan2(x, c);  Ergebnis bereits π-normalisiert in Q1.15
    return fp16_atan2((fp16_t)xi, c);
}

fp16_t fp16_acos(fp16_t x)
{

    int32_t xi = (int32_t)x;

    uint32_t prod      = (uint32_t)(xi * xi);   // bis 1,073,741,824, passt in 32 Bit
    uint32_t prod_q15  = prod >> 15;            // zurück in Q1.15
    int32_t  t_ext     = (int32_t)(1u << 15) - (int32_t)prod_q15; // kann negativ werden
    if (t_ext < 0)      t_ext = 0;
    if (t_ext > 0x7FFF) t_ext = 0x7FFF;
    fp16_t   t_q15      = (fp16_t)t_ext;

    // c = sqrt(1 - x^2) in Q1.15
    fp16_t c = fp16_sqrt(t_q15, 15);

    // acos(x) = atan2(c, x);  Ergebnis bereits π-normalisiert in Q1.15
    // Wertebereich: [0 .. 1) (entspricht [0 .. π))
    return fp16_atan2(c, (fp16_t)xi);
}


fp16_t fp16_copysign(fp16_t x, fp16_t y)
{
   int32_t result = abs((int32_t)x);

   if(fp16_signbit(y))
   {
      result = -result;
   }

   fp16_sat_m(result);

   return (fp16_t)result;
}




/*!
    \brief      Compute absolute value
    \details    Returns the absolute value of x: |x|.


    \param      x
    \returns     absolute value of x

*/
fp16_t fp16_fabs (fp16_t x)
{
   int32_t result = (x < 0) ? (-(int32_t)x) : ((int32_t)x);
   fp16_sat_m(result);
   return result;
}


/*!
    \brief      Compute absolute value (operates on int values)
    \details    Returns the absolute value of x: |int(x)|.


    \param     x
    \returns   absolute value of int(x)

*/
fp16_t fp16_abs (fp16_t x, uint8_t frac)
{
   int32_t result = (x < 0) ? (-(int32_t)x) : ((int32_t)x);
   result&=~((1<<frac)-1);
   fp16_sat_m(result);
   return result;
}



fp16_t fp16_fma (fp16_t x, uint8_t xfrac, fp16_t y, uint8_t yfrac, fp16_t z, uint8_t zfrac)
{
   int32_t result;
   int8_t relshift = xfrac+yfrac-zfrac;

   result = (int32_t)x*(int32_t)y;
   fpxx_ashift_m(result,relshift);
   result += z;
   fp16_sat_m(result);
   return (fp16_t)result;
}

