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

/*
    Units used in all the firmware :
    - distances, coordinates : m
    - angles : rad
    - time : s
    - velocity : m/s
    - acceleration : m/(s^2)
    - temperature : °C
*/

#include <stdlib.h>
#include <stdio.h>
#include "main.h"

int main(void) {
    int i = 0;
    VIVE_sensors_data* vive_sensors_data = NULL;
    CyGlobalIntEnable; /* Enable global interrupts. */ 
    
    USB_Serial_Start(0, USB_Serial_5V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
    
    USB_Serial_PutString("START\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    CyDelay(3000);
    
    USB_Serial_PutString("INIT\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    VIVE_sensors* vive_sensors = VIVE_sensors_create();
    VIVE_sensors_init(vive_sensors);
    
    //timing_reset_Write(254);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    USB_Serial_PutString("READY\n");
    while(USB_Serial_CDCIsReady() == 0u);

    for(;;) {
        if(VIVE_pulses_decoded) {
            
            VIVE_pulses_decoded = 0;
            vive_sensors_data = VIVE_sensors_process_pulses(vive_sensors);
            free(vive_sensors_data);
            
            for(int i = 0; i < 8; i++) {
                char buffer[512];
                sprintf(buffer, "ANGLE %d %d %f\n", i, vive_sensors_data->axis, vive_sensors_data->angles[i]);
                USB_Serial_PutString(buffer);
                while(USB_Serial_CDCIsReady() == 0u);
            }
        }
    }
}

/* [] END OF FILE */
