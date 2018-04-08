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
#include <stdlib.h>
#include <math.h>
#include "configuration.h"
#include "Position_finder.h"

/*
    --------------
    Public methods
    --------------
*/

Position_finder* Position_finder_create() {
    Position_finder* position_finder = (Position_finder*) malloc(1*sizeof(Position_finder));
    
    return position_finder;
}

void Position_finder_init(Position_finder *position_finder, Position3D* beacon_position, double led_height) {
    position_finder->vive_sensors_data_v = NULL;
    position_finder->vive_sensors_data_h = NULL;
    
    position_finder->beacon_position = beacon_position;
    
    position_finder->led_height = led_height;
}

void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data) {
    Position2D led_positions[8];

    // Update data
    if(vive_sensors_data->axis == VERTICAL_AXIS) {
        free(position_finder->vive_sensors_data_v); // free old data
        position_finder->vive_sensors_data_v = vive_sensors_data;
    }
    else if(vive_sensors_data->axis == HORIZONTAL_AXIS) {
        free(position_finder->vive_sensors_data_h); // free old data
        position_finder->vive_sensors_data_h = vive_sensors_data;
    }
    
    if(position_finder->vive_sensors_data_h == NULL || position_finder->vive_sensors_data_v == NULL)
        return;
    
    // Process LED angles
    for(int i = 0; i < 8; i++) {
        if(position_finder->vive_sensors_data_h->angles[i] == ANGLE_invalid_value || position_finder->vive_sensors_data_v->angles[i] == ANGLE_invalid_value)
            continue;
        
        double v_angle = position_finder->beacon_position->alpha+position_finder->vive_sensors_data_v->angles[i];
        double h_angle = position_finder->beacon_position->beta-(position_finder->vive_sensors_data_h->angles[i]);
        
        double l = (position_finder->beacon_position->z - position_finder->led_height)*tan(CY_M_PI/2-fabs(v_angle));
        led_positions[i].x = position_finder->beacon_position->x + cos(h_angle)*l;
        led_positions[i].y = position_finder->beacon_position->y + sin(h_angle)*l;
        
        char buffer[512];
        sprintf(buffer, "LEDPOS %d %f %f\n", i, led_positions[i].x, led_positions[i].y);
        USB_Serial_PutString(buffer);
        while(USB_Serial_CDCIsReady() == 0u);
    }
    
    /*for(int i = 0; i < 8; i++) {
        char buffer[512];
        sprintf(buffer, "LEDPOS %d %f %f\n", i, led_positions[i].x, led_positions[i].y);
        USB_Serial_PutString(buffer);
        while(USB_Serial_CDCIsReady() == 0u);
    }*/
}

/* [] END OF FILE */
