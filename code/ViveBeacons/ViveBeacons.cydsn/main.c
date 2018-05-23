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
    Units used in all the firmware :
    - distances, coordinates : m
    - angles : rad
    - time : s
    - velocity : m/s
    - acceleration : m/(s^2)
    - temperature : °C
*/

#include <stdlib.h>
#include <stdio.h>
#include "position.h"
#include "main.h"

// Handlers
UART_commands_manager* uart_commands_manager = NULL;
VIVE_sensors_data* vive_sensors_data = NULL;
VIVE_sensors* vive_sensors = NULL;
Position_finder* position_finder = NULL;

int main(void) {    
    // Constructors
    vive_sensors = VIVE_sensors_create();
    position_finder = Position_finder_create();
    uart_commands_manager = UART_commands_manager_create();
    
    CyGlobalIntEnable;
    
    // Initialization
    UART_commands_manager_init(uart_commands_manager);
    UART_commands_manager_register_command(uart_commands_manager, "SBPOS", callback_set_beacon_position);
    UART_commands_manager_register_command(uart_commands_manager, "AYR", callback_are_you_ready);
    UART_commands_manager_register_command(uart_commands_manager, "GPOS", callback_send_position);
    
    VIVE_sensors_init(vive_sensors);
    while(beacon_position_initialized == false) // Wait until beacon position is initialized
        UART_commands_manager_check_commands(uart_commands_manager);
        
    Position_finder_init(position_finder, &beacon_position, LED_COORD_HEIGHT, tracker_led_offset);
    
    configured = true;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;) {
        UART_commands_manager_check_commands(uart_commands_manager);
        
        if(configured) {
            if(VIVE_pulses_decoded) {
                VIVE_pulses_decoded = 0;
                vive_sensors_data = VIVE_sensors_process_pulses(vive_sensors);
                Position_finder_find_position(position_finder, vive_sensors_data);
            }
        }
    }
}

void callback_set_beacon_position() {
    // Beacon position
    beacon_position.x = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    beacon_position.y = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    beacon_position.z = atof(UART_commands_manager_get_next_token(uart_commands_manager));

    beacon_position.alpha = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    beacon_position.beta = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    beacon_position.gamma = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    
    beacon_position_initialized = true;
}

// callback functions
void callback_are_you_ready() {
    if(configured)
        UART_commands_manager_send_command(uart_commands_manager, "READY", "");
}

void callback_send_position() {
    // Send back the current position
    bool is_valid = false;
    char buffer[192];
    Position2D* position = position_finder->current_position;
    
    float x = (float) position->x;
    float y = (float) position->y;
    float a = (float) position->a;
    
    sprintf(buffer, "%d %.6f %.6f %.6f", is_valid, x, y, a);
    
    if(configured == true) // If not configured, don't send the position
        UART_commands_manager_send_command(uart_commands_manager, "POS", buffer);
}
/* [] END OF FILE */
