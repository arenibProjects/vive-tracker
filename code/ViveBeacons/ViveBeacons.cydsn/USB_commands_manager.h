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

// Attributes
typedef struct USB_commands_manager USB_commands_manager;
struct USB_commands_manager {
    
};

// Public methods
USB_commands_manager* USB_commands_manager_create();
void USB_commands_manager_init(USB_commands_manager *usb_commands_manager);
void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1]);
//void USB_commands_manager_

#endif

/* [] END OF FILE */
