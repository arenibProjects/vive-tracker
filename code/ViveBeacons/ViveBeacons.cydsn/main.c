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

#include <stdio.h>
#include "project.h"
#include "vive_sensors.h"

int main(void) {
    int i = 0;
    CyGlobalIntEnable; /* Enable global interrupts. */ 
    
    USB_Serial_Start(0, USB_Serial_5V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
    
    vive_sensors vive_sensors;
    vive_sensors_init(&vive_sensors);
    
    timing_reset_Write(254);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;) {
        if(VIVE_pulses_decoded) {
            
            VIVE_pulses_decoded = 0;
            
            if(i%8==0) {
            char buffer[512];
            USB_Serial_PutString("-----------\n");
            while(USB_Serial_CDCIsReady() == 0u);
            sprintf(buffer, "sync_pulses[0] : %d\n", *VIVEDecoder_1_VIVEDecoder_F0_PTR);
            USB_Serial_PutString(buffer);
            while(USB_Serial_CDCIsReady() == 0u);
            sprintf(buffer, "timing[0] : %d\n", *VIVEDecoder_1_VIVEDecoder_F1_PTR);
            USB_Serial_PutString(buffer);
            while(USB_Serial_CDCIsReady() == 0u);
            }
            i++;
        }
    }
}

/* [] END OF FILE */
