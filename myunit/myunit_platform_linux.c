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

#include <stdarg.h>
#include <stdio.h>


int myunit_platform_printf ( const char * format, ... )
{
    int retval;
    va_list arglist;
    va_start( arglist, format );
    retval = vprintf( format, arglist );
    va_end( arglist );
    return retval;
}

void myunit_platform_init(void)
{
    // Step 1: Perform hardware initialization
    // Example: Initialize GPIOs, timers, or communication interfaces
    // init_hardware_resources();

    // Step 2: Configure platform-specific logging
    // Example: Set up UART for printf-like functionality or enable debug logging
    // configure_logging_interface();

    // Step 3: Return success or log failure in case of issues
    // if (initialization_failed) {
    //     log_error("Platform initialization failed");
    //     return false;
    // }
    // return true;
}

void myunit_platform_deinit(void)
{
    // Step 1: Deinitialize hardware resources
    // Example: Turn off peripherals or release communication interfaces
    // deinit_hardware_resources();

    // Step 2: Disable logging or cleanup any open resources
    // Example: Close UART, file handles, or memory-mapped I/O
    // disable_logging_interface();

    // Step 3: Confirm successful cleanup
    // log_info("Platform successfully deinitialized");
}

void myunit_platform_safestate(void)
{
    // Step 1: Disable all non-critical systems
    // Example: Stop timers, disable interrupts, or turn off peripherals
    // disable_non_critical_systems();

    // Step 2: Enable fail-safe mechanisms
    // Example: Trigger a watchdog or put the system in a low-power mode
    // enable_failsafe();

    // Step 3: Log the safe state transition for debugging
    // log_warning("System transitioned to safe state");
}

void myunit_platform_halt(void)
{
    // Step 1: Stop all system processes
    // Example: Disable Watchdog and enter an infinite loop to halt execution
    while (1);

    // Optional: Provide a debug message before halting
    // log_critical("System halted");
}

void myunit_platform_reset(void)
{
    // Step 1: Perform necessary pre-reset actions
    // Example: Save logs, flush buffers, or ensure critical data is safe
    // save_critical_state();

    // Step 2: Trigger a system reset
    // Example: Use a hardware register or watchdog timer
    // NVIC_SystemReset();
}

void myunit_platform_exception(void)
{
    // This function must be called in handler mode, e.g., from the HardFault handler.
    // Example: Ensure proper context switching or stack frame inspection.

    // Step 1: Identify the exception cause
    // Example: Read exception flags or error registers
    // uint32_t exception_cause = read_exception_cause();

    // Step 2: Log the exception for diagnostics
    // log_error("Exception occurred, cause: %d", exception_cause);

    // Step 3: Transition to a safe state or reset the system
    // myunit_platform_safestate();
    // Alternatively: myunit_platform_reset();

    myunit_platform_halt();
}






