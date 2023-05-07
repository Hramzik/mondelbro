#ifndef MDL_COMMON_HPP_INCLUDED
#define MDL_COMMON_HPP_INCLUDED
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
#endif