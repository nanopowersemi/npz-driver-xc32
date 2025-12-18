/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include "../nPZero_xc32.X/main.h"


void __delay_ms(uint32_t ms)
{
    uint32_t Start=_CP0_GET_COUNT();
    uint32_t Duration=TICK_PER_MS*ms;
    
    while((_CP0_GET_COUNT()-Start)<Duration);
}

npz_peripheral_config_s peripheral_3 = {
    .power_mode = POWER_MODE_PERIODIC,
    .communication_protocol = COM_SPI,
    .polling_mode = POLLING_MODE_PERIODIC_READ_COMPARE_THRESHOLD,
    .power_switch_mode = POWER_SWITCH_MODE_LOGIC_OUTPUT_HIGH,
    .interrupt_pin_mode = INTERRUPT_PIN_MODE_INPUT_ACTIVE_HIGH,
    .comparison_mode = COMPARISON_MODE_INSIDE_THRESHOLD,
    .sensor_data_type = DATA_TYPE_INT16,
    .multi_byte_transfer_enable = 0,
    .swap_registers = 0,
    .spi_cfg.bytes_from_sram_num = 2,
    .spi_cfg.bytes_from_sram = {0x20, 0x10},
    .spi_cfg.bytes_from_sram_read_num = 1,
    .spi_cfg.bytes_from_sram_read = {0xA8},
    .spi_cfg.mode = SPIMOD_SPI_MODE_0,
    .polling_period = 50,
    .pre_wait_time = PRE_WAIT_TIME_EXTEND_256,
    .post_wait_time = POST_WAIT_TIME_EXTEND_256,
    .time_to_wait = 10,
    .threshold_over = 1000,
    .threshold_under = 64536,
};

npz_peripheral_config_s peripheral_4 = {
    .communication_protocol = COM_I2C,
    .power_mode = POWER_MODE_PERIODIC,
    .polling_mode = POLLING_MODE_PERIODIC_READ_COMPARE_THRESHOLD,
    .power_switch_mode = POWER_SWITCH_MODE_LOGIC_OUTPUT_HIGH,
    .interrupt_pin_mode = INTERRUPT_PIN_MODE_INPUT_ACTIVE_HIGH,
    .comparison_mode = COMPARISON_MODE_INSIDE_THRESHOLD,
    .sensor_data_type  = DATA_TYPE_INT16,
    .multi_byte_transfer_enable  = MULTIBYTE_TRANSFER_ENABLE,
    .swap_registers = ENDIAN_BIG,
    .polling_period = 0x012C, // Wakeup peripheral every 30 seconds with 10Hz clock
    .i2c_cfg.sensor_address = 0x49,
    .i2c_cfg.command_num = 2,
    .i2c_cfg.bytes_from_sram = {0x01, 0x82, 0x02, 0xA0},
    .i2c_cfg.reg_address_value = 0x00,
    .i2c_cfg.wake_on_nak = ENABLED,
    .i2c_cfg.num_of_retries_on_nak = 3,
    .time_to_wait = 0x31, /* 0x31 (49 in decimal):
                           * When multiplied by 4096: 49 * 4096 = 200704  clock cycles * 2.5?s (1 / 400000),
                           * which equals 501.76ms at 400kHz.
                           * When multiplied by 256: 49 * 256 = 12544  clock cycles * 2.5?s (1 / 400000),
                           * which equals 31.36ms at 400kHz.
                           */
    .pre_wait_time = POST_WAIT_TIME_EXTEND_256,
    .post_wait_time = POST_WAIT_TIME_EXTEND_256,
    .threshold_over = 3200,
    .threshold_under = 1280,
};

npz_adc_config_channels_s npz_adc_internal_config = {
    .wakeup_enable = 0,
    .over_threshold = 0x2B,
    .under_threshold = 0x28,
};

npz_adc_config_channels_s npz_adc_external_config = {
    .wakeup_enable = 0,
    .over_threshold = 0x2D,
    .under_threshold = 0x26,
};

npz_device_config_s npz_configuration = {
    .host_power_mode = HOST_POWER_MODE_LOGIC_OUTPUT,
    .power_switch_normal_mode_per1 = 0,
    .power_switch_normal_mode_per2 = 0,
    .power_switch_normal_mode_per3 = 1,
    .power_switch_normal_mode_per4 = 1,
    .power_switch_gate_boost = 0,
    .system_clock_divider = SCLK_DIV_DISABLE,
    .system_clock_source = SYS_CLOCK_10HZ,
    .io_strength = IO_STR_NORMAL,
    .i2c_pull_mode = I2C_PULL_AUTO,
    .spi_auto = SPI_PINS_ALWAYS_ON,
    .xo_clock_out_sel = XO_CLK_OFF,
    .wake_up_per1 = 0,
    .wake_up_per2 = 0,
    .wake_up_per3 = 1,
    .wake_up_per4 = 1,
    .wake_up_any_or_all = WAKEUP_ANY,
    .global_timeout = 0x0BB8,
    .interrupt_pin_pull_up_pin1 = INT_PIN_PULL_HIGH,
    .interrupt_pin_pull_up_pin2 = INT_PIN_PULL_HIGH,
    .interrupt_pin_pull_up_pin3 = INT_PIN_PULL_HIGH,
    .interrupt_pin_pull_up_pin4 = INT_PIN_PULL_HIGH,
    .adc_ext_sampling_enable = 0,
    .adc_clock_sel = ADC_CLK_256,
    .adc_channels = {0, 0},
    .peripherals = {0, 0, &peripheral_3, &peripheral_4},
};

static void read_peripheral_acc(int peripheral_value)
{
    // Cast the acceleration value to INT16 (signed), and convert it to mg
    float acceleration_x = (float)(int16_t)peripheral_value * 2.0f * 1000.0f / 32768.0f;
    printf("Acceleration X axis: %.2f mg\r\n", acceleration_x);
}

static void read_peripheral_temp(int peripheral_value)
{
    // Calculate the temperature in degrees Celsius
    float temperature = peripheral_value * 0.0078125f;  // AS6212 temperature sensor resolution is 0.0078125°C.
    printf("Calculated temperature: %.3f °C\r\n", temperature);
}

static void npz_read_status_registers(npz_status_s *status)
{
    // Handle status2
    // Read the first status register
    if (npz_read_STA1(&status->status1) != OK)
    {
        return;
    }

    // Handle status1
    if (status->status1.reset_source == RESETSOURCE_NONE)
    {
        printf("Reset source is None\r\n");
    }
    else if (status->status1.reset_source == RESETSOURCE_PWR_RESET)
    {
        printf("Power-on reset triggered\r\n");
    }
    else if (status->status1.reset_source == RESETSOURCE_SOFT_RESET)
    {
        printf("Soft reset triggered (via I2C command)\r\n");
    }
    else if (status->status1.reset_source == RESETSOURCE_EXT_RESET)
    {
        printf("External reset triggered (via RST pin)\r\n");
    }

    if (status->status1.ext_adc_triggered == 1)
    {
        if (!npz_device_handle_adc_external())
        {
            return;
        }
    }

    if (status->status1.int_adc_triggered == 1)
    {
        if (!npz_device_handle_adc_internal())
        {
            return;
        }
    }

    if (status->status1.global_timeout_triggered == 1)
    {
        printf("Global Timeout triggered before any wake up source triggered\r\n");
    }

    // Read the second status register
    if (npz_read_STA2(&status->status2) != OK)
    {
        return;
    }

    // Handle status2
    // Arrays to map peripherals and switches
    npz_psw_e switches[4] = {PSW_LP1, PSW_LP2, PSW_LP3, PSW_LP4};
    uint8_t triggered[4] = {status->status2.per1_triggered, status->status2.per2_triggered,
                            status->status2.per3_triggered, status->status2.per4_triggered};
    uint8_t timeouts[4] = {status->status2.per1_global_timeout, status->status2.per2_global_timeout,
                           status->status2.per3_global_timeout, status->status2.per4_global_timeout};

    // Iterate over each peripheral to check for triggers and timeouts
    for (int i = 0; i < 4; i++)
    {
       if (triggered[i]) // Check if peripheral is triggered
       {
            int peripheral_value = 0;

            npz_device_read_peripheral_value(switches[i], i,
                                             &peripheral_value); // Read the peripheral value based on the switch

            if (npz_configuration.peripherals[i]->communication_protocol == COM_SPI && i == 2)
            {
                read_peripheral_acc(peripheral_value);
            }
            else if (npz_configuration.peripherals[i]->communication_protocol == COM_I2C &&
                (npz_configuration.peripherals[i]->polling_mode == POLLING_MODE_PERIODIC_READ_COMPARE_THRESHOLD ||
                 npz_configuration.peripherals[i]->polling_mode ==
                     POLLING_MODE_PERIODIC_WAIT_INTERRUPT_COMPARE_THRESHOLD))
            {
                read_peripheral_temp(peripheral_value);
            }
        }

        if (timeouts[i]) // Check if global timeout is triggered for this peripheral
        {
            printf("Peripheral %d global timeout was triggered\r\n", i + 1); // Log the timeout event
        }
    }
}

/**@brief Function for detecting the nPZero on the I2C bus
 */
uint8_t npz_search(void)
{
    uint8_t sample_data;

    npz_hal_read(NPZ_I2C_ADDRESS, REG_ID, &sample_data, 1, 5);

    if ((sample_data) == 0x60)
    {
    	printf("[--- nPZero Init OK ---]\r\n");
    	return 1;
    }
    else
    {
    	printf("[--- nPZero Init Not OK 0x%x---]\r\n", sample_data);
    	return 0;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("nPZero-Gen1 PIC32MX TEST ");
    
        // Initialize the npz interface
    npz_hal_init();

    __delay_ms(1);

    // Read the status registers of the npz device after every reset
    npz_status_s npz_status = { 0 };

    npz_read_status_registers(&npz_status);

    npz_search();

    // Send the configuration to the device
    npz_device_configure(&npz_configuration);

    // Logs and reads all configuration registers for debugging purposes
    npz_log_configurations(&npz_configuration);

    // Add a delay in main, to give the user time to flash the MCU before it enters sleep
    // This delay should be removed in production code
    __delay_ms(1);

    // At the end of your operations, put the device into sleep mode
    npz_device_go_to_sleep();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

