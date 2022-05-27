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

/*!
    \brief      Right shifts fixed point number with rounding LSB
    \details    Right shifts fixed point number with rounding LSB
    \param      fp       Fixed point number to right shift
    \param      shift     Number of bits to right shift

    \returns Right shifted fixed point number with rounded LSB
*/

fp16_t fp16_rshift(fp16_t fp, uint8_t shift)
{
   int32_t result = fp;
   fp16_rshift_m(result,shift);
   fp16_sat_m(result);
   return(fp16_t)result;
}

/*!
    \brief      Left shifts fixed point number
    \details    Left shifts fixed point number
    \param      fp       Fixed point number to left shift
    \param      shift     Number of bits to left shift

    \returns Left shifted fixed point number
*/
fp16_t fp16_lshift(fp16_t fp, uint8_t shift)
{
   int32_t result = (int32_t)fp;
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
   if ( fracold > fracnew )
   {
      fp = fp16_rshift(fp,fracold-fracnew);
   }
   else if ( fracold < fracnew )
   {
      fp = fp16_lshift(fp,fracnew-fracold);
   }

   return fp;
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
    int32_t result;
    result = (int32_t)mult1*(int32_t)mult2;
    fp16_shift_m(result,frac2);
    fp16_sat_m(result);
    return (fp16_t)result;
}


/*!
    \brief      Divides two fixed point numbers
    \details    Divides two fixed point numbers

                result = a/b


    \param      divident     divident
    \param      mult2     multiplicant


    \returns a/b with fractional bits of b
*/
fp16_t fp16_div(fp16_t divident, uint8_t frac1, fp16_t divisor, uint8_t frac2)
{
  int32_t result = (divident<<frac2)/divisor;
  fp16_sat_m(result);
  return (fp16_t)result;
}





fp16_t fp16_ceil(fp16_t x, uint8_t xfrac)
{

   int32_t result = (int32_t)x&~((1<<xfrac)-1);

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
   fp16_rshift_m(result,yfrac);

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





fp16_t fp16_sqrt(fp16_t s, uint8_t sfrac)
{
	/* https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method */
    // x[n+1] = (x[n] + s/x[n])/2


	if ( s == 0 )
	{
		return 0;
	}

    if ( fp16_signbit(s) )
    {
        errno = EDOM;
        return 0;
    }


	int32_t x = 1<<sfrac; // start with 1.0

	for (int i = 0; i < FP16_SQRT_ITERATIONS; i++)
	{
		x = (x+((s<<sfrac)/x));     //  x[n] + s/x[n]
		fp16_rshift_m(x,1);         //  /2
	}

	fp16_sat_m(x);
	return x;
}



fp16_t fp16_sin(fp16_t fp)
{

    int64_t nom;
    int32_t x = fp16_signbit(fp)?(-fp):(fp);

    /* Bhaskara I approximation */
    /* https://datagenetics.com/blog/july12019/index.html */
    x = (x*FP16_Q13_M_PI)>>FP16_Q15; //Q13
    nom = ((FP16_Q13_M_PI-x)*x)>>FP16_Q11; //Q13
    x = 5*((FP16_Q13_M_PI*FP16_Q13_M_PI)>>FP16_Q13)-nom; //Q13
    x = (nom<<(FP16_Q14+2))/x; //Q14

    return (fp16_t)fp16_signbit(fp)?(-x):(x);
}

fp16_t fp16_cos(fp16_t fp)
{
    int32_t x = fp+FP16_Q15_ONE_HALF;

    if( x > INT16_MAX )
    {
        x -= UINT16_MAX;
    }

    return fp16_sin((fp16_t)x);
}



fp16_t fp16_tan(fp16_t fp, uint8_t frac)
{
    int32_t result;

    /* avoid division by zero */
    if(fp ==  FP16_Q15_ONE_HALF )
    {
        errno = EDOM;
        return INT16_MAX;
    }

    /* avoid division by zero */
    if(fp ==  -FP16_Q15_ONE_HALF )
    {
        errno = EDOM;
        return INT16_MIN;
    }

    result = (fp16_sin(fp)<<FP16_Q15)/fp16_cos(fp);
    fp16_shift_m(result,FP16_Q15-frac);
    fp16_sat_m(result);
    return (fp16_t)result;
}


/* pi/2-sqrt(1-x)*(a+b*x+c*x*x+d*x*x*x) */
fp16_t fp16_asin(fp16_t fp)
{
    int32_t result;
    bool sign = fp16_signbit(fp);

    if(fp > FP16_Q14_ONE)
    {
        errno = EDOM;
        fp = FP16_Q14_ONE;
    }

    if(fp < FP16_Q14_MINUS_ONE)
    {
        errno = EDOM;
        fp = FP16_Q14_MINUS_ONE;
    }

    if (sign)
    {
        fp = -fp;
    }

    result = FP16_Q14_ASIN_C+((FP16_Q14_ASIN_D*fp)>>FP16_Q14);
    result = FP16_Q14_ASIN_B+((result*fp)>>FP16_Q14);
    result = FP16_Q14_ASIN_A+((result*fp)>>FP16_Q14);
    result = FP16_Q14_M_PI_2-((result*fp16_sqrt(FP16_Q14_ONE-fp,FP16_Q14))>>FP16_Q14);

    if (sign)
    {
        result = -result;
    }

    return result;
}


fp16_t fp16_atan(fp16_t fp, uint8_t frac)
{
    int64_t xx = fp<<(FP16_Q14-frac);
    xx =(xx*xx)>>FP16_Q14;

    int32_t result = FP16_Q14_ONE+xx;
    result = (xx<<FP16_Q14)/result;
    result = fp16_sqrt(result,FP16_Q14);

    result = fp16_asin(result);

    return (fp16_signbit(fp)?(-result):result);
}

int32_t fp32_exp(int32_t fp, uint8_t frac)
{
    const fp16_t one = 1<<frac;

    int64_t result = one;
    bool neg = false;

    if(fp16_signbit(fp))
    {
        neg = true;
        fp = fp16_copysign(fp,1);
    }

    int32_t fpshifted = fp<<frac;

    for (uint8_t k = FP16_EXP_ITERATIONS; k > 0; k--)
    {
        result *= fpshifted/(k<<frac);
        fp16_rshift_m(result,frac);
        result += one;

        //fp16_sat_m(result);
        if (result > INT32_MAX) { result = INT32_MAX; }
        if (result < INT32_MIN) { result = INT32_MIN; }

    }

    if(neg)
    {
        return (one<<frac)/result; /* 1/e^abs(x) */
    }



    return result; /* e^abs(x)  */
}

int32_t fp32_log(fp16_t x, uint8_t frac)
{

    /*
    Natural logarithm of x.
    If x is negative, it causes a domain error.
    If x is zero, it may cause a pole error (depending on the library implementation).
     */
    //To compute the natural logarithm with many digits of precision, the Taylor series approach is not efficient since the convergence is slow. Especially if x is near 1, a good alternative is to use Halley's method or Newton's method to invert the exponential function, because the series of the exponential function converges more quickly. For finding the value of y to give exp(y) − x = 0 using Halley's method, or equivalently to give exp(y/2) − x exp(−y/2) = 0 using Newton's method, the iteration simplifies to
    // {\displaystyle y_{n+1}=y_{n}+2\cdot {\frac {x-\exp(y_{n})}{x+\exp(y_{n})}}}

    //Occording to Wikipedia, there is the Halley-Newton approximation method
    // Using Newton's method, the iteration simplifies to (implementation)
    // which has cubic convergence to ln(x).


    int32_t y = 0;

    /* If x is negative, it causes a domain error. */
    if ( x < 0 )
    {
       errno = EDOM;
       return INT32_MIN;
    }

    /* If x is zero, it may cause a pole error  */
    if ( x == 0 )
    {
       errno = ERANGE;
       return INT32_MIN;
    }

    for (uint8_t n = 0; n < FP16_LOG_ITERATIONS; n++)
    {
        int32_t exp_y = fp32_exp(y,frac);
        y += ((x-exp_y)<<(frac+1))/(x+exp_y);

    }


    return y;
}


fp16_t fp16_sinh(fp16_t fp, uint8_t frac)
{
    int32_t result = (fp32_exp(fp,frac) - fp32_exp(-fp,frac))>>1;
    fp16_sat_m(result);
    return result;
}

fp16_t fp16_cosh(fp16_t fp, uint8_t frac)
{
    int32_t result = (fp32_exp(fp,frac) + fp32_exp(-fp,frac))>>1;
    fp16_sat_m(result);
    return result;
}

fp16_t fp16_tanh(fp16_t fp, uint8_t frac)
{
    int32_t result = ((fp16_sinh(fp,frac))<<frac)/fp16_cosh(fp,frac);
    fp16_sat_m(result);
    return (fp16_t)fp16_fp2fp(result,frac,FP16_Q14);
}





fp16_t fp16_exp(fp16_t fp, uint8_t frac)
{
   int32_t result = fp32_exp(fp, frac);
   fp16_sat_m(result);
   return result;
}



fp16_t fp16_log(fp16_t fp, uint8_t frac)
{
    int32_t result = fp32_log(fp, frac);
    fp16_sat_m(result);
    return result;
}


fp16_t fp16_log10(fp16_t x, uint8_t frac)
{
  return (fp16_log(x,frac)<<frac)/fp16_log(10<<frac,frac);
}


fp16_t fp16_log2(fp16_t x, uint8_t frac)
{
  return (fp16_log(x,frac)<<frac)/fp16_log(2<<frac,frac);
}





fp16_t fp16_pow(fp16_t x,  fp16_t n, uint8_t frac)
{

    int32_t result = x;

    /* If both base and exponent are zero, it may also cause a domain error on certain implementations. */
    /* If base is zero and exponent is negative, it may cause a domain error or a pole error (or none, depending on the library implementation). */
    if ( x == 0 )
    {
        if ( n <= 0 )
        {
            errno = EDOM;
            return INT16_MAX;
        }

        return x;
    }

    // y = exp(n*ln(x))


    if ( x < 0 )
    {
        /* If the base is finite negative and the exponent is finite but not an integer value, it causes a domain error. */

        if( (n & ((1<<frac)-1)) != 0 )
        {
            errno = EDOM;
            return 0;
        }


        result = fp32_log(-x,frac);
        result = (result*n)>>frac;
        result = fp32_exp(result,frac);


        if ( fp16_fp2int(n,frac)&1  )
        {
            result = - result;
        }



    }
    else /* ( x > 0 ) */
    {
        result = fp32_log(x,frac);
        result = (result*n)>>frac;
        result = fp32_exp(result,frac);
    }


    fp16_sat_m(result);
    return (fp16_t)result;

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


/*!
    \brief      Multiply-add
    \details    Returns x*y+z.

    \param     x
    \param     xfrac
    \param     y
    \param     yfrac
    \param     z
    \param     zfrac

    \returns   x*y+z with fractional bits of z

*/
fp16_t fp16_fma (fp16_t x, uint8_t xfrac, fp16_t y, uint8_t yfrac, fp16_t z, uint8_t zfrac)
{
   int32_t result;
   result = (int32_t)x*(int32_t)y;
   fp16_shift_m(result,xfrac+yfrac-zfrac);
   result += z;
   fp16_sat_m(result);
   return (fp16_t)result;
}

