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

#if !defined(POSITION_FINDER_H)
#define POSITION_FINDER_H

#include <cytypes.h>
#include <stdbool.h>
#include "project.h"
#include "VIVE_sensors.h"
#include "position.h"
    
// Attributes
typedef struct Position_finder Position_finder;
struct Position_finder {
    double led_height;
    Position3D* beacon_position;
    Position2D current_position;
    Position2D previous_position;
    VIVE_sensors_data* vive_sensors_data_v;
    VIVE_sensors_data* vive_sensors_data_h;
};

// Public methods
Position_finder* Position_finder_create();
void Position_finder_init(Position_finder *position_finder, Position3D* beacon_position, double led_height);
void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data);

// Utils
double Position_finder_normalize_angle(double angle);


#endif

/* [] END OF FILE */
