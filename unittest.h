/*! \copyright
    Copyright (c) 2013 marcas756@gmail.com
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
    \file       unittest.h

    \brief      Provides preprocessor macros to write unit tests

    \details
*/

#ifndef UNITTEST_H_
#define UNITTEST_H_
#include <stdlib.h>
#include <stdio.h>

#define UNITTEST_VERBOSE 1
#define UNITTEST_TALKATIVE 1

static int uttcerrcnt;
static int uttserrcnt;

#define UNITTEST_NEW_LOGFILE(filename) \
      FILE *fp = freopen(filename, "w" ,stdout); \
      fclose(fp);

#define UNITTEST_LOG2FILE(filename,...) \
      freopen(filename, "a" ,stdout); \
      UNITTEST_PRINTF(__VA_ARGS__); \
      fclose(fp);

#if UNITTEST_VERBOSE

    static char *uttsname;
    static char *uttcname;

    extern void unittest_printf_function ( const char * format, ... );
    #define UNITTEST_PRINTF(...) unittest_printf_function(__VA_ARGS__)

    #define uttcf  "<UTTCF>"
    #define uttcb  "<UTTCB>"
    #define uttsb  "<UTTSB>"
    #define uttce  "<UTTCE>"
    #define uttse  "<UTTSE>"


    static void unittest_printf_sss(const char *s1, const char *s2, const char *s3)
    {
        UNITTEST_PRINTF("%s %s %s\n",s1,s2,s3);
    }

    static void unittest_printf_sssd(const char *s1, const char *s2, const char *s3, int d)
    {
        UNITTEST_PRINTF("%s %s %s %d\n",s1,s2,s3,d);
    }

    #if UNITTEST_TALKATIVE
        static void unittest_printf_sssds(const char *s1, const char *s2, const char *s3, int d, char *s4)
        {
            UNITTEST_PRINTF("%s %s %s %d \"%s\"\n",s1,s2,s3,d,s4);
        }
    #endif

#else /* UNITTEST_VERBOSE */

    #define UNITTEST_PRINTF(...)

#endif /* UNITTEST_VERBOSE */

#if UNITTEST_VERBOSE

    #define UNITTEST_TESTCASE(name) \
        static void unittest_##name()

    #define UNITTEST_EXEC_TESTCASE(name)                                \
        do{                                                             \
            uttcerrcnt  = 0;                                            \
            uttcname = #name;                                           \
            unittest_printf_sss(uttcb,uttsname,uttcname);               \
            unittest_##name();                                          \
            unittest_printf_sssd(uttce,uttsname,uttcname,uttcerrcnt);   \
            uttserrcnt+=uttcerrcnt;                                     \
        }while(0)

        /* message is not compiled into code, thus it can be used as "in code" information (like remarks) */

        #if UNITTEST_TALKATIVE
            #define UNITTEST_ASSERT(message,test) \
                    do{if(!(test)){unittest_printf_sssds(uttcf,uttsname,uttcname,__LINE__,message);uttcerrcnt++;}}while(0)
        #else
            #define UNITTEST_ASSERT(message,test) \
                    do{if(!(test)){unittest_printf_sssd(uttcf,uttsname,uttcname,__LINE__);uttcerrcnt++;}}while(0)
        #endif


#else /* !UNITTEST_VERBOSE */
    #define UNITTEST_TESTCASE(name) \
        static void unittest_##name()

    #define UNITTEST_EXEC_TESTCASE(name)    \
        do{                                 \
            uttcerrcnt = 0;                 \
            unittest_##name();              \
            uttserrcnt+=uttcerrcnt;         \
        }while(0)

#define UNITTEST_ASSERT(message,test) \
        do{if(!(test)){uttcerrcnt++;}}while(0)
#endif /* UNITTEST_VERBOSE */


#if UNITTEST_VERBOSE

    #define UNITTEST_TESTSUITE(name)                 \
        static const char *const_uttsname = #name;   \
        int main()


    #define UNITTEST_TESTSUITE_BEGIN()                           \
        {                                                        \
            uttsname = (char*)const_uttsname;                    \
            uttserrcnt = 0;                                      \
            UNITTEST_PRINTF("%s %s \"%s\" \"%s\" \"%s\"\n",     \
            uttsb,uttsname,__FILE__,__DATE__,__TIME__);


    #define UNITTEST_TESTSUITE_END()                                      \
            UNITTEST_PRINTF("%s %s %d\n",uttse,uttsname,uttserrcnt);    \
        }                                                                 \
        return (uttserrcnt != 0)?EXIT_FAILURE:EXIT_SUCCESS;

#else /* UNITTEST_VERBOSE */

    #define UNITTEST_TESTSUITE(name)    \
            int main()

    #define UNITTEST_TESTSUITE_BEGIN()  { uttserrcnt = 0;
    #define UNITTEST_TESTSUITE_END()    } return (uttserrcnt != 0)?EXIT_FAILURE:EXIT_SUCCESS;

#endif /* UNITTEST_VERBOSE */

#endif /* UNITTEST_H_ */
