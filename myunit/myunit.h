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
#ifndef MYUNIT_H_
#define MYUNIT_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MYUNIT_VERBOSE MYUNIT_VERB2_FAILED_ASSERTS


#define MYUNIT_SILENT                   0   /*!< \brief No output, silent mode */
#define MYUNIT_VERB1_NO_ASSERTS         1   /*!< \brief Normal output, no asserts are printed */
#define MYUNIT_VERB1_FAILED_ASSERTS     2   /*!< \brief Normal output, only failed asserts are printed */
#define MYUNIT_VERB1_ALL_ASSERTS        3   /*!< \brief Normal output, all asserts are printed */
#define MYUNIT_VERB2_NO_ASSERTS         4   /*!< \brief Detailed output, no asserts are printed */
#define MYUNIT_VERB2_FAILED_ASSERTS     5   /*!< \brief Detailed output, only failed asserts are printed */
#define MYUNIT_VERB2_ALL_ASSERTS        6   /*!< \brief Detailed output, all asserts are printed */
#define MYUNIT_VERB3_NO_ASSERTS         7   /*!< \brief Extensive output, no asserts are printed */
#define MYUNIT_VERB3_FAILED_ASSERTS     8   /*!< \brief Extensive output, only failed asserts are printed */
#define MYUNIT_VERB3_ALL_ASSERTS        9   /*!< \brief Extensive output, all asserts are printed */



/*!
    \brief Default verbosity level
    \details If the verbosity level is not defined, it defaults to the highest verbosity level, MYUNIT_VERB3_ALL_ASSERTS.
*/
#if !defined(MYUNIT_VERBOSE)
   #define MYUNIT_VERBOSE  MYUNIT_VERB3_ALL_ASSERTS
#endif

#ifdef MYUNIT_LIST_UNITTESTS
    #undef MYUNIT_VERBOSE
    #define MYUNIT_VERBOSE  MYUNIT_SILENT
#endif




/*!
    \brief Sets the verbosity level based on MYUNIT_VERBOSE
    \details This block defines the verbosity level based on the value of MYUNIT_VERBOSE.
             It checks the current verbosity and assigns the corresponding verbosity level:
             - Level 1: MYUNIT_VERB1_* range
             - Level 2: MYUNIT_VERB2_* range
             - Level 3: MYUNIT_VERB3_* range
             If none of the conditions match, the verbosity is set to MYUNIT_SILENT.
*/
#if (MYUNIT_VERBOSE >= MYUNIT_VERB1_NO_ASSERTS) && (MYUNIT_VERBOSE <= MYUNIT_VERB1_ALL_ASSERTS)
    #define MYUNIT_VERBOSITY_LEVEL 1
#elif (MYUNIT_VERBOSE >= MYUNIT_VERB2_NO_ASSERTS) && (MYUNIT_VERBOSE <= MYUNIT_VERB2_ALL_ASSERTS)
    #define MYUNIT_VERBOSITY_LEVEL 2
#elif (MYUNIT_VERBOSE >= MYUNIT_VERB3_NO_ASSERTS) && (MYUNIT_VERBOSE <= MYUNIT_VERB3_ALL_ASSERTS)
    #define MYUNIT_VERBOSITY_LEVEL 3
#else
    #define MYUNIT_VERBOSITY_LEVEL MYUNIT_SILENT
#endif


/*!
    \brief Sets the assertion verbosity level based on MYUNIT_VERBOSE
    \details This block defines the verbosity level specifically for assertions, depending on the value of MYUNIT_VERBOSE:
             - Level 2: All assertions are printed (MYUNIT_VERB*_ALL_ASSERTS).
             - Level 1: Only failed assertions are printed (MYUNIT_VERB*_FAILED_ASSERTS).
             - Level 0: No assertions are printed.
*/
#if (MYUNIT_VERBOSE == MYUNIT_VERB1_ALL_ASSERTS) || (MYUNIT_VERBOSE == MYUNIT_VERB2_ALL_ASSERTS) || (MYUNIT_VERBOSE == MYUNIT_VERB3_ALL_ASSERTS)
    #define MYUNIT_ASSERT_VERBOSE_LEVEL  2
#elif (MYUNIT_VERBOSE == MYUNIT_VERB1_FAILED_ASSERTS) || (MYUNIT_VERBOSE == MYUNIT_VERB2_FAILED_ASSERTS) || (MYUNIT_VERBOSE == MYUNIT_VERB3_FAILED_ASSERTS)
    #define MYUNIT_ASSERT_VERBOSE_LEVEL  1
#else
    #define MYUNIT_ASSERT_VERBOSE_LEVEL  0
#endif

char *myunit_testsuite_name;                 /*!< Name of the current test suite */
char *myunit_testcase_name;                  /*!< Name of the current test case */
int myunit_testsuite_assert_fail_count;      /*!< Failed assertions in the test suite. */
int myunit_testsuite_assert_success_count;   /*!< Successful assertions in the test suite. */
int myunit_testcase_assert_success_count;    /*!< Successful assertions in the current test case. Reset after each test case. */
int myunit_testcase_assert_fail_count;       /*!< Failed assertions in the current test case. Reset after each test case.*/
int myunit_testcase_success_count;           /*!< Successfully completed test cases in the test suite. */
int myunit_testcase_fail_count;              /*!< Failed test cases in the test suite. */

void (*myunit_action)(void) = NULL;




/*!
    \brief Sets the action for handling assertion failures.

    \param[in] action Function to assign to `myunit_action`. Defaults to `NULL` (no action).
*/
#define MYUNIT_SET_ACTION(action) do{myunit_action=action;}while(0)


/*!
    \brief Initializes the platform for the unit testing framework.

    \details This function performs any platform-specific initialization required
             for the unit testing framework to operate. This may include setting up
             hardware or software resources.
*/
extern void myunit_platform_init(void);

/*!
    \brief Deinitializes the platform for the unit testing framework.

    \details This function releases any resources that were allocated during the
             platform initialization process. It ensures a clean shutdown of the
             unit testing framework on the platform.
*/
extern void myunit_platform_deinit(void);

/*!
    \brief Outputs formatted text to the platform's logging interface.

    \details This function sends formatted text, similar to `printf`, to the
             platform's designated logging or console output.

    \param[in] format A `printf`-style format string.
    \param[in] ...    Additional arguments matching the format string.

    \return The number of characters printed on success, or a negative value on error.
*/
extern int myunit_platform_printf(const char *format, ...);

/*!
    \brief Puts the platform into a safe state.

    \details This function transitions the platform into a safe state, which is
             typically used during critical failures or error conditions. The exact
             behavior depends on the platform implementation.
*/
extern void myunit_platform_safestate(void);

/*!
    \brief Halts execution on the platform.

    \details This function stops the execution of the platform. It is typically
             used to halt the system during fatal errors or debugging.
*/
extern void myunit_platform_halt(void);

/*!
    \brief Resets the platform.

    \details This function triggers a reset of the platform. It is often used
             to restart the system or recover from certain failure conditions.
*/
extern void myunit_platform_reset(void);

/*!
    \brief Handles platform-specific exceptions.

    \details This function must be invoked from handler mode, such as within
             an exception handler like the HardFault handler. It identifies the
             exception cause, logs diagnostic information, and transitions the
             platform to a safe state or performs a system reset.

    \note Handler mode is required to ensure this function operates with the
          appropriate privileges and access to critical system resources.
*/
extern void myunit_platform_exception(void);



#ifndef MYUNIT_CHECKPOINT_SIZE
#define MYUNIT_CHECKPOINT_SIZE 128 /*!< Defines the default number of checkpoints (128 bits). */
#endif

/*!
    \brief Declares the checkpoint storage array.
    \details This macro declares an array to store checkpoint bits. The size of the array is calculated based on the number of
             checkpoints requested, with each checkpoint occupying a single bit. The macro rounds up the size to the nearest byte.
    \param size The number of checkpoint bits to allocate, rounded to the nearest byte.
*/
#define MYUNIT_CHECKPOINTS(size) \
    uint8_t myunit_checkpoints[((size)+7)>>3]

MYUNIT_CHECKPOINTS(MYUNIT_CHECKPOINT_SIZE);


/*!
    \brief Sets a checkpoint at the given index
    \details This macro stores the current line number in the `myunit_checkpoints` array at the specified index.
    \param idx The index in the `myunit_checkpoints` array where the checkpoint will be set.
*/
void MYUNIT_CHECKPOINT_SET(uint8_t bit)
{
    myunit_checkpoints[((unsigned)(bit))>>3] |= (1<<(((unsigned)(bit))&0x07));
}

/*!
    \brief Clears a checkpoint at the given index
    \details This macro resets the value at the specified index in the `myunit_checkpoints` array to 0.
    \param idx The index in the `myunit_checkpoints` array where the checkpoint will be cleared.
*/
void MYUNIT_CHECKPOINT_CLR(uint8_t bit)
{
    myunit_checkpoints[((unsigned)(bit))>>3] &= ~(1<<(((unsigned)(bit))&0x07));
}






/*!
    \brief Checks if a checkpoint at the given index has been passed.
    \details This macro checks whether the specified checkpoint bit has been set in the `myunit_checkpoints` array.
             It returns a non-zero value if the bit is set (i.e., the checkpoint has been passed), and zero if the bit is not set.
    \param bit The index of the checkpoint to check, where the macro verifies if the checkpoint has been passed.
    \return A non-zero value if the checkpoint is passed, zero otherwise.
*/
bool MYUNIT_CHECKPOINT_PASSED(uint8_t bit)
{
    return ((unsigned)((myunit_checkpoints[((unsigned)(bit))>>3] & ((1<<(((unsigned)(bit))&0x07)))) > 0));
}


/*!
    \brief Checks if a checkpoint at the given index has been missed.
    \details This macro checks whether the specified checkpoint bit has not been set in the `myunit_checkpoints` array.
             It returns a non-zero value if the bit is not set (i.e., the checkpoint has been missed), and zero if the bit is set.
    \param bit The index of the checkpoint to check, where the macro verifies if the checkpoint has been missed.
    \return A non-zero value if the checkpoint is missed, zero otherwise.
*/
#define MYUNIT_CHECKPOINT_MISSED(bit) (!MYUNIT_CHECKPOINT_PASSED(bit))



#define MYUNIT_FLAG_SET(bit) MYUNIT_CHECKPOINT_SET(bit)
#define MYUNIT_FLAG_CLR(bit) MYUNIT_CHECKPOINT_CLR(bit)
#define MYUNIT_FLAG_IS_SET(bit) MYUNIT_CHECKPOINT_PASSED(bit)
#define MYUNIT_FLAG_IS_CLR(bit) MYUNIT_CHECKPOINT_MISSED(bit)




/*!
    \brief Retrieves the total number of checkpoints.
    \details This macro calculates the total number of checkpoint bits available in the `myunit_checkpoints` array.
             It determines the size of the array in bytes and multiplies it by 8 to get the total number of bits.
    \return The total number of available checkpoint bits.
*/
#define MYUNIT_CHECKPOINTS_SIZE() ((sizeof(myunit_checkpoints)/sizeof(*myunit_checkpoints))<<3)

/*!
    \brief Initializes all checkpoints
    \details This macro initializes all elements in the `myunit_checkpoints` array by clearing each checkpoint.
             It iterates through the array and resets every index to 0.
*/
#define MYUNIT_CHECKPOINS_INIT()   memset(myunit_checkpoints,0x00,sizeof(myunit_checkpoints))


/*!
    \brief Handles printing and assertion logging based on verbosity level
    \details This block defines macros for printing output and logging assertions based on the verbosity level.
             When the verbosity is set to a non-silent mode, the macros for printing (`MYUNIT_PRINTF`) and
             assertion logging (`MYUNIT_ASSERT_PRINTF`) are enabled. Additionally, various tags are defined for
             marking the start and end of test cases and test suites.
             - For verbosity levels 1, 2, and 3, different levels of detail are printed for assertions.
             - If the verbosity level is set to `MYUNIT_SILENT`, no output is generated.
*/
/*!
    \def MYUNIT_PRINTF
    \brief Macro for printing output during unit tests
    \details When verbosity is enabled, this macro prints formatted output using `myunit_platform_printf`.
    \param ... Variable arguments passed to the underlying platform's print function.
*/
#if (MYUNIT_VERBOSITY_LEVEL !=  MYUNIT_SILENT)

    #define MYUNIT_PRINTF(...) myunit_platform_printf(__VA_ARGS__)

    #define myunit_testcase_begin_tag       "<TCB>" /*!< Tag indicating the start of a test case */
    #define myunit_testcase_failure_tag     "<TCF>" /*!< Tag indicating a test case failed */
    #define myunit_testcase_passed_tag      "<TCP>" /*!< Tag indicating a test case failed */
    #define myunit_testcase_end_tag         "<TCE>" /*!< Tag indicating the end of a test case */
    #define myunit_testsuite_begin_tag      "<TSB>" /*!< Tag indicating the start of a test suite */
    #define myunit_testsuite_end_tag        "<TSE>" /*!< Tag indicating the end of a test suite */
    #define myunit_sequence_begin_tag       "<SQB>" /*!< Tag indicating the start of a test sequence */
    #define myunit_sequence_end_tag         "<SQE>" /*!< Tag indicating the end of a test sequence */


    #if ( MYUNIT_VERBOSITY_LEVEL == 1 )
        #define  MYUNIT_ASSERT_PRINTF(result,message,test)   \
            MYUNIT_PRINTF("%s %s %s %d\n",result,myunit_testsuite_name,myunit_testcase_name,__LINE__)
    #elif ( MYUNIT_VERBOSITY_LEVEL == 2 )
        #define  MYUNIT_ASSERT_PRINTF(result,message,test)   \
            MYUNIT_PRINTF("%s %s %s %d \"%s\"\n",result,myunit_testsuite_name,myunit_testcase_name,__LINE__,message)
    #elif ( MYUNIT_VERBOSITY_LEVEL == 3 )
        #define  MYUNIT_ASSERT_PRINTF(result,message,test)   \
            MYUNIT_PRINTF("%s %s %s %d \"%s\" \"%s\"\n",result,myunit_testsuite_name,myunit_testcase_name,__LINE__,message, #test)
    #else
        #error "MYUNIT : Invalid verbose mode"
    #endif

#else
    #define MYUNIT_PRINTF(...)    /*!< No-op macro when verbosity is disabled */
    #define MYUNIT_ASSERT_PRINTF(...)    /*!< No-op macro when verbosity is disabled */
#endif


/*!
    \brief Defines a test suite and its execution entry point.
    \details This macro creates a test suite by defining a function that will execute the test cases
             in the suite. It also sets up the `main` function to run the test suite, returning
             `EXIT_SUCCESS` if all tests pass and `EXIT_FAILURE` if any test fails.
    \param name The name of the test suite, which is used to generate the function names and variables associated with the suite.
    \note The macro generates a function `name##_exec` where the test suite's logic is implemented.
*/
#define MYUNIT_TESTSUITE(name)                                              \
    static const char *const_myunit_testsuite_name = #name;                 \
    int name##_exec();                                                      \
    int main() { return (name##_exec())?EXIT_FAILURE:EXIT_SUCCESS; }        \
    int name##_exec()

/*!
    \brief Marks the beginning of a test suite.
    \details This macro initializes the platform and test suite setup functions, assigns the test suite name, and
             resets various counters for assertions and test cases. It also prints a message indicating the start
             of the test suite, including the file name, date, and time of the test suite execution.
    \note The printed output includes:
          - Test suite begin tag (`myunit_testsuite_begin_tag`)
          - Test suite name (`myunit_testsuite_name`)
          - The current file name (`__FILE__`)
          - The current date (`__DATE__`)
          - The current time (`__TIME__`)
*/
#define MYUNIT_TESTSUITE_BEGIN()                                                        \
    {                                                                                   \
        myunit_platform_init();                                                         \
        myunit_testsuite_setup();                                                       \
        myunit_testsuite_name = (char*)const_myunit_testsuite_name;                     \
        myunit_testsuite_assert_fail_count = 0;                                         \
        myunit_testsuite_assert_success_count = 0;                                      \
        myunit_testcase_success_count = 0;                                              \
        myunit_testcase_fail_count = 0;                                                 \
        MYUNIT_PRINTF("%s %s \"%s\" \"%s\" \"%s\"\n",                                   \
        myunit_testsuite_begin_tag,myunit_testsuite_name,__FILE__,__DATE__,__TIME__);

/*!
    \brief Marks the end of a test suite and prints the summary.
    \details This macro concludes the execution of a test suite by printing the results, including the number of failed
             and successful test cases and assertions. It then calls the teardown function for the test suite and
             deinitializes the platform. The macro finally returns the number of failed test cases as the function result.
    \note The printed output includes:
          - Test suite end tag (`myunit_testsuite_end_tag`)
          - Test suite name (`myunit_testsuite_name`)
          - Number of failed test cases (`myunit_testcase_fail_count`)
          - Number of failed assertions (`myunit_testsuite_assert_fail_count`)
          - Number of successful test cases (`myunit_testcase_success_count`)
          - Number of successful assertions (`myunit_testsuite_assert_success_count`)
*/
#define MYUNIT_TESTSUITE_END()                          \
        MYUNIT_PRINTF("%s %s %d %d %d %d\n",            \
            myunit_testsuite_end_tag,                   \
            myunit_testsuite_name,                      \
            myunit_testcase_fail_count,                 \
            myunit_testsuite_assert_fail_count,         \
            myunit_testcase_success_count,              \
            myunit_testsuite_assert_success_count);     \
        myunit_testsuite_teardown();                    \
        myunit_platform_deinit();                       \
        return (myunit_testcase_fail_count);            \
    }

/*!
    \brief Defines a test case within a test suite.
    \details This macro creates a function for the specified test case. Each test case is implemented as a function
             with the name `myunit_testcase_##name`, where `name` is the provided name of the test case.
    \param name The name of the test case, which will be used to generate the function name.
    \note The actual implementation of the test case should be placed inside the generated function.
*/
#define MYUNIT_TESTCASE(name)  void myunit_testcase_##name(void)


/*!
    \brief Executes a specific test case within a test suite.
    \details This macro calls the `myunit_exec_testcase` function to execute a test case, passing the function pointer
             to the test case and its name as a string for logging purposes.
    \param name The name of the test case to be executed. This should correspond to the name used in `MYUNIT_TESTCASE`.
*/

#define MYUNIT_EXEC_TESTCASE(name) \
        myunit_exec_testcase(#name, myunit_testcase_##name)


/*!
    \brief Executes a test case within the current test suite.
    \details This function runs a specified test case and prints relevant information about the test case's execution,
             including the test case's start and end. It updates assertion and test case counters based on the results of the test case.
    \param testcase A pointer to the function representing the test case to be executed.
    \param name The name of the test case, used for logging purposes.
    \note The function updates both the test case and test suite assertion success and failure counts.
    \note The printed output includes:
          - The test case begin tag (`myunit_testcase_begin_tag`)
          - The test case end tag (`myunit_testcase_end_tag`)
          - The test suite name (`myunit_testsuite_name`)
          - The test case name (`name`)
          - The number of failed assertions in the test case (`myunit_testcase_assert_fail_count`)
*/
void myunit_exec_testcase(char *name, void(*testcase)())
{
    myunit_testcase_assert_fail_count  = myunit_testcase_assert_success_count = 0;
    myunit_testcase_name = name;
    MYUNIT_PRINTF("%s %s %s\n",myunit_testcase_begin_tag,myunit_testsuite_name,name);
    testcase();
    MYUNIT_PRINTF("%s %s %s %d %d\n",myunit_testcase_end_tag,myunit_testsuite_name,name,myunit_testcase_assert_fail_count,myunit_testcase_assert_success_count);
    myunit_testsuite_assert_fail_count+=myunit_testcase_assert_fail_count;
    myunit_testsuite_assert_success_count+=myunit_testcase_assert_success_count;
    (myunit_testcase_assert_fail_count)?(myunit_testcase_fail_count++):(myunit_testcase_success_count++);
}


/*!
    \brief Configures assertion result printing based on verbosity level.
    \details This block defines how passed and failed assertions are logged, depending on the `MYUNIT_ASSERT_VERBOSE_LEVEL`:
             - If verbosity is set to level 2, both passed and failed assertions are printed.
             - If verbosity is set to level 1, only failed assertions are printed.
             - If verbosity is set to 0, neither passed nor failed assertions are printed.
*/
/*!
    \def MYUNIT_ASSERT_PASSED_PRINTF
    \brief Prints a message for a passed assertion if verbosity level is 2.
    \param assert_result_text The result of the assertion (e.g., "PASS").
    \param message The custom message associated with the assertion.
    \param test The actual test condition that was evaluated.
*/
/*!
    \def MYUNIT_ASSERT_FAILED_PRINTF
    \brief Prints a message for a failed assertion if verbosity level is 2 or 1.
    \param assert_result_text The result of the assertion (e.g., "FAIL").
    \param message The custom message associated with the assertion.
    \param test The actual test condition that was evaluated.
*/
#if ( MYUNIT_ASSERT_VERBOSE_LEVEL  == 2 )
    #define MYUNIT_ASSERT_PASSED_PRINTF(assert_result_text,message,test) MYUNIT_ASSERT_PRINTF(assert_result_text,message,test)
    #define MYUNIT_ASSERT_FAILED_PRINTF(assert_result_text,message,test) MYUNIT_ASSERT_PRINTF(assert_result_text,message,test)
#elif ( MYUNIT_ASSERT_VERBOSE_LEVEL  == 1 )
    #define MYUNIT_ASSERT_PASSED_PRINTF(assert_result_text,message,test)
    #define MYUNIT_ASSERT_FAILED_PRINTF(assert_result_text,message,test) MYUNIT_ASSERT_PRINTF(assert_result_text,message,test)
#else
    #define MYUNIT_ASSERT_PASSED_PRINTF(assert_result_text,message,test)
    #define MYUNIT_ASSERT_FAILED_PRINTF(assert_result_text,message,test)
#endif


/*!
    \brief Evaluates a test condition and logs the result.
    \details This macro checks if the provided test condition is true or false:
             - If the test fails, it increments the failure count and prints a failure message.
             - If the test passes, it increments the success count and prints a success message (depending on verbosity).
    \param message A custom message associated with the assertion.
    \param test The test condition to be evaluated.
    \note The output behavior for passed and failed assertions depends on the verbosity level set by `MYUNIT_ASSERT_VERBOSE_LEVEL`.
*/
#define MYUNIT_ASSERT(message,test)                                                 \
    do{                                                                             \
        if (!(test))                                                                \
        {                                                                           \
            myunit_testcase_assert_fail_count++;                                    \
            MYUNIT_ASSERT_FAILED_PRINTF(myunit_testcase_failure_tag,message,test);  \
            if(myunit_action) myunit_action();                                      \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            myunit_testcase_assert_success_count++;                                 \
            MYUNIT_ASSERT_PASSED_PRINTF(myunit_testcase_passed_tag,message,test);   \
        }                                                                           \
    }while(0)

/*!
    \brief Asserts that two memory regions are equal.
    \details This macro compares two memory regions of the specified size and asserts that they are equal.
             If the memory regions are not equal, the assertion will fail.
    \param mem1 Pointer to the first memory region.
    \param mem2 Pointer to the second memory region.
    \param size The number of bytes to compare between the two memory regions.
*/
#define MYUNIT_ASSERT_MEM_EQUAL(mem1,mem2,size) \
    MYUNIT_ASSERT("MEM_EQUAL",memcmp(mem1,mem2,size) == 0)

/*!
    \brief Asserts that two memory regions are different.
    \details This macro compares two memory regions of the specified size and asserts that they are different.
             If the memory regions are the same, the assertion will fail.
    \param mem1 Pointer to the first memory region.
    \param mem2 Pointer to the second memory region.
    \param size The number of bytes to compare between the two memory regions.
*/
#define MYUNIT_ASSERT_MEM_DIFFER(mem1,mem2,size) \
    MYUNIT_ASSERT("MEM_DIFFERENT",memcmp(mem1,mem2,size) != 0)

/*!
    \brief Asserts that two values are equal.
    \details This macro compares two values and asserts that they are equal. If the values are not equal, the assertion will fail.
    \param var1 The first value to compare.
    \param var2 The second value to compare.
*/
#define MYUNIT_ASSERT_EQUAL(var1,var2) \
    MYUNIT_ASSERT("EQUAL",var1 == var2)

/*!
    \brief Asserts that two values are different.
    \details This macro compares two values and asserts that they are different. If the values are equal, the assertion will fail.
    \param var1 The first value to compare.
    \param var2 The second value to compare.
*/
#define MYUNIT_ASSERT_DIFFER(var1,var2) \
    MYUNIT_ASSERT("DIFFER",var1 != var2)




/*!
    \brief Checks if a variable is within a specified range.

    This macro evaluates whether `var` lies between `min` and `max`, inclusive.
*/
#define MYUNIT_INRANGE(var,min,max) \
        (((var) <= (max))&&((var) >= (min)))


/*!
    \brief Asserts that a variable is within a specified range.

    Asserts if `var` lies between `min` and `max`, inclusive.
   
    \param var The variable being checked, expected to be of comparable type with min and max.
    \param min The minimum boundary of the range (inclusive).
    \param max The maximum boundary of the range (inclusive).
*/
#define MYUNIT_ASSERT_INRANGE(var,min,max) \
    MYUNIT_ASSERT("INRANGE",MYUNIT_INRANGE(var,min,max))



/*!
    \brief Asserts that a given variable is within the valid range for a uint32_t.

    This macro checks if `var` is greater or equal to 0 and less than or equal to
    UINT32_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_UINT32_RANGE(var) \
    MYUNIT_ASSERT("UINT32_RANGE", MYUNIT_INRANGE(var,0,UINT32_MAX))


/*!
    \brief Asserts that a given variable is within the valid range for a int32_t.

    This macro checks if `var` is greater than or equal to INT32_MIN and less than or equal to
    INT32_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_INT32_RANGE(var) \
        MYUNIT_ASSERT("UINT32_RANGE", MYUNIT_INRANGE(var,INT32_MIN,INT32_MAX))

/*!
    \brief Asserts that a given variable is within the valid range for a uint16_t.

    This macro checks if `var` is greater than or equal to 0 and less than or equal to
    UINT16_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_UINT16_RANGE(var) \
        MYUNIT_ASSERT("UINT16_RANGE", MYUNIT_INRANGE(var,0,UINT16_MAX))


/*!
    \brief Asserts that a given variable is within the valid range for a int16_t.

    This macro checks if `var` is greater than or equal to INT16_MIN and less than or equal to
    INT16_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_INT16_RANGE(var) \
    MYUNIT_ASSERT("INT16_RANGE", MYUNIT_INRANGE(var,INT16_MIN,INT16_MAX))

/*!
    \brief Asserts that a given variable is within the valid range for a uint8_t.

    This macro checks if `var` is greater than or equal to 0 and less than or equal to
    UINT8_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_UINT8_RANGE(var) \
        MYUNIT_ASSERT("UINT8_RANGE", MYUNIT_INRANGE(var,0,UINT8_MAX))


/*!
    \brief Asserts that a given variable is within the valid range for a int8_t.

    This macro checks if `var` is greater than or equal to INT8_MIN and less than or equal to
    INT8_MAX.

    \param var  The variable being checked
*/
#define MYUNIT_ASSERT_INT8_RANGE(var) \
    MYUNIT_ASSERT("INT8_RANGE", MYUNIT_INRANGE(var,INT8_MIN,INT8_MAX))

/*!
    \brief Asserts that a specific checkpoint has been passed.
    \details This macro checks if the checkpoint at the specified index has been reached (i.e., its value is not zero).
             If the checkpoint has not been passed, the assertion will fail.
    \param idx The index of the checkpoint to check in the `myunit_checkpoints` array.
*/
#define MYUNIT_ASSERT_CHECKPOINT_PASSED(idx)       \
    MYUNIT_ASSERT("CHKPNT_PASSED", MYUNIT_CHECKPOINT_PASSED(idx))

/*!
    \brief Asserts that a specific checkpoint has been missed.
    \details This macro checks if the checkpoint at the specified index has not been reached (i.e., its value is zero).
             If the checkpoint has been passed, the assertion will fail.
    \param idx The index of the checkpoint to check in the `myunit_checkpoints` array.
*/
#define MYUNIT_ASSERT_CHECKPOINT_MISSED(idx)       \
    MYUNIT_ASSERT("CHKPNT_MISSED", MYUNIT_CHECKPOINT_MISSED(idx))


#define MYUNIT_ASSERT_FLAG_CLR(idx)       \
    MYUNIT_ASSERT("FLAG_CLR", MYUNIT_FLAG_IS_CLR(idx))

#define MYUNIT_ASSERT_FLAG_SET(idx)       \
    MYUNIT_ASSERT("FLAG_SET", MYUNIT_FLAG_IS_SET(idx))


/*!
    \brief Asserts that a specific bit in a variable is cleared (i.e., it is 0).
    \details This macro checks if the bit at the specified position in the given variable is cleared.
             If the bit is set, the assertion will fail.
    \param var The variable to check.
    \param pos The bit position to check, where 0 is the least significant bit.
*/
#define MYUNIT_ASSERT_BIT_CLR(var, pos) \
    MYUNIT_ASSERT("BIT_CLR", (!((var) & (1 << (pos)))))

/*!
    \brief Asserts that a specific bit in a variable is set (i.e., it is 1).
    \details This macro checks if the bit at the specified position in the given variable is set.
             If the bit is cleared, the assertion will fail.
    \param var The variable to check.
    \param pos The bit position to check, where 0 is the least significant bit.
*/
#define MYUNIT_ASSERT_BIT_SET(var, pos) \
    MYUNIT_ASSERT("BIT_SET",  !(!((var) & (1 << (pos)))))


#define MYUNIT_SEQUENCE_BEGIN() \
        { \
            bool seq_passed = false; \
            { \
                int myunit_testcase_assert_fail_count = 0; \
                int myunit_testcase_assert_success_count = 0;\
                MYUNIT_PRINTF("%s %s %d\n",myunit_sequence_begin_tag,myunit_testsuite_name,__LINE__)


#define MYUNIT_SEQUENCE_END(status) \
                seq_passed = MYUNIT_HAS_SEQUENCE_PASSED(); \
                MYUNIT_PRINTF("%s %s %d %d\n",myunit_sequence_end_tag,myunit_testsuite_name,__LINE__,seq_passed); \
            }while(0); \
            MYUNIT_ASSERT_SEQUENCE_##status(); \
        }while(0)



/*! Following macros must only be used within a test sequence section!*/
#define MYUNIT_SEQUENCE_STATUS() (myunit_testcase_assert_fail_count)
#define MYUNIT_HAS_SEQUENCE_PASSED() (MYUNIT_SEQUENCE_STATUS() == 0)
#define MYUNIT_HAS_SEQUENCE_FAILED() (!MYUNIT_HAS_SEQUENCE_PASSED())

/*! Sequence assertion macros are private and may only be used by MYUNIT_SEQUENCE_END macro! */
#define MYUNIT_ASSERT_SEQUENCE_PASSED() \
    MYUNIT_ASSERT("SEQ_PASSED",seq_passed)
/*!Sequence assertion macros are private and may only be used by MYUNIT_SEQUENCE_END macro! */
#define MYUNIT_ASSERT_SEQUENCE_FAILED() \
    MYUNIT_ASSERT("SEQ_FAILED",!seq_passed)



#define MYUNIT_ASSERT_TRUE(cond) \
    MYUNIT_ASSERT("TRUE",  (cond) == true)

#define MYUNIT_ASSERT_FALSE(cond) \
    MYUNIT_ASSERT("FALSE",  (cond) == false)


#define MYUNIT_ASSERT_IS_NULL(ptr)       \
    MYUNIT_ASSERT("IS_NULL", (ptr) == NULL )

#define MYUNIT_ASSERT_NOT_NULL(ptr)       \
    MYUNIT_ASSERT("IS_NULL", (ptr) != NULL )

#endif /* MYUNIT_H_ */
