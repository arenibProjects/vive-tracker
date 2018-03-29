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
#include "configuration.h"
#include "VIVE_sensors.h"

/*
    --------------
    Public methods
    --------------
*/

VIVE_sensors* VIVE_sensors_create() {
    VIVE_sensors* vive_sensors = (VIVE_sensors*) malloc(1*sizeof(VIVE_sensors));
    
    // TS4231 drivers creation
    vive_sensors->ts4231_drivers[0] = TS4231_driver_create(enveloppe_pins_0, data_pins_0);
    vive_sensors->ts4231_drivers[1] = TS4231_driver_create(enveloppe_pins_1, data_pins_1);
    vive_sensors->ts4231_drivers[2] = TS4231_driver_create(enveloppe_pins_2, data_pins_2);
    vive_sensors->ts4231_drivers[3] = TS4231_driver_create(enveloppe_pins_3, data_pins_3);
    vive_sensors->ts4231_drivers[4] = TS4231_driver_create(enveloppe_pins_4, data_pins_4);
    vive_sensors->ts4231_drivers[5] = TS4231_driver_create(enveloppe_pins_5, data_pins_5);
    vive_sensors->ts4231_drivers[6] = TS4231_driver_create(enveloppe_pins_6, data_pins_6);
    vive_sensors->ts4231_drivers[7] = TS4231_driver_create(enveloppe_pins_7, data_pins_7);
    
    return vive_sensors;
}

void VIVE_sensors_init(VIVE_sensors *vive_sensors) {
    // DMA Configuration for DMA_timing_read
    
    // DMA Channel initialization
    vive_sensors->DMA_timing_read_Chan = DMA_timing_read_DmaInitialize(DMA_timing_read_BYTES_PER_BURST, DMA_timing_read_REQUEST_PER_BURST,
        HI16(DMA_timing_read_SRC_BASE), HI16(DMA_timing_read_DST_BASE));
    
    // DMA Transfer Descriptors (TD) allocation
    for(int i = 0; i < 16; i++)
        vive_sensors->DMA_timing_read_TD[i] = CyDmaTdAllocate();
    
    // DMA TDs configuration
    for(int i = 0; i < 15; i++) // 16-1 = 15 because ...
        CyDmaTdSetConfiguration(vive_sensors->DMA_timing_read_TD[i],  2, vive_sensors->DMA_timing_read_TD[i+1], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(vive_sensors->DMA_timing_read_TD[15], 2, vive_sensors->DMA_timing_read_TD[0], DMA_timing_read__TD_TERMOUT_EN); // ... the last one is different
    
    // DMA TDs address configuration
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[1],  LO16((uint32)VIVEDecoder_2_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 1)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[2],  LO16((uint32)VIVEDecoder_3_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 2)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[3],  LO16((uint32)VIVEDecoder_4_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 3)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[4],  LO16((uint32)VIVEDecoder_5_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 4)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[5],  LO16((uint32)VIVEDecoder_6_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 5)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[6],  LO16((uint32)VIVEDecoder_7_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 6)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[7],  LO16((uint32)VIVEDecoder_8_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 7)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[8],  LO16((uint32)VIVEDecoder_1_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 0)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[9],  LO16((uint32)VIVEDecoder_2_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 1)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[10], LO16((uint32)VIVEDecoder_3_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 2)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[11], LO16((uint32)VIVEDecoder_4_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 3)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[12], LO16((uint32)VIVEDecoder_5_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 4)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[13], LO16((uint32)VIVEDecoder_6_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 5)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[14], LO16((uint32)VIVEDecoder_7_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 6)));
    CyDmaTdSetAddress(vive_sensors->DMA_timing_read_TD[15], LO16((uint32)VIVEDecoder_8_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 7)));
    
    CyDmaChSetInitialTd(vive_sensors->DMA_timing_read_Chan, vive_sensors->DMA_timing_read_TD[0]);
    
    CyDmaChEnable(vive_sensors->DMA_timing_read_Chan, 1);

    // TS4231 drivers init
    for(int i = 0; i < 8; i++)
        TS4231_driver_init(vive_sensors->ts4231_drivers[i]);

    // Start interrupt
    isr_timing_redirect_StartEx(isr_timing_read);
}

void VIVE_sensors_process_pulses(VIVE_sensors *vive_sensors, VIVE_sensors_data* vive_sensors_data) {
    uint8_t timing_readable_reg_value = timing_readable_Read();
    
    // For each sensor
    for(int i = 0; i < 8; i++) {
        vive_sensors_data->angles[i] = ANGLE_invalid_value;
        
        bool timing_readable_bit = (timing_readable_reg_value & (1 << i)) >> i;        
        if(timing_readable_bit) { // If it's readable
            // convert timing to angles
            uint16_t timing = vive_sensors->timing[i];
            
            // constrain
            if(timing>TIMING_angle_max_tick) {
                timing = TIMING_angle_max_tick;
            }
            else if(timing<TIMING_angle_min_tick) {
                timing = TIMING_angle_min_tick;
            }
            
            vive_sensors_data->angles[i] = CY_M_PI*((timing-TIMING_angle_center_tick)/TIMING_cycle_max_tick);
        }
    }
}

/*
    --------------------------
    Interrupt Service Routines
    --------------------------
*/

CY_ISR(isr_timing_read) {
    // Clear pending bit (from interrupt API)
    isr_timing_redirect_ClearPending();

    // Set the flag
    VIVE_pulses_decoded = true;
}

/* [] END OF FILE */
