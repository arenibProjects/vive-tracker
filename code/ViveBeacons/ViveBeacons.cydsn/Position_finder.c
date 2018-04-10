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
    Position2D* led_positions[8];

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
        led_positions[i] = NULL;
        
        if(position_finder->vive_sensors_data_h->angles[i] == ANGLE_invalid_value || position_finder->vive_sensors_data_v->angles[i] == ANGLE_invalid_value)
            continue;
        
        double v_angle = position_finder->beacon_position->alpha+position_finder->vive_sensors_data_v->angles[i];
        double h_angle = position_finder->beacon_position->beta-(position_finder->vive_sensors_data_h->angles[i]);
        double l = (position_finder->beacon_position->z - position_finder->led_height)*tan(CY_M_PI/2-fabs(v_angle));
        
        led_positions[i] = (Position2D*) malloc(1*sizeof(Position2D));
        led_positions[i]->x = position_finder->beacon_position->x + cos(h_angle)*l;
        led_positions[i]->y = position_finder->beacon_position->y + sin(h_angle)*l;
    }
    
    // Compute heading
    double sum = 0;
    int nb_couple = 0;
    
    for(int i = 0; i < 8; i++) {
        for(int j = i+1; j < 8; j++) {
            if(led_positions[i] == NULL || led_positions[j] == NULL)
                continue;
            
            double applied_delta_x = led_positions[j]->x - led_positions[i]->x;
            double applied_delta_y = led_positions[j]->y - led_positions[i]->y;
            double applied_angle = atan2(applied_delta_y, applied_delta_x);
            
            double theorical_delta_x = tracker_led_offset[j][X_AXIS] - tracker_led_offset[i][X_AXIS];
            double theorical_delta_y = tracker_led_offset[j][Y_AXIS] - tracker_led_offset[i][Y_AXIS];           
            double theorical_angle = atan2(theorical_delta_y, theorical_delta_x);
            
            sum += Position_finder_normalize_angle(applied_angle - theorical_angle);
            nb_couple++;
        }
    }
    
    position_finder->current_position.a = sum / nb_couple;
}

double Position_finder_normalize_angle(double angle) {
    while(angle < -CY_M_PI)
        angle += 2*CY_M_PI;
    
    while(angle > CY_M_PI)
        angle -= 2*CY_M_PI;
    
    return angle;
}
/* [] END OF FILE */
