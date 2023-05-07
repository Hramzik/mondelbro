#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED
//--------------------------------------------------


#include "../lib/types/Return_code.hpp"

#define  SDL_MAIN_HANDLED // no argc, argv
#include <SDL2/SDL.h> // when compiling, gcc will find it via -I key
#include "../../../../lib/SDL2/include/SDL2/SDL.h" // for VSC to highlight everything before compile time
#include "../../../../lib/SDL2/include/SDL2/SDL_image.h"
#include "../../../../lib/SDL2/include/SDL2/SDL_mixer.h"
#include "../../../../lib/SDL2/include/SDL2/SDL_ttf.h"

#include <stdio.h>
#include <time.h>


//---------------- settings ------------------------


#define PICTURE_PATH "media/blade1.jpg"
#define MUSIC_PATH   "media/nightcall.mp3"
#define FONT_PATH    "media/font.ttf"

const size_t MDL_WINDOW_WIDTH  = 480;
const size_t MDL_WINDOW_HEIGHT = 360;
const size_t MDL_PIXEL_COUNT   = MDL_WINDOW_WIDTH * MDL_WINDOW_HEIGHT;


const double MDL_ZOOM_FACTOR = 1.05; // > 1
const double MDL_MOVE_FACTOR = 0.02; // 1 == move by full width / height

const size_t PICTURE_GENERATIONS_COUNT = 1;

//--------------------------------------------------

const Uint32 MDL_OUTPUT_FLAGS = SDL_WINDOW_SHOWN | /*SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR |
    SDL_WINDOW_INPUT_GRABBED*/ SDL_WINDOW_ALWAYS_ON_TOP;

const size_t SIZE_T_SIZE = sizeof (size_t);

//--------------------------------------------------


typedef struct Mdl_data {

    double unit_distance;
    double x_left;
    double y_top;

} Mdl_data; const size_t MDL_DATA_SIZE = sizeof (Mdl_data);


typedef struct Pixel_color {

    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char transparency;

} Pixel_color; const size_t PIXEL_COLOR_SIZE = sizeof (Pixel_color);


typedef struct Mdl_picture {

    Pixel_color* pixels; // stored by lines

} Mdl_picture; const size_t MDL_PICTURE_SIZE = sizeof (Mdl_picture);


typedef struct Mdl_output {

    SDL_Window* window;

    SDL_Renderer* renderer;

} Mdl_output; const size_t MDL_OUTPUT_SIZE = sizeof (Mdl_output);


typedef struct Mdl_conditions {

    bool exit;

    bool move_right;
    bool move_left;
    bool move_up;
    bool move_down;

    bool zoom_in;
    bool zoom_out;

} Mdl_conditions; const size_t Mdl_conditions_SIZE = sizeof (Mdl_conditions);



//--------------------------------------------------


typedef struct Mdl {

    Mdl_output* output;

    Mdl_picture picture;

    Mdl_data data;

    Mdl_conditions conditions;

} Mdl; const size_t MDL_SIZE = sizeof (Mdl);


//--------------------------------------------------


typedef struct Fps_handler {

    clock_t cur_frame_time;
    clock_t old_frame_time;

    size_t frame_number;

    double sum_delays_ms;
    double sum_fps;

} Fps_handler; const size_t FPS_HANDLER_SIZE = sizeof (Fps_handler);


//--------------------------------------------------

extern Pixel_color* calculate_mdl_colors_ver1 (void* result, double x0, double y0, size_t width, size_t height, double unit_distance);
extern Pixel_color* calculate_mdl_colors_ver2 (void* result, double x0, double y0, size_t width, size_t height, double unit_distance);
extern Pixel_color* calculate_mdl_colors_ver3 (void* result, double x0, double y0, size_t width, size_t height, double unit_distance);
extern Pixel_color* calculate_mdl_colors_ver4 (void* result, double x0, double y0, size_t width, size_t height, double unit_distance);


//--------------------------------------------------


Mdl*        mdl_ctor                 (void);
Return_code mdl_set_start_data       (Mdl* mdl);
Return_code mdl_set_start_conditions (Mdl* mdl);

Return_code mdl_dtor             (Mdl* mdl);


Return_code mdl_work                      (Mdl* mdl);
Return_code mdl_update_conditions         (Mdl* mdl);
Return_code mdl_update_conditions_keydown (Mdl* mdl, SDL_Event event);
Return_code mdl_update_conditions_keyup   (Mdl* mdl, SDL_Event event);
Return_code mdl_handle_conditions         (Mdl* mdl);
Return_code mdl_zoom               (Mdl* mdl, double new_unit_distance);


Return_code mdl_generate_picture (Mdl* mdl);
Return_code mdl_render_picture   (Mdl* mdl);
Return_code mdl_render_pixel     (Mdl* mdl, size_t x, size_t y);
Pixel_color mdl_get_pixel_color  (Mdl* mdl, size_t x, size_t y);


Mdl_output* mdl_output_ctor (void);
Return_code mdl_output_dtor (Mdl_output* graphics);


Return_code initialize_sdl (void);


double my_min (double val1, double val2);

//--------------------------------------------------

Fps_handler* fps_handler_ctor          (void);
Return_code  fps_handler_dtor          (Fps_handler* handler);
Return_code  fps_handler_next_frame    (Fps_handler* handler);
double       fps_handler_get_delay_ms  (Fps_handler* handler);
double       fps_handler_get_fps       (Fps_handler* handler);
double       fps_handler_get_avg_delay (Fps_handler* handler);
double       fps_handler_get_avg_fps   (Fps_handler* handler);

//--------------------------------------------------

Return_code mdl_render_picture_v2        (Mdl* mdl);
SDL_Surface* mdl_generate_result_surface (Mdl* mdl);
SDL_Texture* mdl_generate_result_texture (Mdl* mdl);

//--------------------------------------------------
#endif