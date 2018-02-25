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
    
    //timing_reset_Write(254);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;) {
        if(VIVE_pulses_decoded) {
            
            VIVE_pulses_decoded = 0;
            
            USB_Serial_PutString("-----------\n");
            
            for(int i = 0; i < 8; i++) {
                char buffer[512];
                sprintf(buffer, "DMA|sync_pulses[%d] : %d\n", i, vive_sensors.sync_pulses[i]);
                USB_Serial_PutString(buffer);
                while(USB_Serial_CDCIsReady() == 0u);
                sprintf(buffer, "DMA|timing[%d] : %d\n", i, vive_sensors.timing[i]);
                USB_Serial_PutString(buffer);
                while(USB_Serial_CDCIsReady() == 0u);
            }
        }
    }
}

/* [] END OF FILE */
