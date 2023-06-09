#ifndef MDL_2_HPP_INCLUDED
#define MDL_2_HPP_INCLUDED
//--------------------------------------------------


#include "mdl_common.hpp"


//--------------------------------------------------


extern Pixel_color calculate_mdl_color_ver1           (Point  point);
extern Pixel_color calculate_mdl_color_ver1_get_color (size_t number_of_operations);


//--------------------------------------------------


Pixel_color* calculate_mdl_colors_ver2 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance);
Return_code  calculate_mdl_color_ver2  (Point point, double unit_distance, Pixel_color* result);


//--------------------------------------------------
#endif