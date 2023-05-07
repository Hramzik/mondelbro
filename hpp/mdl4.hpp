#ifndef MDL_3_HPP_INCLUDED
#define MDL_3_HPP_INCLUDED
//--------------------------------------------------


#include "mdl_common.hpp"

#include <immintrin.h>

//--------------------------------------------------


extern Pixel_color calculate_mdl_color_ver1           (Point  point);
extern Pixel_color calculate_mdl_color_ver1_get_color (size_t number_of_operations);


//--------------------------------------------------


Pixel_color* calculate_mdl_colors_ver4 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance);
Return_code  calculate_mdl_color_ver4  (Point point, double unit_distance, Pixel_color* result);


//--------------------------------------------------
#endif