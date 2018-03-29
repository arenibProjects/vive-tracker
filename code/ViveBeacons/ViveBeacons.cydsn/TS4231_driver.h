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

#if !defined(TS4231_DRIVER_H)
#define TS4231_DRIVER_H

#include <cytypes.h>
#include <stdbool.h>
#include "project.h"

// Defines
#define BUS_DRIVE_DELAY 1 // in microseconds
#define BUS_CHECK_DELAY 500 // in microseconds
#define SLEEP_RECOVERY 100 // in microseconds

#define DEFAULT_CONFIGURATION_VALUE 0x392B

// Enums
typedef enum State State;
enum State {S0, SLEEP, WATCH, S3};

typedef enum Status Status;
enum Status {UNKNOWN, BUS_FAIL, VERIFY_FAIL, WATCH_FAIL, CONFIG_PASS};

// Attributes
typedef struct TS4231_driver TS4231_driver;
struct TS4231_driver {
    uint32_t enveloppe_pin;
    uint32_t data_pin;
    State state;
};

// Public methods
TS4231_driver* TS4231_driver_create(uint32_t enveloppe_pin, uint32_t data_pin);
void TS4231_driver_init(TS4231_driver* ts4231_driver);
bool TS4231_driver_go_to_watch(TS4231_driver* ts4231_driver);
bool TS4231_driver_go_to_sleep(TS4231_driver* ts4231_driver);
void TS4231_driver_delete(TS4231_driver* ts4231_driver);

// Private methods
void TS4231_driver_wait_for_light(TS4231_driver* ts4231_driver);
Status TS4231_driver_configure(TS4231_driver* ts4231_driver, uint16_t configuration_value);
State TS4231_driver_check_bus(TS4231_driver* ts4231_driver);
void TS4231_driver_write_configuration(TS4231_driver* ts4231_driver, uint16_t configuration_value);
uint16_t TS4231_driver_read_configuration(TS4231_driver* ts4231_driver);

#endif

/* [] END OF FILE */
