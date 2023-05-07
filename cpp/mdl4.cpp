

//--------------------------------------------------

#include "../hpp/mdl4.hpp"

//--------------------------------------------------


// version 2

Pixel_color* calculate_mdl_colors_ver4 (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance) { // point = top-left

    Pixel_color* result = (Pixel_color*) result_void_ptr;


    size_t point_number = 0;

    for (size_t num_y = 0; num_y < height; num_y++) {

        size_t num_x = 0;

        for (/*size_t num_x = 0*/; num_x < width - 3; num_x += 4) { // new

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            calculate_mdl_color_ver4 ( { x, y }, unit_distance, result + point_number);

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

Return_code calculate_mdl_color_ver4 (Point point, double unit_distance, Pixel_color* result) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    __m256d bases_x = { point.x, point.x + unit_distance,
                                 point.x + unit_distance * 2,
                                 point.x + unit_distance * 3 };

    __m256d bases_y = { point.y, point.y, point.y, point.y };

    __m256d curent_x     = { 0, 0, 0, 0 };
    __m256d curent_y     = { 0, 0, 0, 0 };
    __m256d temp1        = { 0, 0, 0, 0 };
    __m256d temp2        = { 0, 0, 0, 0 };
    __m256d rs_squared   = { 0, 0, 0, 0 };
    size_t  op_nums  [4] = { 1, 1, 1, 1 };
    bool    finished [4] = { false, false, false, false };
    bool    finished_all =   false;


    do {

        temp1 = _mm256_movedup_pd (curent_x); // save x coord


        temp2    = _mm256_movedup_pd (curent_y);           // temp2 = y*y
        temp2    = _mm256_mul_pd     (temp2,    curent_y);
        curent_x = _mm256_mul_pd     (curent_x, curent_x); // xn+1
        curent_x = _mm256_sub_pd     (curent_x, temp2   );
        curent_x = _mm256_add_pd     (curent_x, bases_x );

        curent_y = _mm256_add_pd (curent_y, curent_y); // yn+1 = 2 * y_n
        curent_y = _mm256_mul_pd (curent_y, temp1   ); // *= xn
        curent_y = _mm256_add_pd (curent_y, bases_y );


        rs_squared = _mm256_movedup_pd (curent_x); // r^2 = x^2
        rs_squared = _mm256_mul_pd     (rs_squared, curent_x);
        temp1      = _mm256_movedup_pd (curent_y); // y^2
        temp1      = _mm256_mul_pd     (temp1,      curent_y);
        rs_squared = _mm256_add_pd     (rs_squared, temp1   ); // r^2 = x^2 + y^2


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

