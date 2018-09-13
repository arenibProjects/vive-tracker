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
    - distances, coordinates : mm or m
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

#define USB_UART_jumper jumper_pins_7

#define ISCONNECTED(pin) if(CyPins_ReadPin(pin) == 0)
#define ISDISCONNECTED(pin) if(CyPins_ReadPin(pin) == 1)

// Handler "objects" pointers (We are in C ...)
UART_commands_manager* uart_commands_manager = NULL;
USB_commands_manager* usb_commands_manager = NULL;
VIVE_sensors_data* vive_sensors_data = NULL;
VIVE_sensors* vive_sensors = NULL;
Position_finder* position_finder = NULL;

int main(void) {    
    // Creating handler objects
    vive_sensors = VIVE_sensors_create();
    position_finder = Position_finder_create();
    ISCONNECTED(USB_UART_jumper)
        usb_commands_manager = USB_commands_manager_create();
    ISDISCONNECTED(USB_UART_jumper)
        uart_commands_manager = UART_commands_manager_create();
    
    CyGlobalIntEnable;
    
    /* --- Tracker initialization --- */
    ISCONNECTED(USB_UART_jumper) {
        // USB init
        USB_commands_manager_init(usb_commands_manager);
        
        // USB register commands
        USB_commands_manager_register_command(usb_commands_manager, "SBPOS", callback_set_beacon_position);
        USB_commands_manager_register_command(usb_commands_manager, "AYR", callback_are_you_ready);
        USB_commands_manager_register_command(usb_commands_manager, "GPOS", callback_send_position);
    }
    
    ISDISCONNECTED(USB_UART_jumper) {
        // UART init
        UART_commands_manager_init(uart_commands_manager);
        
        // UART register commands
        UART_commands_manager_register_command(uart_commands_manager, "SBPOS", callback_set_beacon_position);
        UART_commands_manager_register_command(uart_commands_manager, "AYR", callback_are_you_ready);
        UART_commands_manager_register_command(uart_commands_manager, "GPOS", callback_send_position);
    }
    
    /* VIVE sensors init : VIVE decoders and TS4231 init (It has to wait for light though).
    /!\ Because the tracker is waiting for light on every TS4231, the
    firmware can be stuck there */
    VIVE_sensors_init(vive_sensors);
    
    while(beacon_position_initialized == false) { // Wait until beacon position is initialized
        ISCONNECTED(USB_UART_jumper)
            USB_commands_manager_check_commands(usb_commands_manager);
        
        ISDISCONNECTED(USB_UART_jumper) 
            UART_commands_manager_check_commands(uart_commands_manager);
    }
    
    // Position finder init
    Position_finder_init(position_finder, &beacon_position, LED_COORD_HEIGHT, tracker_led_offset);
    
    configured = true; // Yay, it's configured !
    
    /* --- Tracker initialized --- */

    /* --- Main loop --- */
    for(;;) {
        ISCONNECTED(USB_UART_jumper)
            USB_commands_manager_check_commands(usb_commands_manager);
        
        ISDISCONNECTED(USB_UART_jumper) 
            UART_commands_manager_check_commands(uart_commands_manager);
        
        if(configured) {
            if(VIVE_pulses_decoded) { // If configured and new pulse received, go process it !
                VIVE_pulses_decoded = 0; // Disarm the flag
                vive_sensors_data = VIVE_sensors_process_pulses(vive_sensors); // Get the angles
                Position_finder_find_position(position_finder, vive_sensors_data); // Compute the position
            }
        }
    }
}

/*
    ---------------------------------------------------------------------------
                                    Callbacks
    ---------------------------------------------------------------------------
*/

/*
    ---------------------------------------------------------------------------
    Name : Set beacon position
    Description : Allow the main system to give the beacon position to the vive
    tracker. This is mandatory because the lighthouse beacon position vary with
    the side. This allows the user to replace a dead tracker without having to
    reconfigure it. It will be done automatically by the main system.
    ---------------------------------------------------------------------------
*/

void callback_set_beacon_position() {
    ISCONNECTED(USB_UART_jumper) {
        beacon_position.x = atof(USB_commands_manager_get_next_token(usb_commands_manager));
        beacon_position.y = atof(USB_commands_manager_get_next_token(usb_commands_manager));
        beacon_position.z = atof(USB_commands_manager_get_next_token(usb_commands_manager));

        beacon_position.alpha = atof(USB_commands_manager_get_next_token(usb_commands_manager));
        beacon_position.beta = atof(USB_commands_manager_get_next_token(usb_commands_manager));
        beacon_position.gamma = atof(USB_commands_manager_get_next_token(usb_commands_manager));
    }
    
    ISDISCONNECTED(USB_UART_jumper) {
        beacon_position.x = atof(UART_commands_manager_get_next_token(uart_commands_manager));
        beacon_position.y = atof(UART_commands_manager_get_next_token(uart_commands_manager));
        beacon_position.z = atof(UART_commands_manager_get_next_token(uart_commands_manager));

        beacon_position.alpha = atof(UART_commands_manager_get_next_token(uart_commands_manager));
        beacon_position.beta = atof(UART_commands_manager_get_next_token(uart_commands_manager));
        beacon_position.gamma = atof(UART_commands_manager_get_next_token(uart_commands_manager));
    }
    
    beacon_position_initialized = true;
}

/*
    ---------------------------------------------------------------------------
    Name : Are you ready
    Description : Mandatory function. With this, the main system is able to not
    get stuck waiting for a non-ready device to respond.
    ---------------------------------------------------------------------------
*/

void callback_are_you_ready() {
    if(configured) {
        ISCONNECTED(USB_UART_jumper)
            USB_commands_manager_send_command(usb_commands_manager, "READY", "");
        
        ISDISCONNECTED(USB_UART_jumper) 
            UART_commands_manager_send_command(uart_commands_manager, "READY", "");
    }
}

/*
    ---------------------------------------------------------------------------
    Name : Send position
    Description : Send the last computed position to the main system when
    asked. This avoid the main system to be spammed by the tracker.
    ---------------------------------------------------------------------------
*/

void callback_send_position() {
    bool is_valid = false;
    char buffer[192];
    
    // Unpack the Position2D structure
    Position2D* position = position_finder->current_position;
    float x = (float) position->x;
    float y = (float) position->y;
    float a = (float) position->a;
    
    // Serialize
    sprintf(buffer, "%d %.6f %.6f %.6f", is_valid, x, y, a);
    
    if(configured == true) { // Send the position only if the tracker is configured
        ISCONNECTED(USB_UART_jumper)
            USB_commands_manager_send_command(usb_commands_manager, "POS", buffer);
        
        ISDISCONNECTED(USB_UART_jumper) 
            UART_commands_manager_send_command(uart_commands_manager, "POS", buffer);
    }
}
/* [] END OF FILE */
