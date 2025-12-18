/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MAIN_H    /* Guard against multiple inclusion */
#define _MAIN_H

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include <stdint.h>
#include <string.h>

#include "../nPZero_Driver/Inc/npz.h"
#include "../nPZero_Driver/Inc/npz_device_control.h"
#include "../nPZero_Driver/Inc/npz_hal.h"
#include "../nPZero_Driver/Inc/npz_logs.h"
#include "../nPZero_Driver/Inc/npz_registers.h"

#define _XTAL_FREQ 8000000UL // Example: For a 16MHz clock

#define TICK_PER_MS (_XTAL_FREQ/2/1000)


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

void __delay_ms(uint32_t ms);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MAIN_H */

/* *****************************************************************************
 End of File
 */
