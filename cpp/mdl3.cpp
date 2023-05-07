

//--------------------------------------------------

#include "../hpp/mdl3.hpp"

//--------------------------------------------------


// version 2

Pixel_color* calculate_mdl_colors_ver3 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance) { // point = top-left

    Pixel_color* result = (Pixel_color*) result_void_ptr;


    size_t point_number = 0;

    for (size_t num_y = 0; num_y < height; num_y++) {

        size_t num_x = 0;

        for (/*size_t num_x = 0*/; num_x < width - 3; num_x += 4) { // new

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            calculate_mdl_color_ver3 ( { x, y }, unit_distance, result + point_number);

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

Return_code calculate_mdl_color_ver3 (Point point, double unit_distance, Pixel_color* result) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    double bases_x [4] = { point.x, point.x + unit_distance,
                                    point.x + unit_distance * 2,
                                    point.x + unit_distance * 3 };

    double bases_y [4] = { point.y, point.y, point.y, point.y };

    double curent_x      [4] = { 0, 0, 0, 0 };
    double curent_y      [4] = { 0, 0, 0, 0 };
    double temp1         [4] = { 0, 0, 0, 0 };
    double temp2         [4] = { 0, 0, 0, 0 };
    size_t op_nums       [4] = { 1, 1, 1, 1 };
    double rs_squared    [4] = { 0, 0, 0, 0 };
    bool   finished      [4] = { false, false, false, false };
    bool   finished_all      = false;


    do {

        qdouble_set (temp1, curent_x); // save x coord


        qdouble_mul (curent_x, curent_x); // xn+1
        qdouble_set (temp2,    curent_y); // temp2 = y*y
        qdouble_mul (temp2,    curent_y);
        qdouble_sub (curent_x, temp2   );
        qdouble_add (curent_x, bases_x );

        qdouble_add (curent_y, curent_y); // yn+1 = 2 * y_n
        qdouble_mul (curent_y, temp1   ); // *= xn
        qdouble_add (curent_y, bases_y );


        qdouble_set (rs_squared, curent_x); // r^2 = x^2
        qdouble_mul (rs_squared, curent_x);
        qdouble_set (temp1,      curent_y); // y^2
        qdouble_mul (temp1,      curent_y);
        qdouble_add (rs_squared, temp1   ); // r^2 = x^2 + y^2


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



inline Return_code qdouble_set (double* dst, double* src) {

    if (!dst || !src) return BAD_ARGS;


    for (size_t i = 0; i < 4; i++) dst [i] = src [i];


    return SUCCESS;
}


inline Return_code qdouble_mul (double* dst, double* src) {

    if (!dst || !src) return BAD_ARGS;


    for (size_t i = 0; i < 4; i++) dst [i] *= src [i];


    return SUCCESS;
}


inline Return_code qdouble_add (double* dst, double* src) {

    if (!dst || !src) return BAD_ARGS;


    for (size_t i = 0; i < 4; i++) dst [i] += src [i];


    return SUCCESS;
}


inline Return_code qdouble_sub (double* dst, double* src) {

    if (!dst || !src) return BAD_ARGS;


    for (size_t i = 0; i < 4; i++) dst [i] -= src [i];


    return SUCCESS;
}

