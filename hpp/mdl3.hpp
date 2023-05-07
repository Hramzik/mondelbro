#ifndef MDL_3_HPP_INCLUDED
#define MDL_3_HPP_INCLUDED
//--------------------------------------------------


#include "mdl_common.hpp"


//--------------------------------------------------


extern Pixel_color calculate_mdl_color_ver1           (Point  point);
extern Pixel_color calculate_mdl_color_ver1_get_color (size_t number_of_operations);


//--------------------------------------------------


Pixel_color* calculate_mdl_colors_ver3 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance);
Return_code  calculate_mdl_color_ver3  (Point point, double unit_distance, Pixel_color* result);

inline Return_code qdouble_set (double* dst, double* src);
inline Return_code qdouble_mul (double* dst, double* src);
inline Return_code qdouble_add (double* dst, double* src);
inline Return_code qdouble_sub (double* dst, double* src);

//--------------------------------------------------
#endif