/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/*
    ---------------------------------------------------------------------------
    This file is a PSoC version of an Arduino library. It was trans-coded from
    https://github.com/TriadSemi/TS4231
    ---------------------------------------------------------------------------
*/

#include <stdlib.h>
#include "TS4231_driver.h"

/*
    --------------
    Public methods
    --------------
*/

TS4231_driver* TS4231_driver_create(uint32_t enveloppe_pin, uint32_t data_pin) {
    TS4231_driver* ts4231_driver = (TS4231_driver*) malloc(1*sizeof(TS4231_driver));
    
    ts4231_driver->enveloppe_pin = enveloppe_pin;
    ts4231_driver->data_pin = data_pin;
    ts4231_driver->state = S0; // Unconfigured
    
    return ts4231_driver;
}

void TS4231_driver_init(TS4231_driver* ts4231_driver) {
    Status status = UNKNOWN;
    bool exit = false;
    
    TS4231_driver_wait_for_light(ts4231_driver);
    
    do {
        status = TS4231_driver_configure(ts4231_driver, DEFAULT_CONFIGURATION_VALUE);
        
        if(status == UNKNOWN || status == BUS_FAIL)
            continue;
        else if(status == VERIFY_FAIL) {
            uint16_t readback = TS4231_driver_read_configuration(ts4231_driver);
            
            if(readback == DEFAULT_CONFIGURATION_VALUE)
                exit = true;
            else
                continue;
        }
        else if(status == WATCH_FAIL) {
            
        }
        else if(status == CONFIG_PASS)
            exit = true;
        else
            continue;
        
    }
    while(!exit);
    
    
}

bool TS4231_driver_go_to_sleep(TS4231_driver* ts4231_driver) {
    bool sleep_success;
    
    TS4231_driver_check_bus(ts4231_driver);
    switch (ts4231_driver->state) {
        case S0:
        sleep_success = false;
        break;
        
        case SLEEP:
        sleep_success = true;
        break;
        
        case WATCH:
        // Watch -> State try
        CyPins_ClearPin(ts4231_driver->enveloppe_pin);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
        CyDelayUs(BUS_DRIVE_DELAY);
        
        // Check
        TS4231_driver_check_bus(ts4231_driver);
        if (ts4231_driver->state == SLEEP)
            sleep_success = true;
        else
            sleep_success = false;
        break;
        
        case S3:
        sleep_success = false;
        break;
        
        default:
        sleep_success = false;
        break;
    }
    
    return sleep_success;
}

bool TS4231_driver_go_to_watch(TS4231_driver* ts4231_driver) {
    bool watch_success;
    
    TS4231_driver_check_bus(ts4231_driver);
    switch (ts4231_driver->state) {
        case S0:
        watch_success = false;
        break;
        
        case SLEEP:
        CyPins_SetPin(ts4231_driver->data_pin);
        CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
        CyPins_ClearPin(ts4231_driver->enveloppe_pin);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
        CyPins_ClearPin(ts4231_driver->data_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
        CyPins_SetPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
        CyDelayUs(SLEEP_RECOVERY);
        
        TS4231_driver_check_bus(ts4231_driver);
        if (ts4231_driver->state == WATCH)
            watch_success = true;
        else
            watch_success = false;
        break;
        
        case WATCH:
        watch_success = true;
        break;
        
        case S3:
        CyPins_SetPin(ts4231_driver->enveloppe_pin);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
        CyPins_SetPin(ts4231_driver->data_pin);
        CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
        CyPins_ClearPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_ClearPin(ts4231_driver->data_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
        CyPins_SetPin(ts4231_driver->enveloppe_pin);
        CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
        CyDelayUs(SLEEP_RECOVERY);
        
        TS4231_driver_check_bus(ts4231_driver);
        if (ts4231_driver->state == WATCH)
            watch_success = true;
        else
            watch_success = false;
        break;
        
        default:
        watch_success = false;
        break;
    }
    return watch_success;
}

void TS4231_driver_delete(TS4231_driver* ts4231_driver) {
    free(ts4231_driver);
}

/*
    ---------------
    Private methods
    ---------------
*/

/*
    Function that wait until light is detected on the TS4231. There's no timeout.
*/

void TS4231_driver_wait_for_light(TS4231_driver* ts4231_driver) {
    bool light = false;
    bool exit = false;
    unsigned long time0;
    
    if (TS4231_driver_check_bus(ts4231_driver) == S0) {
        while (exit == false) {
            if (CyPins_ReadPin(ts4231_driver->data_pin) != 0) {
                while (exit == false) {
                    if (CyPins_ReadPin(ts4231_driver->data_pin) == 0) {
                        exit = true;
                        light = true;
                    }
                }
            }
        }
    }
    else
        light = true;  //if not in state S0, light has already been detected
}

Status TS4231_driver_configure(TS4231_driver* ts4231_driver, uint16_t configuration_value) {
    Status configuration_success = UNKNOWN;
    uint16_t readback;
    
    ts4231_driver->state = S0;
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
    CyPins_ClearPin(ts4231_driver->data_pin);
    CyPins_ClearPin(ts4231_driver->enveloppe_pin);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
    if(TS4231_driver_check_bus(ts4231_driver) == S3) {
        TS4231_driver_write_configuration(ts4231_driver, configuration_value);
        readback = TS4231_driver_read_configuration(ts4231_driver);
        if(readback == configuration_value) {
            ts4231_driver->state = SLEEP;
            if(TS4231_driver_go_to_watch(ts4231_driver))
                configuration_success = CONFIG_PASS;
            else
                configuration_success = WATCH_FAIL;
        }
        else
            configuration_success = VERIFY_FAIL;
    }
    else
        configuration_success = BUS_FAIL;
    
    return configuration_success;
}

/*
    TS4231_driver_check_bus() performs a voting function where the bus is sampled 3 times to find 2 identical results.  This is necessary since light detection is asynchronous and can indicate a false state.
*/

State TS4231_driver_check_bus(TS4231_driver* ts4231_driver) {
    uint8_t state;
    uint8_t E_state;
    uint8_t D_state;
    uint8_t S0_count = 0;
    uint8_t SLEEP_count = 0;
    uint8_t WATCH_count = 0;
    uint8_t S3_count = 0;
    
    for (uint8_t i=0; i<3; i++) {
        E_state = CyPins_ReadPin(ts4231_driver->enveloppe_pin);
        D_state = CyPins_ReadPin(ts4231_driver->data_pin);
        
        if (D_state) {
            if (E_state)
                S3_count++;
            else
                SLEEP_count++;
        }
        else {
            if (E_state)
                WATCH_count++;
            else
                S0_count++;
        }
        
        CyDelayUs(BUS_CHECK_DELAY);
    }
    
    if (SLEEP_count >= 2)
        state = SLEEP;
    else if (WATCH_count >= 2)
        state = WATCH;
    else if (S3_count >= 2)
        state = S3;
    else if (S0_count >= 2)
        state = S0;
    else
        state = UNKNOWN;

    ts4231_driver->state = state;
    
    return state;
}

void TS4231_driver_write_configuration(TS4231_driver* ts4231_driver, uint16_t configuration_value) {
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    
    for (uint8_t i = 0; i < 15; i++) {
        configuration_value = configuration_value << 1;
        
        if ((configuration_value & 0x8000) != 0)
            CyPins_SetPin(ts4231_driver->data_pin);
        else
            CyPins_ClearPin(ts4231_driver->data_pin);
            
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_SetPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        CyPins_ClearPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
    }
    
    CyPins_ClearPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
}

uint16_t TS4231_driver_read_configuration(TS4231_driver* ts4231_driver) {
    uint16_t readback = 0x0000;
    
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_ClearPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    
    for (uint8_t i = 0; i < 14; i++) {
        CyPins_SetPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
        readback = (readback << 1) | (CyPins_ReadPin(ts4231_driver->data_pin) == 0 ? 0x0000 : 0x0001);
        CyPins_ClearPin(ts4231_driver->enveloppe_pin);
        CyDelayUs(BUS_DRIVE_DELAY);
    }
    
    CyPins_ClearPin(ts4231_driver->data_pin);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_STRONG);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->enveloppe_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPin(ts4231_driver->data_pin);
    CyDelayUs(BUS_DRIVE_DELAY);
    CyPins_SetPinDriveMode(ts4231_driver->enveloppe_pin, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(ts4231_driver->data_pin, CY_PINS_DM_DIG_HIZ);

    return readback;
}

/* [] END OF FILE */
