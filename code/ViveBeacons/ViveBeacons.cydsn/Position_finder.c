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

void Position_finder_init(Position_finder *position_finder) {
    position_finder->vive_sensors_data_v = NULL;
    position_finder->vive_sensors_data_h = NULL;
}

void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data) {
    Position2D *led_positions[8];

    if(vive_sensors_data->axis == VERTICAL_AXIS)
        position_finder->vive_sensors_data_v = vive_sensors_data;
    else if(vive_sensors_data->axis == HORIZONTAL_AXIS)
        position_finder->vive_sensors_data_h = vive_sensors_data;
}

void Position_finder_find_led_position(double v_angle, double h_angle) {
    
}


/* [] END OF FILE */
