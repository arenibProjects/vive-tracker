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

#include <stdlib.h>
#include <stdbool.h>
#include "project.h"
#include "USB_commands_manager.h"

/*
    ---------------------------------------------------------------------------
    This file is a PSoC version of an Arduino library. It was trans-coded from
    https://github.com/kroimon/Arduino-SerialCommand
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

USB_commands_manager* USB_commands_manager_create() {
    USB_commands_manager* usb_commands_manager = (USB_commands_manager*) malloc(1*sizeof(USB_commands_manager));
    
    usb_commands_manager->callbacks = NULL;
    usb_commands_manager->last_token = NULL;
    usb_commands_manager->command_count = 0;
    
    strcpy(usb_commands_manager->delimiter, " ");
    
    // Clear buffer
    usb_commands_manager->buffer[0] = '\0';
    usb_commands_manager->buffer_pos = 0;
     
    return usb_commands_manager;
}

void USB_commands_manager_init(USB_commands_manager *usb_commands_manager) {
    USB_Serial_Start(0, USB_Serial_3V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
}

void USB_commands_manager_register_command(USB_commands_manager *usb_commands_manager, const char *command, void (*callback_function)()) {
    usb_commands_manager->callbacks = (USB_commands_manager_callback *) realloc(usb_commands_manager->callbacks, (usb_commands_manager->command_count + 1)*sizeof(USB_commands_manager_callback));
    strncpy((usb_commands_manager->callbacks[usb_commands_manager->command_count]).command, command, USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH);
    (usb_commands_manager->callbacks[usb_commands_manager->command_count]).callback_function = callback_function;
    usb_commands_manager->command_count++;
}

void USB_commands_manager_check_commands(USB_commands_manager *usb_commands_manager) {
    char nb_char = 0;
    
    // If we received some packets from the PC
    if(USB_Serial_GetCount() != 0) {
        nb_char = USB_Serial_GetCount();
        
        if((usb_commands_manager->buffer_pos + nb_char) < USB_COMMAND_MANAGER_MAX_BUFFER_SIZE) {
            nb_char = USB_Serial_GetData((uint8 *) (usb_commands_manager->buffer + usb_commands_manager->buffer_pos), nb_char); // Copy the received data into the buffer and update the nb of char read.
            usb_commands_manager->buffer_pos += nb_char + 1;
            usb_commands_manager->buffer[usb_commands_manager->buffer_pos] = '\0';
        }
    }
    
    // Processing the data
    for(int i = usb_commands_manager->buffer_pos - nb_char; i < usb_commands_manager->buffer_pos; i++)
    {
        char character = USB_Serial_GetChar();
        if(character == 0)
            return;
        
        if(character == '\n') {
            char *command = strtok_r(usb_commands_manager->buffer, usb_commands_manager->delimiter, &(usb_commands_manager->last_token));
            
            if(command != NULL) {
                bool matched = false;
                
                for(int i = 0; i < usb_commands_manager->command_count; i++) { // For each command in registered commands
                    if (strncmp(command, (usb_commands_manager->callbacks[i]).command, USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH) == 0) {
                        (*((usb_commands_manager->callbacks[i]).callback_function))();
                        matched = true;
                        break;
                    }
                }
            }
            
            // Clear buffer
            usb_commands_manager->buffer[0] = '\0';
            usb_commands_manager->buffer_pos = 0;
        }
    }
}

char* USB_commands_manager_get_next_token(USB_commands_manager *usb_commands_manager) {
    return strtok_r(NULL, usb_commands_manager->delimiter, &(usb_commands_manager->last_token));
}

void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1], char args[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE - (USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]) {
    USB_Serial_PutString(command);
    USB_Serial_PutString(usb_commands_manager->delimiter);
    USB_Serial_PutString(args);
    USB_Serial_PutString("\n");
}

/* [] END OF FILE */
