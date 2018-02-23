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

#include "project.h"

/* Defines for DMA_timing_read */
#define DMA_timing_read_BYTES_PER_BURST 127
#define DMA_timing_read_REQUEST_PER_BURST 0
#define DMA_timing_read_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_timing_read_DST_BASE (CYDEV_SRAM_BASE)

typedef struct vive_sensors vive_sensors;
struct vive_sensors {
    // Raw values
    uint16_t sync_pulses[8];
    uint16_t timing[8];
    
    // sync pulses decoded
    uint8_t skip;
    uint8_t data;
    uint8_t axis;
};

void vive_sensors_init(vive_sensors *vive_sensors);
void vive_sensors_TS4231_init();

CY_ISR_PROTO(isr_timing_read);
volatile char VIVE_pulses_decoded;

/* [] END OF FILE */
