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
#include "main.h"



int main(void) {
    int i = 0;
    CyGlobalIntEnable; /* Enable global interrupts. */ 
    
    USB_Serial_Start(0, USB_Serial_5V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
    
    CyDelay(3000);
    
    USB_Serial_PutString("STARTING ... \n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    vive_sensors vive_sensors;
    vive_sensors_init(&vive_sensors);
    
    USB_Serial_PutString("INITIALIZATION ... \n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    TS4231_driver *stiff = TS4231_driver_create(enveloppe_pins_0, data_pins_0);
    TS4231_driver_init(stiff);
    
    //timing_reset_Write(254);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    USB_Serial_PutString("CONFIGURED !\n");
    while(USB_Serial_CDCIsReady() == 0u);
    
    for(;;) {
        while(!TS4231_driver_go_to_watch(stiff));
        USB_Serial_PutString("WATCH OK\n");
        while(USB_Serial_CDCIsReady() == 0u);
        CyDelay(1000);
        
        while(!TS4231_driver_go_to_sleep(stiff));
        USB_Serial_PutString("SLEEP OK\n");
        while(USB_Serial_CDCIsReady() == 0u);
        CyDelay(1000);
        
        /*if(VIVE_pulses_decoded) {
            
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
        }*/
    }
}

/* [] END OF FILE */
