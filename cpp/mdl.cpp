

//--------------------------------------------------

#include "../hpp/mdl.hpp"

//--------------------------------------------------


// global function

Pixel_color* calculate_mdl_colors (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance) { // point = top-left

    Pixel_color* result = (Pixel_color*) result_void_ptr;


    size_t point_number = 0;

    for (size_t num_y = 0; num_y < height; num_y++) {

        for (size_t num_x = 0; num_x < width; num_x++) {

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            result [point_number] = calculate_mdl_color ( { x, y } );

            point_number += 1;
        }
    }


    return result;
}


Pixel_color calculate_mdl_color (Point point) {

    size_t number_of_operations = 0;

    Point temp = { 0, 0 };


    while (!calculate_mdl_color_finished (temp, number_of_operations)) {

        calculate_mdl_color_update_point (&temp, point);
        number_of_operations += 1;
    }

    //printf ("debug: passed point x = %lf, y = %lf, r^2 = %lf num = %zd\n", point.x, point.y, temp.x*temp.x + temp.y*temp.y, number_of_operations);


    return calculate_mdl_color_get_color (number_of_operations);
}


bool calculate_mdl_color_finished (Point point, size_t number_of_operations) {

    if (CRITICAL_NUMBER_OF_OPERATIONS > CRITICAL_NUMBER_OF_OPERATIONS) { LOG_MESSAGE ("bad number of op's"); return true; }


    double point_radius = point.x * point.x + point.y * point.y;


    if (point_radius > CRITICAL_POINT_RADIUS_SQUARED) return true;


    if (number_of_operations == CRITICAL_NUMBER_OF_OPERATIONS) return true;


    return false;
}


Return_code calculate_mdl_color_update_point (Point* point, Point base) {

    if (!point) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    double x = point->x;
    double y = point->y;


    point->x = x * x - y * y + base.x;
    point->y = 2 * x * y     + base.y;


    return SUCCESS;
}


Pixel_color calculate_mdl_color_get_color (size_t number_of_operations) {

    Pixel_color result = { 0, 0, 0, 255 };


    double coefficient = (double) number_of_operations / (double) CRITICAL_NUMBER_OF_OPERATIONS;


    result.red   = (unsigned char) (255 * coefficient);
    result.green = (unsigned char) (255 * coefficient);
    result.blue  = (unsigned char) (255 * coefficient);


    return result;
}

