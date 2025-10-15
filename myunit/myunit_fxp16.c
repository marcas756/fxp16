/*****************************************************************************
 *                                                                           *
 *  MyUnit – A lightweight unit testing framework for embedded C projects    *
 *                                                                           *
 *  Forked from uCUnit (C) 2007–2008 Sven Stefan Krauss                      *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  (C) 2025 Marco Bacchi                                                    *
 *      marco@bacchi.at                                                      *
 *                                                                           *
 *  File        : myunit.h                                                   *
 *  Description : Macros for unit testing                                    *
 *  Author      : Marco Bacchi                                               *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of MyUnit, a fork of uCUnit.
 *
 * It is licensed under the Common Public License Version 1.0 (CPL-1.0),
 * as published by IBM Corporation and approved by the Open Source Initiative.
 *
 * MyUnit incorporates substantial modifications and extensions to the original
 * uCUnit codebase. The original work is:
 *   (C) 2007–2008 Sven Stefan Krauss, https://www.ucunit.org
 *
 * This file is distributed under the CPL in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPL for details.
 *
 * A copy of the license should have been provided with this distribution,
 * or can be found online at:
 *     http://www.opensource.org/licenses/cpl1.0.txt
 */
#include "myunit.h"



#include "fxp16.h"
#include "math.h"
#include "stdio.h"
#include <float.h>



typedef struct {
    double mean;   // laufender Mittelwert
    double stddev; // laufende Standardabweichung (Stichprobe)
    double min;
    double max;
    double sum;    // Summe aller x
    double m2;     // Summe der quadrierten Abweichungen (Welford)
    int n;
} stat_t;

void stat_init(stat_t *stat)
{
    stat->max   = -DBL_MAX;
    stat->min   =  DBL_MAX;
    stat->mean  = 0.0;
    stat->stddev= 0.0;
    stat->sum   = 0.0;
    stat->m2    = 0.0;
    stat->n     = 0;
}

void stat_acc(stat_t *stat, double x)
{
    // Min/Max
    if (x > stat->max) stat->max = x;
    if (x < stat->min) stat->min = x;

    // Zähler & Summe
    stat->n++;
    stat->sum += x;

    // Welford-Update für mean und M2
    double delta  = x - stat->mean;
    stat->mean   += delta / stat->n;
    double delta2 = x - stat->mean;
    stat->m2     += delta * delta2;

    // Stichproben-Standardabweichung (n-1 im Nenner, Bessel)
    if (stat->n > 1)
    {
        double var = stat->m2 / (stat->n - 1);
        stat->stddev = sqrt(var);
    }
    else
    {
        stat->stddev = 0.0;
    }
}



void fxp16_sincos_stats_snapshot(void)
{
    stat_t statsin, statcos, statasin, statacos;

    stat_init(&statsin);
    stat_init(&statcos);
    stat_init(&statasin);
    stat_init(&statacos);


    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x++)
    {
        float fp_result = fxp16_fp2flt(fxp16_sin((fxp16_t)fp_x),FXP16_Q15);
        float flt_x = M_PI*fxp16_fp2flt(fp_x,FXP16_Q15);
        float flt_result = sin(flt_x);
        float err = fabs(flt_result-fp_result);

        stat_acc(&statsin,err);

        fp_result = fxp16_fp2flt(fxp16_cos((fxp16_t)fp_x),FXP16_Q15);
        flt_x = M_PI*fxp16_fp2flt(fp_x,FXP16_Q15);
        flt_result = cos(flt_x);
        err = fabs(flt_result-fp_result);

        stat_acc(&statcos,err);


        fp_result = M_PI*fxp16_fp2flt(fxp16_asin((fxp16_t)fp_x),FXP16_Q15);
        flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);
        flt_result = asin(flt_x);
        err = fabs(flt_result-fp_result);

        stat_acc(&statasin,err);


        fp_result = M_PI*fxp16_fp2flt(fxp16_acos((fxp16_t)fp_x),FXP16_Q15);
        flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);
        flt_result = acos(flt_x);
        err = fabs(flt_result-fp_result);

        printf("%f;%f\n", fp_result,err );

        stat_acc(&statacos,err);
    }

    printf("#define FXP16_SIN_ERR_MAX %.15f \n", statsin.max);
    printf("#define FXP16_SIN_ERR_MIN %.15f \n", statsin.min);
    printf("#define FXP16_SIN_ERR_AVG %.15f \n", statsin.mean);
    printf("#define FXP16_SIN_ERR_STDDEV %.15f \n", statsin.stddev);

    printf("#define FXP16_COS_ERR_MAX %.15f \n", statcos.max);
    printf("#define FXP16_COS_ERR_MIN %.15f \n", statcos.min);
    printf("#define FXP16_COS_ERR_AVG %.15f \n", statcos.mean);
    printf("#define FXP16_COS_ERR_STDDEV %.15f \n", statcos.stddev);

    printf("#define FXP16_ASIN_ERR_MAX %.15f \n", statasin.max);
    printf("#define FXP16_ASIN_ERR_MIN %.15f \n", statasin.min);
    printf("#define FXP16_ASIN_ERR_AVG %.15f \n", statasin.mean);
    printf("#define FXP16_ASIN_ERR_STDDEV %.15f \n", statasin.stddev);

    printf("#define FXP16_ACOS_ERR_MAX %.15f \n", statacos.max);
    printf("#define FXP16_ACOS_ERR_MIN %.15f \n", statacos.min);
    printf("#define FXP16_ACOS_ERR_AVG %.15f \n", statacos.mean);
    printf("#define FXP16_ACOS_ERR_STDDEV %.15f \n", statacos.stddev);
}


void fxp16_print_sincos_csv(void)
{
    printf("\"x\";\"sin(x)\";\"cos(x)\"\n");

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x+=256)
    {
       float fp_result = fxp16_fp2flt(fxp16_sin((fxp16_t)fp_x),FXP16_Q15);
       float flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);


       printf("%f;%f;",flt_x,fp_result);

       fp_result = fxp16_fp2flt(fxp16_cos((fxp16_t)fp_x),FXP16_Q15);
       flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);

       printf("%f\n",fp_result);
    }
}




#define MYUNIT_GUARD_UPPER 1.01
#define MYUNIT_GUARD_LOWER 0.99

// Vorzeichenabhängige Guard-Hilfen:
// - Für negative Sollwerte: untere Grenze weiter ins Negative, obere näher zu 0
// - Für positive Sollwerte: untere Grenze näher zu 0, obere weiter ins Positive
#define MYUNIT_GUARDED_LOWER(x) ( ((x) < 0.0f) ? (x)*(MYUNIT_GUARD_UPPER) : (x)*(MYUNIT_GUARD_LOWER) )
#define MYUNIT_GUARDED_UPPER(x) ( ((x) < 0.0f) ? (x)*(MYUNIT_GUARD_LOWER) : (x)*(MYUNIT_GUARD_UPPER) )


#define FXP16_SIN_ERR_MAX 0.000343501567841
#define FXP16_SIN_ERR_MIN 0.000000000000000
#define FXP16_SIN_ERR_AVG 0.000073066253162
#define FXP16_SIN_ERR_STDDEV 0.000057542890207
#define FXP16_COS_ERR_MAX 0.000370681285858
#define FXP16_COS_ERR_MIN 0.000000000000000
#define FXP16_COS_ERR_AVG 0.000077220334278
#define FXP16_COS_ERR_STDDEV 0.000060814559970
#define FXP16_ASIN_ERR_MAX 0.000392913818359
#define FXP16_ASIN_ERR_MIN 0.000000000000000
#define FXP16_ASIN_ERR_AVG 0.000086193425977
#define FXP16_ASIN_ERR_STDDEV 0.000063566804682
#define FXP16_ACOS_ERR_MAX 0.000488758087158
#define FXP16_ACOS_ERR_MIN 0.000000000000000
#define FXP16_ACOS_ERR_AVG 0.000101883960923
#define FXP16_ACOS_ERR_STDDEV 0.000075309966979


MYUNIT_TESTCASE(fxp16_sin)
{
    stat_t stat;

    stat_init(&stat);

    for (int32_t fp_angle = INT16_MIN; fp_angle <= INT16_MAX; fp_angle++)
    {
        float fp_result = fxp16_fp2flt(fxp16_sin((fxp16_t)fp_angle),FXP16_Q15);
        float flt_angle = M_PI*fxp16_fp2flt(fp_angle,FXP16_Q15);
        float flt_result = sin(flt_angle);
        float err = fabs(flt_result-fp_result);



        stat_acc(&stat,err);
    }


    MYUNIT_ASSERT_INRANGE(stat.min,MYUNIT_GUARDED_LOWER(FXP16_SIN_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_SIN_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.max,MYUNIT_GUARDED_LOWER(FXP16_SIN_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_SIN_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.mean,MYUNIT_GUARDED_LOWER(FXP16_SIN_ERR_AVG),MYUNIT_GUARDED_UPPER(FXP16_SIN_ERR_AVG));
}


MYUNIT_TESTCASE(fxp16_cos)
{
    stat_t stat;

    stat_init(&stat);

    for (int32_t fp_angle = INT16_MIN; fp_angle <= INT16_MAX; fp_angle++)
    {
        float fp_result = fxp16_fp2flt(fxp16_cos((fxp16_t)fp_angle),FXP16_Q15);
        float flt_angle = M_PI*fxp16_fp2flt(fp_angle,FXP16_Q15);
        float flt_result = cos(flt_angle);
        float err = fabs(flt_result-fp_result);

        stat_acc(&stat,err);
    }


    MYUNIT_ASSERT_INRANGE(stat.min,MYUNIT_GUARDED_LOWER(FXP16_COS_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_COS_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.max,MYUNIT_GUARDED_LOWER(FXP16_COS_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_COS_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.mean,MYUNIT_GUARDED_LOWER(FXP16_COS_ERR_AVG),MYUNIT_GUARDED_UPPER(FXP16_COS_ERR_AVG));

}


typedef struct {
    int q;
    float qmax;
    float qmin;
    float max;
    float min;
    float mean;
    float stddev;
}myunit_tvec_t;




const myunit_tvec_t myunit_tan_tvec [] = {
        {FXP16_Q0,FXP16_Q0_MAX,FXP16_Q0_MIN,5953.622070312500000,0.000000000000000,0.974373269338132,41.171228169357200},
        {FXP16_Q1,FXP16_Q1_MAX,FXP16_Q1_MIN,2976.811035156250000,0.000000000000000,0.586450701907485,24.775607470474974},
        {FXP16_Q2,FXP16_Q2_MAX,FXP16_Q2_MIN,2976.561035156250000,0.000000000000000,0.409517584822433,20.057355795868730},
        {FXP16_Q3,FXP16_Q3_MAX,FXP16_Q3_MIN,1938.438964843750000,0.000000000000000,0.282774604100553,13.809559846886284},
        {FXP16_Q4,FXP16_Q4_MAX,FXP16_Q4_MIN,787.219482421875000,0.000000000000000,0.151594940458610,5.768131130738064},
        {FXP16_Q5,FXP16_Q5_MAX,FXP16_Q5_MIN,94.552185058593750,0.000000000000000,0.064647465612933,1.376480557336769},
        {FXP16_Q6,FXP16_Q6_MAX,FXP16_Q6_MIN,35.004577636718750,0.000000000000000,0.034236654974063,0.542630082777712},
        {FXP16_Q7,FXP16_Q7_MAX,FXP16_Q7_MIN,12.622955322265625,0.000000000000000,0.017927393730375,0.224727075072795},
        {FXP16_Q8,FXP16_Q8_MAX,FXP16_Q8_MIN,2.835418701171875,0.000000000000000,0.008237381662399,0.068800955806037},
        {FXP16_Q9,FXP16_Q9_MAX,FXP16_Q9_MIN,0.606903076171875,0.000000000000000,0.004084797970175,0.022858778048510},
        {FXP16_Q10,FXP16_Q10_MAX,FXP16_Q10_MIN,0.196939468383789,0.000000000000000,0.002125471169110,0.009118067152425},
        {FXP16_Q11,FXP16_Q11_MAX,FXP16_Q11_MIN,0.067139625549316,0.000000000000000,0.001078372874722,0.003373444498749},
        {FXP16_Q12,FXP16_Q12_MAX,FXP16_Q12_MIN,0.018020153045654,0.000000000000000,0.000560105395821,0.001210396641787},
        {FXP16_Q13,FXP16_Q13_MAX,FXP16_Q13_MIN,0.005579233169556,0.000000000000000,0.000308416622478,0.000471573508183},
        {FXP16_Q14,FXP16_Q14_MAX,FXP16_Q14_MIN,0.001777648925781,0.000000000000000,0.000177141921457,0.000180487749927},
        {FXP16_Q15,FXP16_Q15_MAX,FXP16_Q15_MIN,0.000631332397461,0.000000000000000,0.000120943719947,0.000090870401745}
};


void myunit_tan_compute_stats(uint8_t Q, stat_t *stat)
{

    stat_init(stat);

    for (int32_t fp_angle = INT16_MIN; fp_angle <= INT16_MAX; fp_angle++)
    {

        float fp_result = fxp16_fp2flt(fxp16_tan((fxp16_t)fp_angle, Q), Q);
        float  flt_result = tan(M_PI*fxp16_fp2flt(fp_angle,FXP16_Q15));

        // skip non-finite and clipped values
        if (!isfinite(fp_result) || !isfinite(flt_result))
            continue;
        if (fp_result >= FXP16_MAX_FROM(Q) || fp_result <= FXP16_MIN_FROM(Q))
            continue;

        float err = fabs(flt_result-fp_result);

        if (!isfinite(err))
            continue;

        stat_acc(stat,err);
     }



}


void fxp16_tan_stats_snapshot(void)
{
    stat_t stat;

    for(uint8_t Q = 0; Q <= FXP16_Q15; Q++)
    {
        myunit_tan_compute_stats(Q, &stat);
        printf("{FXP16_Q%d,FXP16_Q%d_MAX,FXP16_Q%d_MIN,%.15f,%.15f,%.15f,%.15f},\n",Q,Q,Q,stat.max,stat.min,stat.mean,stat.stddev);
    }
}




MYUNIT_TESTCASE(fxp16_tan)
{

    stat_t stat;

    for(int idx = 0; idx < sizeof(myunit_tan_tvec)/sizeof(*myunit_tan_tvec); idx++)
    {
        MYUNIT_PRINTF("Processing testvector FXP16_Q%d ...\n",myunit_tan_tvec[idx].q );

        myunit_tan_compute_stats(myunit_tan_tvec[idx].q, &stat);

        MYUNIT_ASSERT_INRANGE(stat.min,    MYUNIT_GUARDED_LOWER(myunit_tan_tvec[idx].min),
                                   MYUNIT_GUARDED_UPPER(myunit_tan_tvec[idx].min));

        MYUNIT_ASSERT_INRANGE(stat.max,    MYUNIT_GUARDED_LOWER(myunit_tan_tvec[idx].max),
                                   MYUNIT_GUARDED_UPPER(myunit_tan_tvec[idx].max));

        MYUNIT_ASSERT_INRANGE(stat.mean,    MYUNIT_GUARDED_LOWER(myunit_tan_tvec[idx].mean),
                                   MYUNIT_GUARDED_UPPER(myunit_tan_tvec[idx].mean));
    }
}





void fxp16_atan2_stats_snapshot(void)
{
    stat_t stat;
    stat_init(&stat);



    for (int32_t fp_y = INT16_MIN; fp_y < INT16_MAX; fp_y+=2)
    {


            // 0 .. 100: Prozent (65535 = letzter Index → 100 %)
            int percent = (int)((((uint32_t)(fp_y - (int32_t)INT16_MIN)) *100) / (double)UINT16_MAX);

            MYUNIT_PRINTF("Status %d \%\n",percent);



        for (int32_t fp_x = INT16_MIN; fp_x < INT16_MAX; fp_x+=2)
        {
            float fp_result = M_PI*fxp16_fp2flt(fxp16_atan2((fxp16_t)fp_y, (fxp16_t)fp_x),FXP16_Q15);
            float flt_x= fxp16_fp2flt(fp_x,FXP16_Q15);
            float flt_y= fxp16_fp2flt(fp_y,FXP16_Q15);
            float flt_result = atan2(flt_y,flt_x);
            float err = fabs(flt_result-fp_result);
            stat_acc(&stat,err);
        }
    }


    printf("#define FXP16_ATAN2_ERR_MAX %.15f \n", stat.max);
    printf("#define FXP16_ATAN2_ERR_MIN %.15f \n", stat.min);
    printf("#define FXP16_ATAN2_ERR_AVG %.15f \n", stat.mean);
    printf("#define FXP16_ATAN2_ERR_STDDEV %.15f \n", stat.stddev);
}

void fxp16_print_atan2_csv(void)
{
    printf("\"x\";\"y\";\"atan2(x)\"\n");

    for (int32_t fp_y = INT16_MIN; fp_y < INT16_MAX; fp_y+=512)
    {
        for (int32_t fp_x = INT16_MIN; fp_x < INT16_MAX; fp_x+=512)
        {
            float flt_x= fxp16_fp2flt(fp_x,FXP16_Q15);
            float flt_y= fxp16_fp2flt(fp_y,FXP16_Q15);
            float fp_result = fxp16_fp2flt(fxp16_atan2((fxp16_t)fp_y, (fxp16_t)fp_x),FXP16_Q15);

            printf("%f;%f;%f\n",flt_x,flt_y,fp_result);
        }
    }
}

#define FXP16_ATAN2_ERR_MAX 0.107259750366211
#define FXP16_ATAN2_ERR_MIN 0.000000000000000
#define FXP16_ATAN2_ERR_AVG 0.000107545578110
#define FXP16_ATAN2_ERR_STDDEV 0.000103821588797


MYUNIT_TESTCASE(fxp16_atan2)
{
        stat_t stat;
        stat_init(&stat);



        for (int32_t fp_y = INT16_MIN; fp_y < INT16_MAX; fp_y+=8)
        {

            if (fp_y%2048== 0)
            {
                // 0 .. 100: Prozent (65535 = letzter Index → 100 %)
                int percent = (int)((((uint32_t)(fp_y - (int32_t)INT16_MIN)) *100) / (double)UINT16_MAX);
                MYUNIT_PRINTF("Status %d \%\n",percent);
            }

            for (int32_t fp_x = INT16_MIN; fp_x < INT16_MAX; fp_x+=8)
            {
                float fp_result = M_PI*fxp16_fp2flt(fxp16_atan2((fxp16_t)fp_y, (fxp16_t)fp_x),FXP16_Q15);
                float flt_x= fxp16_fp2flt(fp_x,FXP16_Q15);
                float flt_y= fxp16_fp2flt(fp_y,FXP16_Q15);
                float flt_result = atan2(flt_y,flt_x);
                float err = fabs(flt_result-fp_result);
                stat_acc(&stat,err);
            }
        }



        MYUNIT_ASSERT_INRANGE(stat.min,MYUNIT_GUARDED_LOWER(FXP16_ATAN2_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ATAN2_ERR_MAX));
        MYUNIT_ASSERT_INRANGE(stat.max,MYUNIT_GUARDED_LOWER(FXP16_ATAN2_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ATAN2_ERR_MAX));
        MYUNIT_ASSERT_INRANGE(stat.mean,MYUNIT_GUARDED_LOWER(FXP16_ATAN2_ERR_AVG),MYUNIT_GUARDED_UPPER(FXP16_ATAN2_ERR_AVG));

}



void fxp16_print_atan_csv(void)
{

    for(uint8_t Q = FXP16_Q0; Q <= FXP16_Q15; Q++)
    {
        printf("\"x_q%d\";\"atan(x_q%d)\";",Q,Q);
    }

    printf("\n");


    for (int32_t fp_x = INT16_MIN; fp_x < INT16_MAX; fp_x+=512)
    {
        for(uint8_t Q = FXP16_Q0; Q <= FXP16_Q15; Q++)
        {
            float flt_x= fxp16_fp2flt(fp_x,Q);
            float fp_result = fxp16_fp2flt(fxp16_atan((fxp16_t)fp_x,Q),FXP16_Q15);
            printf("%f;%f;",flt_x,fp_result);
        }





        printf("\n");



    }

}

void myunit_atan_compute_stats(uint8_t Q, stat_t *stat)
{

    stat_init(stat);

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x++)
    {

        float fp_result = M_PI*fxp16_fp2flt(fxp16_atan((fxp16_t)fp_x,Q),FXP16_Q15);
        float flt_x= fxp16_fp2flt(fp_x,Q);
        float flt_result = atan(flt_x);

        float err = fabs(flt_result-fp_result);

        if (!isfinite(err))
            continue;

        stat_acc(stat,err);
     }
}

void fxp16_atan_stats_snapshot(void)
{
    stat_t stat;
    stat_init(&stat);

    for(uint8_t Q = 0; Q <= FXP16_Q15; Q++)
    {
        myunit_atan_compute_stats(Q, &stat);
        printf("{FXP16_Q%d,FXP16_Q%d_MAX,FXP16_Q%d_MIN,%.15f,%.15f,%.15f,%.15f},\n",Q,Q,Q,stat.max,stat.min,stat.mean,stat.stddev);
    }
}



const myunit_tvec_t myunit_atan_tvec [] = {
        {FXP16_Q0,FXP16_Q0_MAX,FXP16_Q0_MIN,0.000257968902588,0.000000000000000,0.000047706172154,0.000029252064485},
        {FXP16_Q1,FXP16_Q1_MAX,FXP16_Q1_MIN,0.000267028808594,0.000000000000000,0.000041944553232,0.000039646232268},
        {FXP16_Q2,FXP16_Q2_MAX,FXP16_Q2_MIN,0.000317931175232,0.000000000000000,0.000067591245624,0.000044291924041},
        {FXP16_Q3,FXP16_Q3_MAX,FXP16_Q3_MIN,0.000321388244629,0.000000000000000,0.000071546309755,0.000054914047206},
        {FXP16_Q4,FXP16_Q4_MAX,FXP16_Q4_MIN,0.000335216522217,0.000000000000000,0.000083025489175,0.000058227878419},
        {FXP16_Q5,FXP16_Q5_MAX,FXP16_Q5_MIN,0.000335216522217,0.000000000000000,0.000080531449044,0.000058952394724},
        {FXP16_Q6,FXP16_Q6_MAX,FXP16_Q6_MIN,0.000367760658264,0.000000000000000,0.000086743040271,0.000062094790036},
        {FXP16_Q7,FXP16_Q7_MAX,FXP16_Q7_MIN,0.000374913215637,0.000000000000000,0.000089634715252,0.000063742121491},
        {FXP16_Q8,FXP16_Q8_MAX,FXP16_Q8_MIN,0.000374913215637,0.000000000000000,0.000078695575858,0.000058280102740},
        {FXP16_Q9,FXP16_Q9_MAX,FXP16_Q9_MIN,0.000374913215637,0.000000000000000,0.000086481457661,0.000063536611216},
        {FXP16_Q10,FXP16_Q10_MAX,FXP16_Q10_MIN,0.000374913215637,0.000000000000000,0.000086077172531,0.000063309576058},
        {FXP16_Q11,FXP16_Q11_MAX,FXP16_Q11_MIN,0.000355720520020,0.000000000000000,0.000083891158962,0.000061563174364},
        {FXP16_Q12,FXP16_Q12_MAX,FXP16_Q12_MIN,0.000356793403625,0.000000000000000,0.000086529940748,0.000063065909219},
        {FXP16_Q13,FXP16_Q13_MAX,FXP16_Q13_MIN,0.000358700752258,0.000000000000000,0.000087169923351,0.000063665448271},
        {FXP16_Q14,FXP16_Q14_MAX,FXP16_Q14_MIN,0.000347167253494,0.000000000000000,0.000085683717197,0.000062590258928},
        {FXP16_Q15,FXP16_Q15_MAX,FXP16_Q15_MIN,0.000347167253494,0.000000000000000,0.000085944880066,0.000062622966195}
};


MYUNIT_TESTCASE(fxp16_atan)
{

    stat_t stat;

    for(int idx = 0; idx < sizeof(myunit_atan_tvec)/sizeof(*myunit_atan_tvec); idx++)
    {
        MYUNIT_PRINTF("Processing testvector FXP16_Q%d ...\n",myunit_atan_tvec[idx].q );

        myunit_atan_compute_stats(myunit_atan_tvec[idx].q, &stat);

        MYUNIT_ASSERT_INRANGE(stat.min,    MYUNIT_GUARDED_LOWER(myunit_atan_tvec[idx].min),
                                   MYUNIT_GUARDED_UPPER(myunit_atan_tvec[idx].min));

        MYUNIT_ASSERT_INRANGE(stat.max,    MYUNIT_GUARDED_LOWER(myunit_atan_tvec[idx].max),
                                   MYUNIT_GUARDED_UPPER(myunit_atan_tvec[idx].max));

        MYUNIT_ASSERT_INRANGE(stat.mean,    MYUNIT_GUARDED_LOWER(myunit_atan_tvec[idx].mean),
                                   MYUNIT_GUARDED_UPPER(myunit_atan_tvec[idx].mean));
    }
}


void myunit_sqrt_compute_stats(uint8_t Q, stat_t *stat)
{

    stat_init(stat);

    for (int32_t fp_x = 0; fp_x <= INT16_MAX; fp_x++)
    {

        float fp_result = fxp16_fp2flt(fxp16_sqrt((fxp16_t)fp_x,Q),Q);
        float flt_x= fxp16_fp2flt(fp_x,Q);
        float flt_result = sqrt(flt_x);

        float err = fabs(flt_result-fp_result);

        //printf("%f;%f;%f\n",flt_x,flt_result,fp_result);

        stat_acc(stat,err);
     }
}


void fxp16_sqrt_stats_snapshot(void)
{
    stat_t stat;
    stat_init(&stat);

    for(uint8_t Q = FXP16_Q0; Q <= FXP16_Q15; Q++)
    {

        myunit_sqrt_compute_stats(Q, &stat);
        printf("{FXP16_Q%d,FXP16_Q%d_MAX,FXP16_Q%d_MIN,%.15f,%.15f,%.15f,%.15f},\n",Q,Q,Q,stat.max,stat.min,stat.mean,stat.stddev);
    }
}


const myunit_tvec_t myunit_sqrt_tvec [] = {
    {FXP16_Q0,FXP16_Q0_MAX,FXP16_Q0_MIN,0.997238159179688,0.000000000000000,0.498047396700710,0.288763579427037},
    {FXP16_Q1,FXP16_Q1_MAX,FXP16_Q1_MIN,0.499015808105469,0.000000000000000,0.249344470457799,0.144346237610908},
    {FXP16_Q2,FXP16_Q2_MAX,FXP16_Q2_MIN,0.249656677246094,0.000000000000000,0.124740861436294,0.072195749676760},
    {FXP16_Q3,FXP16_Q3_MAX,FXP16_Q3_MIN,0.124877929687500,0.000000000000000,0.062660516478900,0.036092310617114},
    {FXP16_Q4,FXP16_Q4_MAX,FXP16_Q4_MIN,0.062458038330078,0.000000000000000,0.031299721489631,0.018052031538075},
    {FXP16_Q5,FXP16_Q5_MAX,FXP16_Q5_MIN,0.031234741210938,0.000000000000000,0.015705258239450,0.009024424447474},
    {FXP16_Q6,FXP16_Q6_MAX,FXP16_Q6_MIN,0.015619277954102,0.000000000000000,0.007839268093448,0.004514047640074},
    {FXP16_Q7,FXP16_Q7_MAX,FXP16_Q7_MIN,0.007810592651367,0.000000000000000,0.003931120330662,0.002256688481633},
    {FXP16_Q8,FXP16_Q8_MAX,FXP16_Q8_MIN,0.003905296325684,0.000000000000000,0.001961383047274,0.001128939898477},
    {FXP16_Q9,FXP16_Q9_MAX,FXP16_Q9_MIN,0.001952648162842,0.000000000000000,0.000983275619433,0.000564418321677},
    {FXP16_Q10,FXP16_Q10_MAX,FXP16_Q10_MIN,0.000976562500000,0.000000000000000,0.000490562529649,0.000282414289547},
    {FXP16_Q11,FXP16_Q11_MAX,FXP16_Q11_MIN,0.000488281250000,0.000000000000000,0.000245828254663,0.000141202538371},
    {FXP16_Q12,FXP16_Q12_MAX,FXP16_Q12_MIN,0.000244140625000,0.000000000000000,0.000122592554135,0.000070681710443},
    {FXP16_Q13,FXP16_Q13_MAX,FXP16_Q13_MIN,0.000122070312500,0.000000000000000,0.000061431936246,0.000035329369041},
    {FXP16_Q14,FXP16_Q14_MAX,FXP16_Q14_MIN,0.000061035156250,0.000000000000000,0.000030611060879,0.000017695375384},
    {FXP16_Q15,FXP16_Q15_MAX,FXP16_Q15_MIN,0.000030517578125,0.000000000000000,0.000015341695061,0.000008844031636}
};

void fxp16_print_tan_table_csv(void)
{
    printf("\"x\";\"tan(x)/Q0\";\"tan(x)/Q1\";\"tan(x)/Q2\";\"tan(x)/Q3\";\"tan(x)/Q4\";\"tan(x)/Q5\";\"tan(x)/Q6\";\"tan(x)/Q7\";\"tan(x)/Q8\";\"tan(x)/Q9\";\"tan(x)/Q10\";\"tan(x)/Q11\";\"tan(x)/Q12\";\"tan(x)/Q13\";\"tan(x)/Q14\";\"tan(x)/Q15\"\n");

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x+=256)
    {
        float flt_x= fxp16_fp2flt(fp_x,FXP16_Q15);

        printf("%f",flt_x);

        for(uint8_t Q = FXP16_Q0; Q <= FXP16_Q15; Q++)
        {
            float fp_result = fxp16_fp2flt(fxp16_tan((fxp16_t)fp_x,Q),Q);
            printf(";%f",fp_result);
        }
        printf("\n");
    }
}


MYUNIT_TESTCASE(fxp16_sqrt)
{

    stat_t stat;

    for(int idx = 0; idx < sizeof(myunit_sqrt_tvec)/sizeof(*myunit_sqrt_tvec); idx++)
    {
        MYUNIT_PRINTF("Processing testvector FXP16_Q%d ...\n",myunit_sqrt_tvec[idx].q );

        myunit_sqrt_compute_stats(myunit_sqrt_tvec[idx].q, &stat);

        MYUNIT_ASSERT_INRANGE(stat.min,    MYUNIT_GUARDED_LOWER(myunit_sqrt_tvec[idx].min),
                                   MYUNIT_GUARDED_UPPER(myunit_sqrt_tvec[idx].min));

        MYUNIT_ASSERT_INRANGE(stat.max,    MYUNIT_GUARDED_LOWER(myunit_sqrt_tvec[idx].max),
                                   MYUNIT_GUARDED_UPPER(myunit_sqrt_tvec[idx].max));

        MYUNIT_ASSERT_INRANGE(stat.mean,    MYUNIT_GUARDED_LOWER(myunit_sqrt_tvec[idx].mean),
                                   MYUNIT_GUARDED_UPPER(myunit_sqrt_tvec[idx].mean));
    }


}



void fxp16_print_asinacos_table_csv(void)
{
    printf("\"x\";\"asin(x)\";\"acos(x)\"\n");

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x+=16)
    {
       float fp_result = fxp16_fp2flt(fxp16_asin((fxp16_t)fp_x),FXP16_Q15);
       float flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);


       printf("%f;%f;",flt_x,fp_result);

       fp_result = fxp16_fp2flt(fxp16_acos((fxp16_t)fp_x),FXP16_Q15);
       flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);

       printf("%f\n",fp_result);
    }
}


MYUNIT_TESTCASE(fxp16_asin)
{
    stat_t stat;

    stat_init(&stat);

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x++)
    {
        float fp_result = M_PI*fxp16_fp2flt(fxp16_asin((fxp16_t)fp_x),FXP16_Q15);
        float flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);
        float flt_result = asin(flt_x);
        float err = fabs(flt_result-fp_result);

        stat_acc(&stat,err);
    }


    MYUNIT_ASSERT_INRANGE(stat.min,MYUNIT_GUARDED_LOWER(FXP16_ASIN_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ASIN_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.max,MYUNIT_GUARDED_LOWER(FXP16_ASIN_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ASIN_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.mean,MYUNIT_GUARDED_LOWER(FXP16_ASIN_ERR_AVG),MYUNIT_GUARDED_UPPER(FXP16_ASIN_ERR_AVG));
}


MYUNIT_TESTCASE(fxp16_acos)
{
    stat_t stat;

    stat_init(&stat);

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x++)
    {
        float fp_result = M_PI*fxp16_fp2flt(fxp16_acos((fxp16_t)fp_x),FXP16_Q15);
        float flt_x = fxp16_fp2flt(fp_x,FXP16_Q15);
        float flt_result = acos(flt_x);
        float err = fabs(flt_result-fp_result);

        stat_acc(&stat,err);
    }


    MYUNIT_ASSERT_INRANGE(stat.min,MYUNIT_GUARDED_LOWER(FXP16_ACOS_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ACOS_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.max,MYUNIT_GUARDED_LOWER(FXP16_ACOS_ERR_MIN),MYUNIT_GUARDED_UPPER(FXP16_ACOS_ERR_MAX));
    MYUNIT_ASSERT_INRANGE(stat.mean,MYUNIT_GUARDED_LOWER(FXP16_COS_ERR_AVG),MYUNIT_GUARDED_UPPER(FXP16_ACOS_ERR_AVG));
}

/*
void fxp16_print_sinhcosh_table_csv(void)
{
    printf("\"x\";\"sinh(x)\";\"cosh(x)\"\n");

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x+=128)
    {
       float fp_result = fxp16_fp2flt(fxp16_sinh((fxp16_t)fp_x,FXP16_Q8),FXP16_Q8);
       float flt_x = fxp16_fp2flt(fp_x,FXP16_Q8);


       printf("%f;%f;",flt_x,fp_result);

       fp_result = fxp16_fp2flt(fxp16_cosh((fxp16_t)fp_x,FXP16_Q8),FXP16_Q8);
       flt_x = fxp16_fp2flt(fp_x,FXP16_Q8);

       printf("%f\n",fp_result);
    }
}
*/


//fxp16_t fxp16_fp2fp(fxp16_t fp, uint8_t fracold, uint8_t fracnew)
//{
//    fxp32_t result = fp;
//    fpxx_ashift_m(result, fracold - fracnew);
//    fxp16_sat_m(result);
//    return (fxp16_t) result;
//}

void fxp16_print_sinhcosh_table_csv(void)
{


    printf("\"x\";\"sinh(x)\";\"cosh(x)\";\"tanh(x)\"\n");

    for (int32_t fp_x = INT16_MIN; fp_x <= INT16_MAX; fp_x+=16)
    {

        float fp_result = fxp16_fp2flt(fxp16_sinh(FXP16_Q4, fp_x,FXP16_Q8),FXP16_Q4);
        float flt_x = fxp16_fp2flt(fp_x,FXP16_Q8);

        printf("%f;%f;",flt_x,fp_result);

        fp_result = fxp16_fp2flt(fxp16_cosh(FXP16_Q4, fp_x,FXP16_Q8),FXP16_Q4);

        printf("%f;",fp_result);

        fp_result = fxp16_fp2flt(fxp16_tanh(FXP16_Q4, fp_x,FXP16_Q8),FXP16_Q4);

        printf("%f\n",fp_result);

    }
}




MYUNIT_TESTCASE(fxp16_sinh)
{


}



void myunit_testsuite_setup()
{
    

}

void myunit_testsuite_teardown()
{

}

MYUNIT_TESTSUITE(selftest)
{
    MYUNIT_TESTSUITE_BEGIN();

   //fxp16_sincos_stats_snapshot();
   MYUNIT_EXEC_TESTCASE(fxp16_sin);
   MYUNIT_EXEC_TESTCASE(fxp16_cos);

    //fxp16_tan_stats_snapshot();
    MYUNIT_EXEC_TESTCASE(fxp16_tan);
    //fxp16_print_tan_table_csv();


    //fxp16_atan2_stats_snapshot();
    //fxp16_print_atan2_csv();
    //MYUNIT_EXEC_TESTCASE(fxp16_atan2);

   // fxp16_atan_stats_snapshot();
   //MYUNIT_EXEC_TESTCASE(fxp16_atan);
    //fxp16_print_atan_csv();


   MYUNIT_EXEC_TESTCASE(fxp16_asin);
   MYUNIT_EXEC_TESTCASE(fxp16_acos);


   //fxp16_sqrt_stats_snapshot();
   MYUNIT_EXEC_TESTCASE(fxp16_sqrt);

   //fxp16_print_sincos_table_csv();
   //fxp16_print_asinacos_table_csv();


   MYUNIT_EXEC_TESTCASE(fxp16_sinh);
   fxp16_print_sinhcosh_table_csv();




   MYUNIT_TESTSUITE_END();
}
