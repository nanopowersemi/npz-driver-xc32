/**
 * @file npz_hal.c
 *
 * @brief Source file for npz Hardware Abstraction Layer (HAL)
 *
 * This file contains the hardware-specific functions and configurations for
 * the I2C communication interface. Users are required to customize these
 * functions according to their target hardware and MCU setup.
 *
 * The functions in this file serve as an abstraction layer between the 
 * driver and the hardware-specific I2C implementation on the target system.
 * Users should modify these functions to match the I2C peripheral and
 * communication protocol used in their system.
 *
 * @note This file provides placeholder functions and configurations that need
 * to be adapted to the specific hardware and MCU setup. Users must replace
 * these placeholders with the actual implementations relevant to their system.
 *
 * @warning Incorrect configuration or improper implementation of these functions
 * may result in I2C communication failures or unexpected behavior.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "../../nPZero_xc32.X/main.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

#define I2C_DELAY_MS 1

/*****************************************************************************
 * Data
 *****************************************************************************/

/*****************************************************************************
 * Private Methods
 *****************************************************************************/

/*****************************************************************************
 * Public Methods
 *****************************************************************************/

/**
 * @brief Function to read registers over I2C.
 */
npz_status_e npz_hal_read(uint8_t slave_address, uint8_t slave_register, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    I2C1_WriteRead(slave_address, slave_register, pData, size);
    
	return OK;
}

/**
 * @brief Function to write to registers over I2C.
 */
npz_status_e npz_hal_write(uint8_t slave_address, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    I2C1_Write(slave_address, pData, size);
    
	return OK;
}

/**
 * @brief Function to initialize I2C instance that will communicate with npz.
 */
npz_status_e npz_hal_init() 
{

    I2C1_Initialize();

	return OK;
}
