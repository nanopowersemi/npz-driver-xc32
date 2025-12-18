# nPZero Pic32 Host
<img src="https://dev.nanopowersemi.com/npz.png" alt="nPZero" width="300">

This repository provides demonstrations on how to use the **nPZero Driver API** with the Pic32 microcontroller platform MPLAB X and XC32 Compiler, focusing on setting up code examples for peripherals and the ADC channels.

## API Driver Overview
The **nPZero Driver** provides a comprehensive API for interfacing with the nPZero IC. It can be used on any hardware platform to interact with the IC, offering a register-level interface that allows users to efficiently control and manage power-related functionalities in their applications.
The driver also includes a Hardware Abstraction Layer (HAL) for I2C operations, which can be customized to match specific hardware configurations, making it adaptable across various platforms.

## Executing Code Examples with the nPZero Driver
This project utilizes the **nPZero Driver** to execute the following functionalities:

- The Peripheral Mode examples demonstrate communication using I2C:
    - These examples show how to communicate with the temperature sensor connected via the I2C interface, which is bundled with the
      nPZero G1 Development Kit (DevKit).

- The ADC examples illustrates how to configure the internal and external ADC channels.

These examples demonstrate the necessary procedures for initialization, configuration, and data exchange required for sensor interaction using the **nPZero Driver**.


## Using the nPZero Configurator
This project is compatible with the nPZero Configurator, allowing the configuration to be established directly in the tool:
1) Configure the device as required in the nPZero Configurator
2) Ensure the *Default* template is selected in the *Generated Code* section
3) Click on Save As in the *Generated code* section, and replace the *main.c* file in the *src* folder of this project
   (When making subsequent changes to the configuration, it is sufficient to press *Save* in the *Generated Code* section and the *main.c* file will be updated automatically)
4) Build and flash the code in VSCode as normal

