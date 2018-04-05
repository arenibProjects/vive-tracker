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

#if !defined(VIVE_SENSORS_H)
#define VIVE_SENSORS_H

#include <cytypes.h>
#include <stdbool.h>
#include <math.h>
#include "project.h"
#include "TS4231_driver.h"

// Defines
#define DMA_timing_read_BYTES_PER_BURST 127
#define DMA_timing_read_REQUEST_PER_BURST 0
#define DMA_timing_read_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_timing_read_DST_BASE (CYDEV_SRAM_BASE)

#define TIMING_cycle_max_tick 60000
#define TIMING_cycle_divided_by_3_tick TIMING_cycle_max_tick/3

#define TIMING_angle_center_tick 28800
#define TIMING_angle_min_tick TIMING_angle_center_tick - TIMING_cycle_divided_by_3_tick
#define TIMING_angle_max_tick TIMING_angle_center_tick + TIMING_cycle_divided_by_3_tick

#define ANGLE_invalid_value NAN

#define CLK_frequency 7200000 // in Hertz

// Enums
typedef enum Names Names;
enum Names {STIFF, NIVIDI, PIRNOIR, ARMEN, VIELLE, MILLIER, PETITMINOU, KEREON};

// Attributes
typedef struct VIVE_sensors VIVE_sensors;
struct VIVE_sensors {
    // TS4231_drivers
    TS4231_driver* ts4231_drivers[8];
    
    // Variable declarations for DMA_timing_read
    uint8 DMA_timing_read_Chan;
    uint8 DMA_timing_read_TD[16];
    
    // Raw values
    volatile uint16_t sync_pulses[8];
    volatile uint16_t timing[8];
};

// Data structure for transferring VIVE sensors data to Position_finder
typedef struct VIVE_sensors_data VIVE_sensors_data;
struct VIVE_sensors_data {
    // Processed angles
    double angles[8];
    
    // sync pulses decoded
    uint8_t skip;
    uint8_t data;
    uint8_t axis;
};

// Global variable
volatile bool VIVE_pulses_decoded;

// Public methods
VIVE_sensors* VIVE_sensors_create();
void VIVE_sensors_init(VIVE_sensors *vive_sensors);
VIVE_sensors_data* VIVE_sensors_process_pulses(VIVE_sensors *vive_sensors);

// Interrupt Service Routines
CY_ISR_PROTO(isr_timing_read);

#endif

/* [] END OF FILE */
