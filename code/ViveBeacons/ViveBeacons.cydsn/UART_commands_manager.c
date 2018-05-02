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

/* [] END OF FILE */

#include <stdbool.h>
#include <stdlib.h>
#include "project.h"
#include "UART_commands_manager.h"

/*
    --------------
    Public methods
    --------------
*/

UART_commands_manager* UART_commands_manager_create() {
    UART_commands_manager* uart_commands_manager = (UART_commands_manager*) malloc(1*sizeof(UART_commands_manager));
    
    uart_commands_manager->callbacks = NULL;
    uart_commands_manager->last_token = NULL;
    uart_commands_manager->command_count = 0;
    
    strcpy(uart_commands_manager->delimiter, " ");
    
                // Clear buffer
            uart_commands_manager->buffer[0] = '\0';
            uart_commands_manager->buffer_pos = 0;
            
    return uart_commands_manager;
}

void UART_commands_manager_init(UART_commands_manager *uart_commands_manager) {
    UART_TEENSY_Start();
}
void UART_commands_manager_register_command(UART_commands_manager *uart_commands_manager, const char *command, void (*callback_function)()) {
    uart_commands_manager->callbacks = (UART_commands_manager_callback *) realloc(uart_commands_manager->callbacks, (uart_commands_manager->command_count + 1)*sizeof(UART_commands_manager_callback));
    strncpy((uart_commands_manager->callbacks[uart_commands_manager->command_count]).command, command, UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH);
    (uart_commands_manager->callbacks[uart_commands_manager->command_count]).callback_function = callback_function;
    uart_commands_manager->command_count++;
}

void UART_commands_manager_check_commands(UART_commands_manager *uart_commands_manager) {
    while(UART_TEENSY_GetRxBufferSize() > 0) {
        char character = UART_TEENSY_GetChar();
        if(character == 0)
            break;
        
        if(character == '\n') {
            char *command = strtok_r(uart_commands_manager->buffer, uart_commands_manager->delimiter, &(uart_commands_manager->last_token));
            
            if(command != NULL) {
                bool matched = false;
                
                for(int i = 0; i < uart_commands_manager->command_count; i++) { // For each command in registered commands
                    if (strncmp(command, (uart_commands_manager->callbacks[i]).command, UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH) == 0) {
                        (*((uart_commands_manager->callbacks[i]).callback_function))();
                        matched = true;
                        break;
                    }
                }
            }
            
            // Clear buffer
            uart_commands_manager->buffer[0] = '\0';
            uart_commands_manager->buffer_pos = 0;
        } else {
            if(uart_commands_manager->buffer_pos < UART_COMMAND_MANAGER_MAX_BUFFER_SIZE) {
                uart_commands_manager->buffer[uart_commands_manager->buffer_pos] = character;
                uart_commands_manager->buffer_pos++;
                uart_commands_manager->buffer[uart_commands_manager->buffer_pos] = '\0';
            }
        }
    }
}

char* UART_commands_manager_get_next_token(UART_commands_manager *uart_commands_manager) {
    return strtok_r(NULL, uart_commands_manager->delimiter, &(uart_commands_manager->last_token));
}

void UART_commands_manager_send_command(UART_commands_manager *uart_commands_manager, char command[UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1], char args[UART_COMMAND_MANAGER_MAX_BUFFER_SIZE - (UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]) {
    UART_TEENSY_PutString(command);
    UART_TEENSY_PutString(uart_commands_manager->delimiter);
    UART_TEENSY_PutString(args);
    UART_TEENSY_PutString("\n");
}