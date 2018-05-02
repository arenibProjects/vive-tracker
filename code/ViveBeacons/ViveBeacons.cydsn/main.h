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

#if !defined(MAIN_H)
#define MAIN_H

#include "project.h"
#include "VIVE_sensors.h"
#include "TS4231_driver.h"
#include "Position_finder.h"
#include "configuration.h"
#include "UART_commands_manager.h"

// Position of LEDs in tracker's coordinates (aka relative to its center)
double tracker_led_offset[8][2] = {
    {40, 0},
    {40*SQRT_2, 40*SQRT_2},
    {0, 40},
    {-40*SQRT_2, 40*SQRT_2},
    {-40, 0},
    {-40*SQRT_2, -40*SQRT_2},
    {0, -40},
    {40*SQRT_2, -40*SQRT_2},
};

// Handlers
extern UART_commands_manager* uart_commands_manager;
extern VIVE_sensors_data* vive_sensors_data;
extern VIVE_sensors* vive_sensors;
extern Position_finder* position_finder;

// Callback functions
void callback_get_position();

#endif

/* [] END OF FILE */
