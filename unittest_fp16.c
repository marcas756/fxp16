/*! \copyright
    Copyright (c) 2017, marcas756@gmail.com
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
	\file	    unittest_fp16.c

    \brief

    \details
*/



#include "unittest.h"
#include "fp16.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fenv.h>
#include <errno.h>

#define REPETITIONS 1000

 typedef struct {
    uint8_t fract;
    float prec;
    float min;
    float max;
 }fp16_properties_t;

 const fp16_properties_t fp16_props [16] = {
       {FP16_Q0,FP16_Q0_PREC,FP16_Q0_MIN,FP16_Q0_MAX},
       {FP16_Q1,FP16_Q1_PREC,FP16_Q1_MIN,FP16_Q1_MAX},
       {FP16_Q2,FP16_Q2_PREC,FP16_Q2_MIN,FP16_Q2_MAX},
       {FP16_Q3,FP16_Q3_PREC,FP16_Q3_MIN,FP16_Q3_MAX},
       {FP16_Q4,FP16_Q4_PREC,FP16_Q4_MIN,FP16_Q4_MAX},
       {FP16_Q5,FP16_Q5_PREC,FP16_Q5_MIN,FP16_Q5_MAX},
       {FP16_Q6,FP16_Q6_PREC,FP16_Q6_MIN,FP16_Q6_MAX},
       {FP16_Q7,FP16_Q7_PREC,FP16_Q7_MIN,FP16_Q7_MAX},
       {FP16_Q8,FP16_Q8_PREC,FP16_Q8_MIN,FP16_Q8_MAX},
       {FP16_Q9,FP16_Q9_PREC,FP16_Q9_MIN,FP16_Q9_MAX},
       {FP16_Q10,FP16_Q10_PREC,FP16_Q10_MIN,FP16_Q10_MAX},
       {FP16_Q11,FP16_Q11_PREC,FP16_Q11_MIN,FP16_Q11_MAX},
       {FP16_Q12,FP16_Q12_PREC,FP16_Q12_MIN,FP16_Q12_MAX},
       {FP16_Q13,FP16_Q13_PREC,FP16_Q13_MIN,FP16_Q13_MAX},
       {FP16_Q14,FP16_Q14_PREC,FP16_Q14_MIN,FP16_Q14_MAX},
       {FP16_Q15,FP16_Q15_PREC,FP16_Q15_MIN,FP16_Q15_MAX}
 };


/*
  fixed-point multiplication & scaling.
  Substitute inline assembly for hardware-specific
  optimization suited to a particluar DSP processor.
  Scaling ensures that result remains 16-bit.
*/
fp16_t fp16_mult_q15(fp16_t a, fp16_t b)
{
    /* shift right one less bit (i.e. 15-1) */
    int32_t c = ((int32_t)a * (int32_t)b) >> 14;
    /* last bit shifted out = rounding-bit */
    int32_t result = c & 0x01;
    /* last shift + rounding bit */
    result = (c >> 1) + result;

    /* -1.0*-1.0 = +1.0 which exceeds fp16 range! thus saturation is required */
    fp16_sat_m(result);

    return (fp16_t) result;
}





float randf(float min, float max) {
    return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

float f_saturate(float x, float min, float max)
{

   if (x < min)
   {
      return min;
   }

   if (x > max)
   {
      return max;
   }

   return x;
}


uint8_t randfp (fp16_t *fp)
{
   *fp  = (fp16_t)(rand()%UINT16_MAX);
   return rand()&0xF;
}




UNITTEST_TESTCASE(fp16_check_shiftops)
{
   for (int pos = 0; pos < INT16_MAX; pos++)
   {
      for(int rshift = 0; rshift < 15; rshift++)
      {
         int neg = -pos;

         int negshifted = -((-neg)>>rshift);
         int posshifted = pos >> rshift;

         UNITTEST_ASSERT("Unexpected result", -negshifted == posshifted);


      }
   }


   for (int pos = 0; pos < INT16_MAX; pos++)
   {
      for(int lshift = 0; lshift < 15; lshift++)
      {
         int neg = -pos;

         int negshifted = neg << lshift;
         int posshifted = pos << lshift;

         UNITTEST_ASSERT("Unexpected result", -negshifted == posshifted);

      }
   }
}







UNITTEST_TESTCASE(fp16_add)
{
   /*
       c (+/-delta_c) = a (+/-delta_a) + b (+/-delta_b)
       c-a-b (+/-delta_c) = (+/-delta_a) + (+/-delta_b)
       (+/-delta_c) = (+/-delta_a) + (+/-delta_b)
   */
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (int rep=0; rep < REPETITIONS; rep++)
      {
         float fa = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         fp16_t fpa = fp16_flt2fp(fa,fp16_props[tmp].fract);
         fa = fp16_fp2flt(fpa,fp16_props[tmp].fract); /* delta_a = 0 */

         float fb = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         fp16_t fpb = fp16_flt2fp(fb,fp16_props[tmp].fract);
         fb = fp16_fp2flt(fpb,fp16_props[tmp].fract); /* delta_b = 0 */

         /* delta_b = 0; delta_a = 0 */
         /* (+/-delta_c) = (+/-delta_a) + (+/-delta_b) => delta_c = 0 */

         //UNITTEST_PRINTF("%f + %f = %f\n",fa,fb,fp16_fptof(fp16_add(fpa,fpb),fp16_properties[tmp].fract));
         UNITTEST_ASSERT("Addition test failed",fp16_flt2fp(fa+fb,fp16_props[tmp].fract) == fp16_add(fpa,fpb));
      }
   }
}

UNITTEST_TESTCASE(fp16_sub)
{

   /*
       c (+/-delta_c) = a (+/-delta_a) - b (+/-delta_b)
       c-a+b (+/-delta_c) = (+/-delta_a) - (+/-delta_b)
       (+/-delta_c) = (+/-delta_a) - (+/-delta_b)
   */

   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (int rep=0; rep < REPETITIONS; rep++)
      {
         float fa = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         fp16_t fpa = fp16_flt2fp(fa,fp16_props[tmp].fract);
         fa = fp16_fp2flt(fpa,fp16_props[tmp].fract); /* delta_a = 0 */

         float fb = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         fp16_t fpb = fp16_flt2fp(fb,fp16_props[tmp].fract);
         fb = fp16_fp2flt(fpb,fp16_props[tmp].fract); /* delta_b = 0 */

         /* delta_b = 0; delta_a = 0 */
         /* (+/-delta_c) = (+/-delta_a) - (+/-delta_b) => delta_c = 0 */

        //UNITTEST_PRINTF("%f - %f = %f\n",fa,fb,fp16_fptof(fp16_sub(fpa,fpb),fp16_properties[tmp].fract));
         UNITTEST_ASSERT("Subtraction test failed",fp16_flt2fp(fa-fb,fp16_props[tmp].fract) == fp16_sub(fpa,fpb));
      }
   }
}

UNITTEST_TESTCASE(fp16_mult_q15)
{
   for (int rep=0; rep < REPETITIONS ; rep++)
   {
      float max = sqrt(FP16_Q15_MAX);
      float min = -max;

      float fa = randf(min,max);
      float fb = randf(min,max);

      fp16_t fpa = fp16_flt2fp(fa,FP16_Q15);
      fp16_t fpb = fp16_flt2fp(fb,FP16_Q15);

      float fpc_1 = fp16_fp2flt(fp16_mult(fpa,FP16_Q15,fpb,FP16_Q15),FP16_Q15);
      float fpc_2 = fp16_fp2flt(fp16_mult_q15(fpa,fpb),FP16_Q15);

      // UNITTEST_PRINTF("%0.19f * %0.19f :  %0.19f / %0.19f / %0.19f\n",fa,fb,fa*fb,fpc_1,fpc_2);
      UNITTEST_ASSERT("fp16_mult differs from fp16_mult_q15, expected same result for both functions", fpc_1 == fpc_2);
   }
}




UNITTEST_TESTCASE(fp16_err_mult)
{

   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      float errmin = FLT_MAX;
      float errmax = FLT_MIN;


      for (int rep=0; rep < REPETITIONS ; rep++)
      {


         float max = sqrt(fp16_props[tmp].max);
         float min = -max;

         float fa = randf(min,max);
         float fb = randf(min,max);

         fp16_t fpa = fp16_flt2fp(fa,fp16_props[tmp].fract);
         fp16_t fpb = fp16_flt2fp(fb,fp16_props[tmp].fract);



         float fc = fa*fb;
         float fpc = fp16_fp2flt(fp16_mult(fpa,fp16_props[tmp].fract,fpb,fp16_props[tmp].fract),fp16_props[tmp].fract);


         UNITTEST_PRINTF("%0.19f\t\t%0.19f\t\t%0.19f\t\t%0.19f\t\t%0.19f\t\t%0.19f\n",fa,fb,fc,fp16_fp2flt(fpa,fp16_props[tmp].fract),fp16_fp2flt(fpb,fp16_props[tmp].fract),fpc );




         float z = fc-fpc;

         if(z > errmax)
         {
            errmax = z;
         }

         if(z < errmin)
         {
            errmin= z;
         }
      }

      UNITTEST_PRINTF("%0.19f\t\t%0.19f\n",errmin,errmax);
   }
}


bool fp16_fptofp_helper(float f, int old, int new)
{
   fp16_t fp1,fp2,fpr;

   fp1 = fp16_flt2fp(f,fp16_props[old].fract);
   fp2 = fp16_flt2fp(f,fp16_props[new].fract);
   fpr = fp16_fp2fp(fp1,fp16_props[old].fract,fp16_props[new].fract);


   UNITTEST_PRINTF("%d;%d;",old,new);
   UNITTEST_PRINTF("%0.15f;",f);
   UNITTEST_PRINTF("%0.15f;",fp16_fp2flt(fp1,fp16_props[old].fract));
   UNITTEST_PRINTF("%0.15f;",fp16_fp2flt(fp2,fp16_props[new].fract));
   UNITTEST_PRINTF("%0.15f\n",fp16_fp2flt(fpr,fp16_props[new].fract));

   return (fpr == fp2);
}


UNITTEST_TESTCASE(fp16_fp2fp)
{

   UNITTEST_PRINTF("\"OldFract\";");
   UNITTEST_PRINTF("\"NewFract\";");
   UNITTEST_PRINTF("\"FloatIn\";");
   UNITTEST_PRINTF("\"Float2FixedOldFract\";");
   UNITTEST_PRINTF("\"Float2FixedNewFract\";");
   UNITTEST_PRINTF("\"FixedOldFract2FixedNewFract\"\n");
/*
   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(-123.5, FP16_Q1, FP16_Q0));
   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(123.5, FP16_Q1, FP16_Q0));


   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(-123.25, FP16_Q1, FP16_Q0));
   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(123.25, FP16_Q1, FP16_Q0));

   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(-123.75, FP16_Q1, FP16_Q0));
   UNITTEST_ASSERT("Unexpected result",fp16_fptofp_helper(123.75, FP16_Q1, FP16_Q0));
*/

   fp16_t fpold = INT16_MAX;





   for (int old = 0; old < sizeof(fp16_props)/sizeof(*fp16_props); old++)
   {
      for (int new = 0; new < sizeof(fp16_props)/sizeof(*fp16_props); new++)
      {

         float f = fp16_fp2flt(fp16_fp2fp(fpold,old,new),new);

         UNITTEST_PRINTF("%d;%d;",old,new);
         UNITTEST_PRINTF("%0.15f;",fp16_fp2flt(fpold,old));
         UNITTEST_PRINTF("%0.15f\n",f);




      }
   }










}











UNITTEST_TESTCASE(fp16_rshift)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      fp16_t fp = fp16_flt2fp(fp16_props[tmp].prec,fp16_props[tmp].fract);

      fp16_t fp2 = fp16_rshift(fp,1);
      fp16_t fp3 = fp16_rshift(-fp,1);


      UNITTEST_PRINTF("%d; %0.15f, %0.15f %0.15f\n", fp16_props[tmp].fract , fp16_fp2flt(fp,fp16_props[tmp].fract), fp16_fp2flt(fp2,fp16_props[tmp].fract-1), fp16_fp2flt(fp3,fp16_props[tmp].fract-1));


   }
}


UNITTEST_TESTCASE(fp16_fma)
{

   fp16_t x,y,z,r;
   uint8_t xfrac,yfrac,zfrac;


   for (int tmp = 0; tmp < 10; tmp++)
   {
      uint8_t xfrac = randfp(&x);
      uint8_t yfrac = randfp(&y);
      uint8_t zfrac = randfp(&z);
      r = fp16_fma(x,xfrac,y,yfrac,z,zfrac);



   UNITTEST_PRINTF("(%0.15g)*(%0.15g)+(%0.15g)=%0.15g\n",fp16_fp2flt(x,xfrac),fp16_fp2flt(y,yfrac),fp16_fp2flt(z,zfrac),fp16_fp2flt(r,zfrac));
   }



}


UNITTEST_TESTCASE(fp16_ceil)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_ceil(fp,fp16_props[tmp].fract);

         UNITTEST_ASSERT("Unexpected Result",
               f_saturate(ceil(flt), fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}

UNITTEST_TESTCASE(fp16_floor)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_floor(fp,fp16_props[tmp].fract);

         UNITTEST_ASSERT("Unexpected Result",
               f_saturate(floor(flt), fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}

UNITTEST_TESTCASE(fp16_trunc)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_trunc(fp,fp16_props[tmp].fract);

         UNITTEST_ASSERT("Unexpected Result",
               f_saturate(trunc(flt), fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}


UNITTEST_TESTCASE(fp16_round)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_round(fp,fp16_props[tmp].fract);

         //UNITTEST_PRINTF("%f;%f;%f\n",flt,f_saturate(rint(flt), fp16_props[tmp].min, fp16_props[tmp].max),fp16_fp2flt(fp,fp16_props[tmp].fract));


         UNITTEST_ASSERT("Unexpected Result",
               f_saturate(round(flt), fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}


UNITTEST_TESTCASE(fp16_int2fp_fp2int)
{

   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         UNITTEST_ASSERT("Unexpected result",
               f_saturate((int)flt, fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2int(fp16_int2fp(flt,fp16_props[tmp].fract),fp16_props[tmp].fract));
      }
   }
}


UNITTEST_TESTCASE(fp16_ftofp_fptof)
{
   /* fp = f (+/- prec/2) */
   /* f = fp (+/- prec/2) */

   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {

      /* test all on-grid floats */
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         float x = flt;
         float y = fp16_fp2flt(fp16_flt2fp(x,fp16_props[tmp].fract),fp16_props[tmp].fract);
         UNITTEST_ASSERT("Unexpected result",x == y);
      }

      /* test some off-grid floats */
      for (int rep = 0; rep < REPETITIONS; rep++)
      {
         float x = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         float y = fp16_fp2flt(fp16_flt2fp(x,fp16_props[tmp].fract),fp16_props[tmp].fract);
         UNITTEST_ASSERT("Unexpected result",fabs(y-x)<=fp16_props[tmp].prec/2);
      }
   }
}

UNITTEST_TESTCASE(fp16_precision)
{

   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      float errmin = FLT_MAX;
      float errmax = FLT_MIN;


      for (int rep=0; rep < REPETITIONS ; rep++)
      {
         float x = randf(fp16_props[tmp].min,fp16_props[tmp].max);
         float y = fp16_fp2flt(fp16_flt2fp(x,fp16_props[tmp].fract),fp16_props[tmp].fract);

         float err = y-x;

         if(err > errmax)
         {
            errmax = err;
         }

         if(err < errmin)
         {
            errmin= err;
         }
      }

      UNITTEST_ASSERT("Unexpected result",errmax-errmin <= fp16_props[tmp].prec );
   }
}

UNITTEST_TESTCASE(fp16_fabs)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_fabs(fp);

         UNITTEST_ASSERT("Unexpected result",
               f_saturate(fabs(flt),fp16_props[tmp].min,fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}

UNITTEST_TESTCASE(fp16_abs)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_abs(fp,fp16_props[tmp].fract);

         UNITTEST_ASSERT("Unexpected result",
               f_saturate(abs(flt),fp16_props[tmp].min,fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}

UNITTEST_TESTCASE(fp16_fmod)
{

   for (int rep = 0; rep < REPETITIONS; rep++)
   {
      fp16_t x;
      uint8_t xfrac = randfp (&x);
      fp16_t y;
      uint8_t yfrac = randfp (&y);

      if(y == 0) continue;

      float rflt = fmod(fp16_fp2flt(x,xfrac),fp16_fp2flt(y,yfrac));
      rflt = f_saturate(rflt,fp16_props[xfrac].min,fp16_props[xfrac].max);
      float rfp = fp16_fp2flt(fp16_fmod(x,xfrac,y,yfrac),xfrac);

      UNITTEST_ASSERT("Unexpected result",fabs(rflt-rfp) <= fp16_props[xfrac].prec/2 );

   }
}


UNITTEST_TESTCASE(fp16_lround)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         int result = fp16_lround(fp,fp16_props[tmp].fract);


         UNITTEST_ASSERT("Unexpected result",
               lround(flt) ==
               result);
      }
   }
}

UNITTEST_TESTCASE(fp16_fp2int)
{
   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         int result = fp16_fp2int(fp,fp16_props[tmp].fract);

         UNITTEST_ASSERT("Unexpected result",
               lround(flt) ==
               result);
      }
   }
}


UNITTEST_TESTCASE(fp16_int2fp)
{
      for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
      {
         for (float flt = INT16_MIN; flt > INT16_MAX ; flt+=1)
         {
            fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
            int result = fp16_int2fp(fp,fp16_props[tmp].fract);

            UNITTEST_ASSERT("Unexpected result",
                  f_saturate(flt,fp16_props[tmp].min,fp16_props[tmp].max) ==
                  result);
         }
      }

}


#define fp16_rint(fp,frac) fp16_round(fp,frac)

UNITTEST_TESTCASE(fp16_rint)
{
   /*
   To-nearest rounding direction mode
   This macro expands to a value of type int that identifies the to-nearest rounding direction mode for functions fegetround and fesetround.

   Rounding x to-nearest selects the possible value that is nearest to x, with halfway cases rounded away from zero.
   */

   switch (fegetround()) {
      case FE_DOWNWARD: printf ("downward\n"); break;
      case FE_TONEAREST: printf ("to-nearest\n"); break;
      case FE_TOWARDZERO: printf ("toward-zero\n"); break;
      case FE_UPWARD: printf ("upward\n"); break;
      default: printf ("unknown\n");
    }

   UNITTEST_ASSERT("Unsupported direction", fegetround() == FE_TONEAREST);


   for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
   {
      for (float flt = fp16_props[tmp].max; flt >= fp16_props[tmp].min  ; flt-=fp16_props[tmp].prec)
      {
         fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
         fp = fp16_rint(fp,fp16_props[tmp].fract);

         UNITTEST_PRINTF("%f;%f;%f\n",flt,f_saturate(rint(flt), fp16_props[tmp].min, fp16_props[tmp].max),fp16_fp2flt(fp,fp16_props[tmp].fract));

         UNITTEST_ASSERT("Unexpected Result",
               f_saturate(rint(flt), fp16_props[tmp].min, fp16_props[tmp].max) ==
               fp16_fp2flt(fp,fp16_props[tmp].fract));
      }
   }
}

float sqrt_maxerr_best_effort [16] = {
        0.644365,
        0.322183,
        0.147372,
        0.071044,
        0.034930,
        0.017324,
        0.008628,
        0.004305,
        0.002151,
        0.001075,
        0.000537,
        0.000269,
        0.000134,
        0.000067,
        0.000034,
        0.000017,
};

UNITTEST_TESTCASE(fp16_sqrt)
{
    int tmp = FP16_Q1;

    for(int tmp = 0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
    {
        float maxerr = 0;

        for (float flt = 0; flt <= fp16_props[tmp].max  ; flt+=fp16_props[tmp].prec)
        {
            fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
            fp = fp16_sqrt(fp,fp16_props[tmp].fract);

            //UNITTEST_PRINTF("%f;%f;%f\n", flt, sqrt(flt),fp16_fp2flt(fp,fp16_props[tmp].fract));

            float err = fabs(sqrt(flt)-fp16_fp2flt(fp,fp16_props[tmp].fract));

            UNITTEST_ASSERT("Unexpected result", fabs(sqrt(flt)-fp16_fp2flt(fp,fp16_props[tmp].fract)) <= sqrt_maxerr_best_effort[tmp]);

            if (err > maxerr)
            {
                maxerr = err;
            }
        }

        //printf("%f,\n",tmp,maxerr*1.1);
    }
}


float cbrt_maxerr_best_effort [16] = {
        1.099593,
        0.571826,
        0.274866,
        0.145397,
        0.071236,
        0.039832,
        0.019887,
        0.011322,
        0.005568,
        0.003513,
        0.001441,
        0.000892,
        0.000537,
        0.000239,
        0.000252,
        0.000140,
};

UNITTEST_TESTCASE(fp16_cbrt)
{


    for (int tmp = FP16_Q0; tmp < sizeof(fp16_props)/sizeof(*fp16_props); tmp++)
    {
        float maxerr = 0;

        for (float flt = 0; flt <= fp16_props[tmp].max  ; flt+=fp16_props[tmp].prec)
        {
            fp16_t fp = fp16_flt2fp(flt,fp16_props[tmp].fract);
            fp = fp16_cbrt(fp,fp16_props[tmp].fract);

            float err = fabs(cbrt(flt)-fp16_fp2flt(fp,fp16_props[tmp].fract));

            if (err > maxerr)
            {
                maxerr = err;
            }


             //UNITTEST_PRINTF("%f;%f;%f\n", flt, cbrt(flt),fp16_fp2flt(fp,fp16_props[tmp].fract));


             UNITTEST_ASSERT("Unexpected result", err <= cbrt_maxerr_best_effort[tmp] );

        }

        //printf("%f,\n",tmp,maxerr*1.1);
    }
}

float hypot_maxerr_best_effort [16] = {
        1.781250,
        0.892090,
        0.448975,
        0.220825,
        0.110962,
        0.055573,
        0.027557,
        0.016490,
        0.016317,
        0.061286,
        0.036698,
        0.032176,
        0.018519,
        0.015648,
        0.011531,
        0.008245,
};

UNITTEST_TESTCASE(fp16_hypot)
{

    for (int Qx = FP16_Q0; Qx < sizeof(fp16_props)/sizeof(*fp16_props); Qx++)
    {

        float maxerr = 0;

        for(int rep = 0; rep < REPETITIONS; rep++)
        {
            float aflt = randf(fp16_props[Qx].min, fp16_props[Qx].max);
            float bflt = randf(fp16_props[Qx].min, fp16_props[Qx].max);
            float cflt = f_saturate(hypot(aflt,bflt),fp16_props[Qx].min,fp16_props[Qx].max);

            fp16_t afp = fp16_flt2fp(aflt,Qx);
            fp16_t bfp = fp16_flt2fp(bflt,Qx);
            fp16_t cfp = fp16_hypot(afp,bfp,Qx);

            float err = fabs(cflt-fp16_fp2flt(cfp,Qx));

            UNITTEST_ASSERT("Unexpected result", err <= hypot_maxerr_best_effort[Qx] );

            if (err > maxerr)
            {
                maxerr = err;
            }

            //UNITTEST_PRINTF("%f;%f\n", cflt, fp16_fp2flt(cfp,Qx));

        }

       // printf("%f,\n",maxerr*1.5);
    }
}

#define SIN_MAX_ERR 0.00174436


UNITTEST_TESTCASE(fp16_sin)
{

    for (float flt = fp16_props[FP16_Q15].min; flt <= fp16_props[FP16_Q15].max; flt+=fp16_props[FP16_Q15].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q15);
        fp = fp16_sin(fp);

        //UNITTEST_ASSERT("Unexpected result",fabs(sin(flt*M_PI)-fp16_fp2flt(fp,FP16_Q14)) <= SIN_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,sin(flt*M_PI),fp16_fp2flt(fp,FP16_Q14));

       // UNITTEST_PRINTF("%d\n",(int32_t)round((M_PI) * (1 << FP16_Q15)));
    }
}


#define COS_MAX_ERR 0.0018385


UNITTEST_TESTCASE(fp16_cos)
{

    for (float flt = fp16_props[FP16_Q15].min; flt <= fp16_props[FP16_Q15].max  ; flt+=fp16_props[FP16_Q15].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q15);
        fp = fp16_cos(fp);



        UNITTEST_ASSERT("Unexpected result",fabs(cos(flt*M_PI)-fp16_fp2flt(fp,FP16_Q14)) <= COS_MAX_ERR);


        // UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,cos(flt*M_PI),fp16_fp2flt(fp,FP16_Q14));
    }
}

UNITTEST_TESTCASE(fp16_tan)
{

    for (float flt = fp16_props[FP16_Q15].min; flt <= fp16_props[FP16_Q15].max  ; flt+=fp16_props[FP16_Q15].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q15);
        fp = fp16_tan(fp,FP16_Q8);

       UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(tan(flt*M_PI),FP16_Q8_MIN,FP16_Q8_MAX),fp16_fp2flt(fp,FP16_Q8));
    }
}



#define ASIN_MAX_ERR 0.00018311

UNITTEST_TESTCASE(fp16_asin)
{
    for (float flt = -1.0; flt <= +1.0  ; flt+=fp16_props[FP16_Q14].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q14);
        fp = fp16_asin(fp);

        UNITTEST_ASSERT("Unexpected result",fabs(asin(flt)-fp16_fp2flt(fp,FP16_Q14)) <= ASIN_MAX_ERR);

        // UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,asin(flt),fp16_fp2flt(fp,FP16_Q14));
    }
}


#define ACOS_MAX_ERR 0.0002391


UNITTEST_TESTCASE(fp16_acos)
{
    for (float flt = -1.0; flt <= +1.0  ; flt+=fp16_props[FP16_Q14].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q14);
        fp = fp16_acos(fp);

        UNITTEST_ASSERT("Unexpected result",fabs(acos(flt)-fp16_fp2flt(fp,FP16_Q13)) <= ACOS_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,acos(flt),fp16_fp2flt(fp,FP16_Q13));

    }
}



#define ATAN_MAX_ERR 0.00782



UNITTEST_TESTCASE(fp16_atan)
{
    for (float flt = fp16_props[FP16_Q8].min; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_atan(fp,FP16_Q8);

        UNITTEST_ASSERT("Unexpected result",fabs(atan(flt)-fp16_fp2flt(fp,FP16_Q14)) <= ATAN_MAX_ERR);


        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,atan(flt),fp16_fp2flt(fp,FP16_Q14));

    }
}



#define EXP_MAX_ERR_POS     1.042305
#define EXP_MAX_ERR_NEG     0.0040115



UNITTEST_TESTCASE(fp16_exp)
{
    for (float flt = fp16_props[FP16_Q8].min; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_exp(fp,FP16_Q8);

        if (flt > 0)
        {
            UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(exp(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= EXP_MAX_ERR_POS);

        }
        else
        {
            UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(exp(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= EXP_MAX_ERR_NEG);
        }

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(exp(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}


#define LOG_MAX_ERR 0.3421


UNITTEST_TESTCASE(fp16_log)
{
    for (float flt = fp16_props[FP16_Q8].prec; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_log(fp,FP16_Q8);

        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(log(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= LOG_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(log(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}


#define LOG10_MAX_ERR 0.154334


UNITTEST_TESTCASE(fp16_log10)
{
    for (float flt = fp16_props[FP16_Q8].prec; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_log10(fp,FP16_Q8);

        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(log10(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= LOG10_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(log10(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}

#define LOG2_MAX_ERR 0.5195313


UNITTEST_TESTCASE(fp16_log2)
{
    for (float flt = fp16_props[FP16_Q8].prec; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_log2(fp,FP16_Q8);

        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(log2(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= LOG2_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(log2(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}


#define SINH_MAX_ERR 1.20559

UNITTEST_TESTCASE(fp16_sinh)
{
    for (float flt = fp16_props[FP16_Q8].min; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_sinh(fp,FP16_Q8);


        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(sinh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= SINH_MAX_ERR);

        // UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(sinh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}


#define COSH_MAX_ERR 1.20576



UNITTEST_TESTCASE(fp16_cosh)
{
    for (float flt = fp16_props[FP16_Q8].min; flt <= fp16_props[FP16_Q8].max  ; flt+=fp16_props[FP16_Q8].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,FP16_Q8);
        fp = fp16_cosh(fp,FP16_Q8);


        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(cosh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q8)) <= COSH_MAX_ERR);

        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(cosh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max),fp16_fp2flt(fp,FP16_Q8));

    }
}

#define TANH_MAX_ERR 0.00590244
#define  TANH_FRAC  FP16_Q8

/*
UNITTEST_TESTCASE(fp16_tanh)
{
    for (float flt = fp16_props[TANH_FRAC].min; flt <= fp16_props[TANH_FRAC].max  ; flt+=fp16_props[TANH_FRAC].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,TANH_FRAC);
        fp = fp16_tanh(fp,TANH_FRAC);
        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(tanh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q14)) <= TANH_MAX_ERR);
        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(tanh(flt),fp16_props[TANH_FRAC].min,fp16_props[TANH_FRAC].max),fp16_fp2flt(fp,FP16_Q14));
    }
}
**/


UNITTEST_TESTCASE(fp16_tanh)
{
    for (float flt = fp16_props[TANH_FRAC].min; flt <= fp16_props[TANH_FRAC].max  ; flt+=fp16_props[TANH_FRAC].prec)
    {
        fp16_t fp = fp16_flt2fp(flt,TANH_FRAC);
        fp = fp16_tanh(fp,TANH_FRAC);
        UNITTEST_ASSERT("Unexpected result",fabs(f_saturate(tanh(flt),fp16_props[FP16_Q8].min,fp16_props[FP16_Q8].max)-fp16_fp2flt(fp,FP16_Q14)) <= TANH_MAX_ERR);
        //UNITTEST_PRINTF("%0.15f;%0.15f;%0.15f\n",flt,f_saturate(tanh(flt),fp16_props[TANH_FRAC].min,fp16_props[TANH_FRAC].max),fp16_fp2flt(fp,FP16_Q14));
    }
}



typedef struct {
    uint8_t frac;
    float nflt;
    float maxerr;
}ut_pow_tvec1_t;

ut_pow_tvec1_t ut_pow_tvec1 [] = {
        {FP16_Q8,   0.5,     0.072},
        {FP16_Q8,   1,       1.04688},
        {FP16_Q8,   1.5,     1.1985},
        {FP16_Q8,   2,       0.97609},
        {FP16_Q8,   2.5,     1.58067},
        {FP16_Q8,   3,       1.63257},
};


UNITTEST_TESTCASE(fp16_pow)
{
    /* Check with positive base and exponent */

    for(int idx = 0; idx < sizeof(ut_pow_tvec1)/sizeof(*ut_pow_tvec1); idx++)
    {
        float nflt = ut_pow_tvec1[idx].nflt;
        uint8_t frac = ut_pow_tvec1[idx].frac;
        float err, maxerr = 0;

        for (float xflt = 0; xflt <= fp16_props[frac].max  ; xflt+=fp16_props[frac].prec)
        {

            fp16_t xfp = fp16_flt2fp(xflt,frac);
            fp16_t nfp = fp16_flt2fp(nflt,frac);
            xfp = fp16_pow(xfp,nfp,frac);

            err = fabs(f_saturate(pow(xflt,nflt),fp16_props[FP16_Q8].min,fp16_props[frac].max)-fp16_fp2flt(xfp,frac));

            if(err > maxerr)
            {
                maxerr = err;
            }

            UNITTEST_ASSERT("Unexpected result",err <= ut_pow_tvec1[idx].maxerr);
        }

        // UNITTEST_PRINTF("tvecidx %d: maxerr=%f\n",idx,maxerr);
    }



    /* Check Domain Errors */

    /* i = sqrt(-1) */
    /* If the base is finite negative and the exponent is finite but not an integer value, it causes a domain error. */
    float nflt = 0.5;
    float xflt = -1.0;

    errno = 0;
    UNITTEST_ASSERT("Unexpected result",errno == 0);

    fp16_t xfp = fp16_flt2fp(xflt,FP16_Q8);
    fp16_t nfp = fp16_flt2fp(nflt,FP16_Q8);
    xfp = fp16_pow(xfp,nfp,FP16_Q8);

    UNITTEST_ASSERT("Unexpected result",errno == EDOM);

    /* If both base and exponent are zero, it may also cause a domain error on certain implementations. */

    nflt = 0.0;
    xflt = 0.0;

    errno = 0;
    UNITTEST_ASSERT("Unexpected result",errno == 0);

    xfp = fp16_flt2fp(xflt,FP16_Q8);
    nfp = fp16_flt2fp(nflt,FP16_Q8);
    xfp = fp16_pow(xfp,nfp,FP16_Q8);

    UNITTEST_ASSERT("Unexpected result",errno == EDOM);

    /* If base is zero and exponent is negative, it may cause a domain error or a pole error (or none, depending on the library implementation). */

    nflt = -1.0;
    xflt = 0.0;

    errno = 0;
    UNITTEST_ASSERT("Unexpected result",errno == 0);

    xfp = fp16_flt2fp(xflt,FP16_Q8);
    nfp = fp16_flt2fp(nflt,FP16_Q8);
    xfp = fp16_pow(xfp,nfp,FP16_Q8);

    UNITTEST_ASSERT("Unexpected result",errno == EDOM);


}








/* add additional unit test cases here */

#define UNITTEST_DEVELOPMENT 1

UNITTEST_TESTSUITE(fp16)
{
    srand((unsigned int)time(NULL));

    UNITTEST_TESTSUITE_BEGIN();

#if (!UNITTEST_DEVELOPMENT)

    UNITTEST_EXEC_TESTCASE(fp16_check_shiftops);
    UNITTEST_EXEC_TESTCASE(fp16_precision);
    UNITTEST_EXEC_TESTCASE(fp16_ftofp_fptof);
    UNITTEST_EXEC_TESTCASE(fp16_floor);
    UNITTEST_EXEC_TESTCASE(fp16_ceil);
    UNITTEST_EXEC_TESTCASE(fp16_trunc);
    UNITTEST_EXEC_TESTCASE(fp16_fabs);
    UNITTEST_EXEC_TESTCASE(fp16_abs);
    UNITTEST_EXEC_TESTCASE(fp16_fmod);
    UNITTEST_EXEC_TESTCASE(fp16_fp2int);
    UNITTEST_EXEC_TESTCASE(fp16_int2fp);
    UNITTEST_EXEC_TESTCASE(fp16_round);
    UNITTEST_EXEC_TESTCASE(fp16_lround);
    UNITTEST_EXEC_TESTCASE(fp16_sqrt);
    UNITTEST_EXEC_TESTCASE(fp16_cbrt);
    UNITTEST_EXEC_TESTCASE(fp16_hypot);
    UNITTEST_EXEC_TESTCASE(fp16_sin);
    UNITTEST_EXEC_TESTCASE(fp16_cos);
    //UNITTEST_EXEC_TESTCASE(fp16_tan);

    UNITTEST_EXEC_TESTCASE(fp16_asin);
    UNITTEST_EXEC_TESTCASE(fp16_acos);
    UNITTEST_EXEC_TESTCASE(fp16_atan);

    UNITTEST_EXEC_TESTCASE(fp16_sinh);
    UNITTEST_EXEC_TESTCASE(fp16_cosh);
    UNITTEST_EXEC_TESTCASE(fp16_tanh);


    UNITTEST_EXEC_TESTCASE(fp16_exp);
    UNITTEST_EXEC_TESTCASE(fp16_log);
    UNITTEST_EXEC_TESTCASE(fp16_log10);
    UNITTEST_EXEC_TESTCASE(fp16_log2);

    UNITTEST_EXEC_TESTCASE(fp16_pow);

#else

   UNITTEST_EXEC_TESTCASE(fp16_sin);


#endif











    /* add additional unit test cases to be executed here */

    UNITTEST_TESTSUITE_END();
}
