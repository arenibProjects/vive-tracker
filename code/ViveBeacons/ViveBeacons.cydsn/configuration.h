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

#if !defined(CONFIGURATION_H)
#define CONFIGURATION_H

#include <cytypes.h>

#define J_AXIS 0
#define K_AXIS 1

#define SQRT_2 1.41421356237

// Sweeping axis
#define HORIZONTAL_AXIS K_AXIS
#define VERTICAL_AXIS J_AXIS

#define BEACON_COORD_X 315
#define BEACON_COORD_Y 30
#define BEACON_COORD_Z 315

#define BEACON_COORD_ALPHA 0
#define BEACON_COORD_BETA CY_M_PI/2
#define BEACON_COORD_GAMMA 0

#define LED_COORD_HEIGHT 0

// Helps for offset table
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#endif 

/* [] END OF FILE */
