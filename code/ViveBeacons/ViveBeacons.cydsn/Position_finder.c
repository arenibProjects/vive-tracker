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
    
    position_finder->current_position = (Position2D*) malloc(1*sizeof(Position2D));
    position_finder->previous_position = (Position2D*) malloc(1*sizeof(Position2D));
    
    return position_finder;
}

void Position_finder_init(Position_finder *position_finder, Position3D* beacon_position, double led_height, double tracker_led_offset[8][2]) {
    position_finder->vive_sensors_data_v = NULL;
    position_finder->vive_sensors_data_h = NULL;
    
    position_finder->beacon_position = beacon_position;
    
    position_finder->led_height = led_height;
    
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 2; j++)
            position_finder->tracker_led_offset[i][j] = tracker_led_offset[i][j];
}

void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data) {
    Position2D* led_positions[8];
    Position2D* position = position_finder->current_position;
    double *heading_values = NULL;
    double *x_values = NULL;
    double *y_values = NULL;
    
    for(int i = 0; i < 8; i++) {
        led_positions[i] = NULL;
    }

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
        if(isnan(position_finder->vive_sensors_data_h->angles[i]) || isnan(position_finder->vive_sensors_data_v->angles[i]))
            continue;
        
        double v_angle = position_finder->beacon_position->alpha+position_finder->vive_sensors_data_v->angles[i];
        double h_angle = position_finder->beacon_position->beta-(position_finder->vive_sensors_data_h->angles[i]);
        double l = (position_finder->beacon_position->z - position_finder->led_height)*tan(CY_M_PI/2-fabs(v_angle));
        
        Position2D* current_led_position = (Position2D*) malloc(1*sizeof(Position2D));
        current_led_position->x = position_finder->beacon_position->x + cos(h_angle)*l;
        current_led_position->y = position_finder->beacon_position->y + sin(h_angle)*l;
        led_positions[i] = current_led_position;
    }
    
    // Compute heading
    double sum = 0;
    int nb_couple = 0;
    
    for(int i = 0; i < 8; i+=2) {
        for(int j = i+2; j < 8; j+=2) {
            if(led_positions[i] == NULL || led_positions[j] == NULL)
                continue;
            
            if(isnan(led_positions[i]->x) || isnan(led_positions[i]->y) || isnan(led_positions[j]->x) || isnan(led_positions[j]->y))
                continue; 
            
            // Compute heading for each sensors
            double applied_delta_x = led_positions[j]->x - led_positions[i]->x;
            double applied_delta_y = led_positions[j]->y - led_positions[i]->y;
            double applied_angle = atan2(applied_delta_y, applied_delta_x);
            
            double theorical_delta_x = position_finder->tracker_led_offset[j][X_AXIS] - position_finder->tracker_led_offset[i][X_AXIS];
            double theorical_delta_y = position_finder->tracker_led_offset[j][Y_AXIS] - position_finder->tracker_led_offset[i][Y_AXIS];           
            double theorical_angle = atan2(theorical_delta_y, theorical_delta_x);
            
            nb_couple++;
            
            // Add to heading_values table
            heading_values = (double*) realloc(heading_values, nb_couple*sizeof(double));
            heading_values[nb_couple-1] = Position_finder_normalize_angle(applied_angle - theorical_angle);
        }
    }
    
    if(nb_couple > 1) {
        // -- Median filter --
        qsort(heading_values, nb_couple, sizeof(double), Position_finder_compare); // sort
        
        if((nb_couple % 2) == 0) // If length of heading_values is even
            position->a = (heading_values[(int) floor(nb_couple/2.0)] + heading_values[(int) ceil(nb_couple/2.0)]) / 2;
        else
            position->a = heading_values[nb_couple/2];
    } else if(nb_couple == 1) {
        position->a = heading_values[0];
    } else
        position->a = NAN;
    
    int nb_led_pos = 0;
    
    // Compute X,Y position
    for(int i = 0; i < 8; i+=2) {
        double heading = position_finder->current_position->a;
        Position2D* current_led_position = led_positions[i];
        
        if(led_positions[i] == NULL)
            continue;
        
        // compute x and y led coordinates
        double x_rotated = cos(-heading)*current_led_position->x - sin(-heading)*current_led_position->y;
        double y_rotated = sin(-heading)*current_led_position->x + cos(-heading)*current_led_position->y;
        
        nb_led_pos++;
        x_values = (double*) realloc(x_values, nb_led_pos*sizeof(double));
        x_values[nb_led_pos-1] = x_rotated - position_finder->tracker_led_offset[i][X_AXIS];
        
        y_values = (double*) realloc(y_values, nb_led_pos*sizeof(double));
        y_values[nb_led_pos-1] = y_rotated - position_finder->tracker_led_offset[i][Y_AXIS];
    }
    
    if(nb_led_pos > 1) {
        // -- Median filter --
        qsort(x_values, nb_led_pos, sizeof(double), Position_finder_compare);
        qsort(y_values, nb_led_pos, sizeof(double), Position_finder_compare);
        
        if((nb_led_pos % 2) == 0) { // If length of heading_values is even
            position->x = (x_values[(int) floor(nb_led_pos/2.0)] + x_values[(int) ceil(nb_led_pos/2.0)]) / 2;
            position->y = (y_values[(int) floor(nb_led_pos/2.0)] + y_values[(int) ceil(nb_led_pos/2.0)]) / 2;
        } else {
            position->x = x_values[nb_led_pos/2];
            position->y = x_values[nb_led_pos/2];
        }
    } else if(nb_led_pos == 1) {
        position->x = x_values[0];
        position->y = y_values[0];
    } else {
        position->x = NAN;
        position->y = NAN;
    }
    
    // Free old data
    for(int i = 0; i < 8; i++) {
        Position2D* current_led_position = led_positions[i];
        free(current_led_position);
    }
    
    free(heading_values);
    free(x_values);
    free(y_values);
}

double Position_finder_normalize_angle(double angle) {
    while(angle < -CY_M_PI)
        angle += 2*CY_M_PI;
    
    while(angle > CY_M_PI)
        angle -= 2*CY_M_PI;
    
    return angle;
}

static int Position_finder_compare(void const *a, void const *b)
{
    int ret = 0;
    double const *pa = a;
    double const *pb = b;
    double diff = *pa - *pb;

    if (diff > 0)
        ret = 1;
    else if (diff < 0)
        ret = -1;
    else
        ret = 0;

    return ret;
}
/* [] END OF FILE */
