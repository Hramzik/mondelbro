#ifndef MDL_HPP_INCLUDED
#define MDL_HPP_INCLUDED
//--------------------------------------------------


#include "mdl_common.hpp"


//--------------------------------------------------


Pixel_color* calculate_mdl_colors_ver1 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance);

Pixel_color calculate_mdl_color_ver1              (Point  point);
Return_code calculate_mdl_color_ver1_update_point (Point* point, Point base);
bool        calculate_mdl_color_ver1_finished     (Point  point, size_t number_of_operations);
Pixel_color calculate_mdl_color_ver1_get_color    (              size_t number_of_operations);


//--------------------------------------------------
#endif