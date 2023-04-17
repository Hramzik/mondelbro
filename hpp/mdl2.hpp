#ifndef MDL2_HPP_INCLUDED
#define MDL2_HPP_INCLUDED
//--------------------------------------------------

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../lib/logs.hpp"

//---------------- settings ------------------------

const double CRITICAL_POINT_RADIUS_SQUARED = 4;
const size_t CRITICAL_NUMBER_OF_OPERATIONS = 255;

//--------------------------------------------------


typedef struct Pixel_color {

    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char transparency;

} Pixel_color; const size_t PIXEL_COLOR_SIZE = sizeof (Pixel_color);


typedef struct Point {

    double x;
    double y;

} Point; const size_t POINT_SIZE = sizeof (Point);


//--------------------------------------------------


Pixel_color* calculate_mdl_colors      (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance);
Return_code  calculate_mdl_color_ver2  (Point point, double unit_distance, Pixel_color* result, size_t amount);


//--------------------------------------------------
#endif