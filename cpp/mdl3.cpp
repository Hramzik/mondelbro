

//--------------------------------------------------

#include "../hpp/mdl2.hpp"

//--------------------------------------------------


// version 2

Pixel_color* calculate_mdl_colors (void* result_void_ptr, double x0, double y0, size_t width, size_t height, double unit_distance) { // point = top-left

    Pixel_color* result = (Pixel_color*) result_void_ptr;


    size_t point_number = 0;

    for (size_t num_y = 0; num_y < height; num_y++) {

        for (size_t num_x = 0; num_x < width; num_x += 4) {

            double x = x0 + (double) num_x * unit_distance;
            double y = y0 - (double) num_y * unit_distance;

            size_t amount = width - num_x; if (amount > 4) amount = 4;

            calculate_mdl_color_ver2 ( { x, y }, unit_distance, result + point_number, amount);

            point_number += amount;
        }
    }


    return result;
}

Return_code calculate_mdl_color_ver2 (Point point, double unit_distance, Pixel_color* result, size_t amount) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Point bases [4] = { point, { point.x + unit_distance,     point.y },
                               { point.x + unit_distance * 2, point.y },
                               { point.x + unit_distance * 3, point.y } };

    Point  curent_coords [4] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }};
    double temp1         [4] = { 0, 0, 0, 0 };
    double temp2         [4] = { 0, 0, 0, 0 };
    size_t op_nums       [4] = { 1, 1, 1, 1 };
    double rs_squared    [4] = { 0, 0, 0, 0 };
    bool   finished      [4] = { false, false, false, false };
    bool   finished_all      = false;


    do {

        for (size_t i = 0; i < amount; i++) temp1 [i] = curent_coords [i].x; // save x coord


        for (size_t i = 0; i < amount; i++) curent_coords [i].x *= curent_coords [i].x; // xn+1
        for (size_t i = 0; i < amount; i++) temp2         [i]    = curent_coords [i].y; // temp2 = y*y
        for (size_t i = 0; i < amount; i++) temp2         [i]   *= curent_coords [i].y;
        for (size_t i = 0; i < amount; i++) curent_coords [i].x -= temp2 [i];
        for (size_t i = 0; i < amount; i++) curent_coords [i].x += bases [i].x;

        for (size_t i = 0; i < amount; i++) curent_coords [i].y *= 2;         // yn+1
        for (size_t i = 0; i < amount; i++) curent_coords [i].y *= temp1 [i]; // *= xn
        for (size_t i = 0; i < amount; i++) curent_coords [i].y += bases [i].y;


        for (size_t i = 0; i < amount; i++) rs_squared [i]  = curent_coords [i].x; // r^2 = x^2
        for (size_t i = 0; i < amount; i++) rs_squared [i] *= curent_coords [i].x;
        for (size_t i = 0; i < amount; i++) temp1      [i]  = curent_coords [i].y; // y^2
        for (size_t i = 0; i < amount; i++) temp1      [i] *= curent_coords [i].y;
        for (size_t i = 0; i < amount; i++) rs_squared [i] += temp1         [i]; // r^2 = x^2 + y^2


        for (size_t i = 0; i < amount; i++) { // r^2 > 4?

            if (rs_squared [i] >  CRITICAL_POINT_RADIUS_SQUARED) finished [i] = true;
            if (op_nums    [i] >= CRITICAL_NUMBER_OF_OPERATIONS) finished [i] = true;

            if (!finished [i]) op_nums [i] += 1;
        }


        finished_all = true;

        for (size_t i = 0; i < amount; i++) { // finished all?

            finished_all &= finished [i];
        }

    } while (!finished_all);


    //--------------------------------------------------

    //printf ("op nums: ");
    //for (size_t i = 0; i < amount; i++) printf ("%zd ", op_nums [i]);
    //printf ("\n");
    double coefficients [4] = { 0, 0, 0, 0 };

    for (size_t i = 0; i < amount; i++) {
        coefficients [i] = (double) op_nums [i] / (double) CRITICAL_NUMBER_OF_OPERATIONS;
 //printf ("%zd = %lf\n", i, coefficients [i]);
    }


    unsigned char red [4] = { 0, 0, 0, 0 };

    for (size_t i = 0; i < amount; i++) {

        red [i] = (unsigned char) (255 * coefficients [i]);
    }


    unsigned char green [4] = { 0, 0, 0, 0 };

    for (size_t i = 0; i < amount; i++) {

        green [i] = (unsigned char) (255 * coefficients [i]);
    }


    unsigned char blue [4] = { 0, 0, 0, 0 };

    for (size_t i = 0; i < amount; i++) {

        blue [i] = (unsigned char) (255 * coefficients [i]);
    }


    Pixel_color* results [4] = { result, result, result, result };

    for (size_t i = 0; i < amount; i++) {

        results [i] += i;
    }


    for (size_t i = 0; i < amount; i++) {

        results [i]->red   = red [i];
        results [i]->green = green [i];
        results [i]->blue  = blue [i];
    }


    return SUCCESS;
}


Return_code set (double)