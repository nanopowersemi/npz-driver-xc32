/**
 * @file npz_hal.h
 * 
 * @brief Header file for npz Hardware Abstraction Layer (HAL)
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
 * to be adapted to the specific hardware and MCU setup. The user is expected to modify
 * the function declaration in the npz_hal.c to fit their setup.
 *
 * @warning Incorrect configuration or improper implementation of these functions
 * may result in I2C communication failures or unexpected behavior.
 */

#ifndef __NPZ_HAL_H
#define __NPZ_HAL_H

/** @cond */
#include "../../nPZero_xc32.X/main.h"

/** @endcond */

#define NPZ_I2C_ADDRESS			0x7a  // 0x3D npz I2c address shifted left by 1 bit
#define I2C_TRANSMISSION_TIMEOUT_MS 1300

/** Enumerations. */


/**
 * @brief Function to read from registers over I2C.
 *
 * @note The device 7 bits address value in datasheet must be shifted to the left before calling the interface.
 * @note Function is blocking.
 * @param [in] slave_address I2C Address for slave.
 * @param [out] pData Pointer to data buffer where read data will be stored.
 * @param [in] size Size of data to be received.
 * @param [in] timeout Timeout before transmission is asserted.
 * @return npz_status_e Status
 */
npz_status_e npz_hal_read(uint8_t slave_address, uint8_t slave_register,
		uint8_t *pData, uint16_t size, uint32_t timeout);

/**
 * @brief Function to write to registers over I2C.
 *
 * @note The device 7 bits address value in datasheet must be shifted to the left before calling the interface.
 * @note Function is blocking.
 * @param [in] slave_address I2C Address for slave.
 * @param [in] pData Pointer to data buffer to write.
 * @param [in] size Size of data buffer to be sent.
 * @param [in] timeout Timeout before transmission is asserted.
 * @return npz_status_e Status
 */
npz_status_e npz_hal_write(uint8_t slave_address, uint8_t *pData, uint16_t size,
		uint32_t timeout);

/**
 * @brief Function to initialize hardware dependent I2C interface.
 *
 * @return npz_status_e Status
 */
npz_status_e npz_hal_init(void);

#endif /* __NPZ_HAL_H */
