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

#if !defined(USB_COMMANDS_MANAGER_H)
#define USB_COMMANDS_MANAGER_H

// Defines
#define USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH 8
#define USB_COMMAND_MANAGER_MAX_BUFFER_SIZE 8

// Data structure
typedef struct USB_commands_manager_callback USB_commands_manager_callback;
struct USB_commands_manager_callback {
    char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1];
    void (*callback_function)();
};

// Attributes
typedef struct USB_commands_manager USB_commands_manager;
struct USB_commands_manager {
    USB_commands_manager_callback *callbacks;
    char buffer[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE];
    int command_count;
    int buffer_pos;
    char *last_token;
    char delimiter[2];
};

// Public methods
USB_commands_manager* USB_commands_manager_create();
void USB_commands_manager_init(USB_commands_manager *usb_commands_manager);
void USB_commands_manager_register_command(USB_commands_manager *usb_commands_manager, const char *command, void (*callback_function)());
void USB_commands_manager_check_commands(USB_commands_manager *usb_commands_manager);
char* USB_commands_manager_get_next_token(USB_commands_manager *usb_commands_manager);
void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1], char args[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE - (USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]);

#endif

/* [] END OF FILE */
