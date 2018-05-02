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

#if !defined(UART_COMMANDS_MANAGER_H)
#define UART_COMMANDS_MANAGER_H

// Defines
#define UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH 32
#define UART_COMMAND_MANAGER_MAX_BUFFER_SIZE 256

// Data structure
typedef struct UART_commands_manager_callback UART_commands_manager_callback;
struct UART_commands_manager_callback {
    char command[UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1];
    void (*callback_function)();
};

// Attributes
typedef struct UART_commands_manager UART_commands_manager;
struct UART_commands_manager {
    UART_commands_manager_callback *callbacks;
    char buffer[UART_COMMAND_MANAGER_MAX_BUFFER_SIZE];
    int command_count;
    int buffer_pos;
    char *last_token;
    char delimiter[2];
};

// Public methods
UART_commands_manager* UART_commands_manager_create();
void UART_commands_manager_init(UART_commands_manager *uart_commands_manager);
void UART_commands_manager_register_command(UART_commands_manager *uart_commands_manager, const char *command, void (*callback_function)());
void UART_commands_manager_send_command(UART_commands_manager *uart_commands_manager, char command[UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1], char args[UART_COMMAND_MANAGER_MAX_BUFFER_SIZE - (UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 2]);
void UART_commands_manager_check_commands(UART_commands_manager *uart_commands_manager);
char* UART_commands_manager_get_next_token(UART_commands_manager *uart_commands_manager);

#endif

/* [] END OF FILE */
