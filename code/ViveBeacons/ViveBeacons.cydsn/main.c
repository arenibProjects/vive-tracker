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
    
    CyGlobalIntEnable; /* Enable global interrupts. */ 
    /*
    USB_Serial_Start(0, USB_Serial_5V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
    
    USB_Serial_PutString("START\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    CyDelay(3000);
    
    USB_Serial_PutString("INIT\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);*/
    
    // Beacon position
    Position3D beacon_position;
    beacon_position.x = BEACON_COORD_X;
    beacon_position.y = BEACON_COORD_Y;
    beacon_position.z = BEACON_COORD_Z;
    
    beacon_position.alpha = BEACON_COORD_ALPHA;
    beacon_position.beta = BEACON_COORD_BETA;
    beacon_position.gamma = BEACON_COORD_GAMMA;
    
    // Initialization
    Position_finder_init(position_finder, &beacon_position, LED_COORD_HEIGHT, tracker_led_offset);
    VIVE_sensors_init(vive_sensors);
    UART_commands_manager_init(uart_commands_manager);
    
    UART_commands_manager_register_command(uart_commands_manager, "GETPOS", callback_get_position);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
/*
    USB_Serial_PutString("READY\n");
    while(USB_Serial_CDCIsReady() == 0u);*/

    for(;;) {
        UART_commands_manager_check_commands(uart_commands_manager);
        
        if(VIVE_pulses_decoded) {
            VIVE_pulses_decoded = 0;
            vive_sensors_data = VIVE_sensors_process_pulses(vive_sensors);
            Position_finder_find_position(position_finder, vive_sensors_data);
            
            /*
            char buffer[512];
            sprintf(buffer, "HEADING %f\n", position_finder->current_position.a);
            USB_Serial_PutString(buffer);
            while(USB_Serial_CDCIsReady() == 0u);*/
        }
    }
}

void callback_get_position() {
    // Send back the current position
    char buffer[192];
    Position2D* position = position_finder->current_position;
    float x = (float) position->x;
    float y = (float) position->y;
    float a = (float) position->a;
    sprintf(buffer, "%.6f %.6f %.6f", x, y, a);
    UART_commands_manager_send_command(uart_commands_manager, "POS", buffer);
}

/* [] END OF FILE */
