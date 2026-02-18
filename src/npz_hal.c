/**
 * @file npz_hal.c
 *
 * @brief Source file for the nPZero Hardware Abstraction Layer (HAL)
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

#include "../nPZero_xc32.X/main.h"

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
#include <xc.h>

npz_status_e npz_hal_read(uint8_t slave_address, uint8_t slave_register, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    // 1. Iniciar Condição de START
    I2C1CONbits.SEN = 1; 
    while(I2C1CONbits.SEN); // Espera o hardware limpar o bit (fim do Start)

    // 2. Enviar Endereço do Escravo (Escrita)
    I2C1TRN = (slave_address << 1); 
    while(I2C1STATbits.TRSTAT); // Espera transmissão terminar
    if(I2C1STATbits.ACKSTAT) return ERR; // Se NACK, sai

    // 3. Enviar Endereço do Registo que queres ler
    I2C1TRN = slave_register;
    while(I2C1STATbits.TRSTAT);
    if(I2C1STATbits.ACKSTAT) return ERR;

    // 4. Repeated START para mudar para modo de LEITURA
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN);

    // 5. Enviar Endereço do Escravo (Leitura)
    I2C1TRN = (slave_address << 1) | 0x01;
    while(I2C1STATbits.TRSTAT);
    if(I2C1STATbits.ACKSTAT) return ERR;

    // 6. Ciclo de Leitura
    for(uint16_t i = 0; i < size; i++) {
        I2C1CONbits.RCEN = 1; // Ativa modo de receção
        while(!I2C1STATbits.RBF); // Espera buffer de receção estar cheio
        pData[i] = I2C1RCV; // Lê o dado

        if(i < (size - 1)) {
            I2C1CONbits.ACKDT = 0; // Prepara ACK
        } else {
            I2C1CONbits.ACKDT = 1; // Prepara NACK para o último byte
        }
        I2C1CONbits.ACKEN = 1; // Envia o (N)ACK
        while(I2C1CONbits.ACKEN);
    }

    // 7. Condição de STOP
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);

    return OK;
}

}

/**
 * @brief Function to write to registers over I2C.
 */
npz_status_e npz_hal_write(uint8_t slave_address, uint8_t *pData, uint16_t size, uint32_t timeout)
{
/**
 * @brief Function to write to registers over I2C using XC32 Bare Metal.
 */
npz_status_e npz_hal_write(uint8_t slave_address, uint8_t *pData, uint16_t size, uint32_t timeout)
{
    // 1. Iniciar Condição de START
    I2C1CONbits.SEN = 1; 
    while(I2C1CONbits.SEN); // Espera o hardware completar o Start

    // 2. Enviar Endereço do Escravo com bit de Escrita (W = 0)
    I2C1TRN = (slave_address << 1); 
    while(I2C1STATbits.TRSTAT); // Espera fim da transmissão
    
    // Verifica se o escravo respondeu com ACK
    if(I2C1STATbits.ACKSTAT) {
        I2C1CONbits.PEN = 1; // Envia STOP para libertar o barramento
        return ERR; 
    }

    // 3. Enviar o Buffer de Dados (incluindo o registo, se estiver no pData)
    for(uint16_t i = 0; i < size; i++) {
        I2C1TRN = pData[i]; // Carrega o dado no registo de transmissão
        
        while(I2C1STATbits.TRSTAT); // Espera transmissão terminar
        
        if(I2C1STATbits.ACKSTAT) { // Se o escravo não der ACK a meio do buffer
            I2C1CONbits.PEN = 1;
            return ERR;
        }
    }

    // 4. Condição de STOP
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);

    return OK;
}

}

/**
 * @brief Function to initialize the I2C instance that will communicate with the nPZero.
 */
npz_status_e npz_hal_init() 
{
	I2C1_Initialize();

	return OK;
}
