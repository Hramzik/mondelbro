

//--------------------------------------------------

#include "../hpp/mdl2.hpp"

//--------------------------------------------------


// version 2

Pixel_color* calculate_mdl_colors_ver2 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance) { // point = top-left

    Pixel_color* result = (Pixel_color*) result_void_ptr;


    size_t point_number = 0;

    for (size_t num_y = 0; num_y < height; num_y++) {

        size_t num_x = 0;

        for (/*size_t num_x = 0*/; num_x < width - 3; num_x += 4) { // new

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            calculate_mdl_color_ver2 ( { x, y }, unit_distance, result + point_number);

            point_number += 4;
        }

        for (/*size_t num_x = 0*/; num_x < width; num_x++) { // old

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            result [point_number] = calculate_mdl_color_ver1 ( { x, y } );

            point_number += 4;;
        }
    }


    return result;
}

Return_code calculate_mdl_color_ver2 (Point point, double unit_distance, Pixel_color* result) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Point bases [4] = { point, { point.x + unit_distance,     point.y },
                               { point.x + unit_distance * 2, point.y },
                               { point.x + unit_distance * 3, point.y } };

    double curent_x      [4] = { 0, 0, 0, 0 };
    double curent_y      [4] = { 0, 0, 0, 0 };
    double temp1         [4] = { 0, 0, 0, 0 };
    double temp2         [4] = { 0, 0, 0, 0 };
    size_t op_nums       [4] = { 1, 1, 1, 1 };
    double rs_squared    [4] = { 0, 0, 0, 0 };
    bool   finished      [4] = { false, false, false, false };
    bool   finished_all      = false;


    do {

        for (size_t i = 0; i < 4; i++) temp1 [i] = curent_x [i]; // save x coord


        for (size_t i = 0; i < 4; i++) curent_x [i]   *= curent_x [i]; // xn+1
        for (size_t i = 0; i < 4; i++) temp2    [i]    = curent_y [i]; // temp2 = y*y
        for (size_t i = 0; i < 4; i++) temp2    [i]   *= curent_y [i];
        for (size_t i = 0; i < 4; i++) curent_x [i]   -= temp2    [i];
        for (size_t i = 0; i < 4; i++) curent_x [i]   += bases    [i].x;

        for (size_t i = 0; i < 4; i++) curent_y [i] += curent_y [i]; // yn+1 = 2 * y_n
        for (size_t i = 0; i < 4; i++) curent_y [i] *= temp1    [i]; // *= xn
        for (size_t i = 0; i < 4; i++) curent_y [i] += bases    [i].y;


        for (size_t i = 0; i < 4; i++) rs_squared [i]  = curent_x [i]; // r^2 = x^2
        for (size_t i = 0; i < 4; i++) rs_squared [i] *= curent_x [i];
        for (size_t i = 0; i < 4; i++) temp1      [i]  = curent_y [i]; // y^2
        for (size_t i = 0; i < 4; i++) temp1      [i] *= curent_y [i];
        for (size_t i = 0; i < 4; i++) rs_squared [i] += temp1    [i]; // r^2 = x^2 + y^2


        for (size_t i = 0; i < 4; i++) { // r^2 > 4?

            if (rs_squared [i] >  CRITICAL_POINT_RADIUS_SQUARED) finished [i] = true;
            if (op_nums    [i] >= CRITICAL_NUMBER_OF_OPERATIONS) finished [i] = true;

            if (!finished [i]) op_nums [i] += 1;
        }


        finished_all = true;

        for (size_t i = 0; i < 4; i++) { // finished all?

            finished_all &= finished [i];
        }

    } while (!finished_all);


    //--------------------------------------------------


    for (size_t i = 0; i < 4; i++) {

        result [i] = calculate_mdl_color_ver1_get_color (op_nums [i]);
    }


    return SUCCESS;
}

