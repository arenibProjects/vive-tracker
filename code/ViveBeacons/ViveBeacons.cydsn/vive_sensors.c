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

#include "vive_sensors.h"

void vive_sensors_init(vive_sensors *vive_sensors) {
    /* Variable declarations for DMA_timing_read */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_timing_read_Chan;
    uint8 DMA_timing_read_TD[16];

    /* DMA Configuration for DMA_timing_read */
    DMA_timing_read_Chan = DMA_timing_read_DmaInitialize(DMA_timing_read_BYTES_PER_BURST, DMA_timing_read_REQUEST_PER_BURST,
        HI16(DMA_timing_read_SRC_BASE), HI16(DMA_timing_read_DST_BASE));
    DMA_timing_read_TD[0] = CyDmaTdAllocate();
    DMA_timing_read_TD[1] = CyDmaTdAllocate();
    DMA_timing_read_TD[2] = CyDmaTdAllocate();
    DMA_timing_read_TD[3] = CyDmaTdAllocate();
    DMA_timing_read_TD[4] = CyDmaTdAllocate();
    DMA_timing_read_TD[5] = CyDmaTdAllocate();
    DMA_timing_read_TD[6] = CyDmaTdAllocate();
    DMA_timing_read_TD[7] = CyDmaTdAllocate();
    DMA_timing_read_TD[8] = CyDmaTdAllocate();
    DMA_timing_read_TD[9] = CyDmaTdAllocate();
    DMA_timing_read_TD[10] = CyDmaTdAllocate();
    DMA_timing_read_TD[11] = CyDmaTdAllocate();
    DMA_timing_read_TD[12] = CyDmaTdAllocate();
    DMA_timing_read_TD[13] = CyDmaTdAllocate();
    DMA_timing_read_TD[14] = CyDmaTdAllocate();
    DMA_timing_read_TD[15] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_timing_read_TD[0],  2, DMA_timing_read_TD[1], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[1],  2, DMA_timing_read_TD[2], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[2],  2, DMA_timing_read_TD[3], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[3],  2, DMA_timing_read_TD[4], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[4],  2, DMA_timing_read_TD[5], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[5],  2, DMA_timing_read_TD[6], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[6],  2, DMA_timing_read_TD[7], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[7],  2, DMA_timing_read_TD[8], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[8],  2, DMA_timing_read_TD[9], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[9],  2, DMA_timing_read_TD[10], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[10], 2, DMA_timing_read_TD[11], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[11], 2, DMA_timing_read_TD[12], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[12], 2, DMA_timing_read_TD[13], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[13], 2, DMA_timing_read_TD[14], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[14], 2, DMA_timing_read_TD[15], CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_timing_read_TD[15], 2, DMA_timing_read_TD[0], DMA_timing_read__TD_TERMOUT_EN);
    CyDmaTdSetAddress(DMA_timing_read_TD[0],  LO16((uint32)VIVEDecoder_1_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 0)));
    CyDmaTdSetAddress(DMA_timing_read_TD[1],  LO16((uint32)VIVEDecoder_2_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 1)));
    CyDmaTdSetAddress(DMA_timing_read_TD[2],  LO16((uint32)VIVEDecoder_3_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 2)));
    CyDmaTdSetAddress(DMA_timing_read_TD[3],  LO16((uint32)VIVEDecoder_4_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 3)));
    CyDmaTdSetAddress(DMA_timing_read_TD[4],  LO16((uint32)VIVEDecoder_5_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 4)));
    CyDmaTdSetAddress(DMA_timing_read_TD[5],  LO16((uint32)VIVEDecoder_6_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 5)));
    CyDmaTdSetAddress(DMA_timing_read_TD[6],  LO16((uint32)VIVEDecoder_7_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 6)));
    CyDmaTdSetAddress(DMA_timing_read_TD[7],  LO16((uint32)VIVEDecoder_8_VIVEDecoder_F0_PTR), LO16((uint32)(vive_sensors->sync_pulses + 7)));
    CyDmaTdSetAddress(DMA_timing_read_TD[8],  LO16((uint32)VIVEDecoder_1_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 0)));
    CyDmaTdSetAddress(DMA_timing_read_TD[9],  LO16((uint32)VIVEDecoder_2_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 1)));
    CyDmaTdSetAddress(DMA_timing_read_TD[10], LO16((uint32)VIVEDecoder_3_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 2)));
    CyDmaTdSetAddress(DMA_timing_read_TD[11], LO16((uint32)VIVEDecoder_4_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 3)));
    CyDmaTdSetAddress(DMA_timing_read_TD[12], LO16((uint32)VIVEDecoder_5_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 4)));
    CyDmaTdSetAddress(DMA_timing_read_TD[13], LO16((uint32)VIVEDecoder_6_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 5)));
    CyDmaTdSetAddress(DMA_timing_read_TD[14], LO16((uint32)VIVEDecoder_7_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 6)));
    CyDmaTdSetAddress(DMA_timing_read_TD[15], LO16((uint32)VIVEDecoder_8_VIVEDecoder_F1_PTR), LO16((uint32)(vive_sensors->timing + 7)));
    CyDmaChSetInitialTd(DMA_timing_read_Chan, DMA_timing_read_TD[0]);
    CyDmaChEnable(DMA_timing_read_Chan, 1);

    // Triad chips init
    //vive_sensors_TS4231_init();

    // Start interrupt
    isr_timing_redirect_StartEx(isr_timing_read);
}

void vive_sensors_TS4231_init() {
    CyPins_SetPin(data_pins_0);
    CyPins_SetPin(data_pins_1);
    CyPins_SetPin(data_pins_2);
    CyPins_SetPin(data_pins_3);
    CyPins_SetPin(data_pins_4);
    CyPins_SetPin(data_pins_5);
    CyPins_SetPin(data_pins_6);
    CyPins_SetPin(data_pins_7);

    CyPins_SetPin(enveloppe_pins_0);
    CyPins_SetPin(enveloppe_pins_1);
    CyPins_SetPin(enveloppe_pins_2);
    CyPins_SetPin(enveloppe_pins_3);
    CyPins_SetPin(enveloppe_pins_4);
    CyPins_SetPin(enveloppe_pins_5);
    CyPins_SetPin(enveloppe_pins_6);
    CyPins_SetPin(enveloppe_pins_7);

    CyPins_SetPinDriveMode(data_pins_0, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_2, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_3, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_4, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_5, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_6, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(data_pins_7, CY_PINS_DM_STRONG);

    CyPins_SetPinDriveMode(enveloppe_pins_0, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_1, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_2, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_3, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_4, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_5, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_6, CY_PINS_DM_STRONG);
    CyPins_SetPinDriveMode(enveloppe_pins_7, CY_PINS_DM_STRONG);

    CyPins_ClearPin(enveloppe_pins_0);
    CyPins_ClearPin(enveloppe_pins_1);
    CyPins_ClearPin(enveloppe_pins_2);
    CyPins_ClearPin(enveloppe_pins_3);
    CyPins_ClearPin(enveloppe_pins_4);
    CyPins_ClearPin(enveloppe_pins_5);
    CyPins_ClearPin(enveloppe_pins_6);
    CyPins_ClearPin(enveloppe_pins_7);

    CyDelay(2);

    CyPins_ClearPin(data_pins_0);
    CyPins_ClearPin(data_pins_1);
    CyPins_ClearPin(data_pins_2);
    CyPins_ClearPin(data_pins_3);
    CyPins_ClearPin(data_pins_4);
    CyPins_ClearPin(data_pins_5);
    CyPins_ClearPin(data_pins_6);
    CyPins_ClearPin(data_pins_7);

    CyDelayUs(10);

    CyPins_SetPin(enveloppe_pins_0);
    CyPins_SetPin(enveloppe_pins_1);
    CyPins_SetPin(enveloppe_pins_2);
    CyPins_SetPin(enveloppe_pins_3);
    CyPins_SetPin(enveloppe_pins_4);
    CyPins_SetPin(enveloppe_pins_5);
    CyPins_SetPin(enveloppe_pins_6);
    CyPins_SetPin(enveloppe_pins_7);

    CyDelayUs(100);

    CyPins_SetPinDriveMode(data_pins_0, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_1, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_2, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_3, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_4, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_5, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_6, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(data_pins_7, CY_PINS_DM_DIG_HIZ);

    CyPins_SetPinDriveMode(enveloppe_pins_0, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_1, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_2, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_3, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_4, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_5, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_6, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(enveloppe_pins_7, CY_PINS_DM_DIG_HIZ);
}

CY_ISR(isr_timing_read) {
    // Clear pending bit (from interrupt API)
    isr_timing_redirect_ClearPending();

    VIVE_pulses_decoded = 1;
}

/* [] END OF FILE */
