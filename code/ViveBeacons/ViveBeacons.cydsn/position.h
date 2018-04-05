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

#if !defined(POSITION_H)
#define POSITION_H

// Position2D structure : Represent a position on a plane
typedef struct Position2D Position2D;
struct Position2D {
    double x;
    double y;
    
    double a;
};

// Position structure : Represent a position in a 3D space
typedef struct Position3D Position3D;
struct Position3D {
    double x;
    double y;
    double z;
    
    double alpha;
    double beta;
    double gamma;
};

#endif

/* [] END OF FILE */
