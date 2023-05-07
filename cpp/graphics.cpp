

//--------------------------------------------------
#include "../hpp/graphics.hpp"

#include "../lib/logs.cpp"
//--------------------------------------------------


Mdl* mdl_ctor (void) {

    Mdl* mdl = (Mdl*) calloc (1, MDL_SIZE);


    try_return (initialize_sdl (), nullptr);


    mdl->output = mdl_output_ctor ();


    Pixel_color* mdl_pixels = (Pixel_color*) calloc (MDL_PIXEL_COUNT, SIZE_T_SIZE);
    mdl->picture = { .pixels = mdl_pixels };


    mdl_set_start_data (mdl);


    mdl_set_start_conditions (mdl);


    return mdl;
}


Return_code initialize_sdl (void) {

    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){

        LOG_MESSAGE ("SDL could not initialize!\n");
        return LIB_ERR;
    }


    if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1")) {

        LOG_MESSAGE ("Error: Linear texture filtering not enabled!");
        return LIB_ERR;
    }


    return SUCCESS;
}


Mdl_output* mdl_output_ctor (void) {


    Mdl_output* graphics = (Mdl_output*) calloc (1, MDL_OUTPUT_SIZE);


    graphics->window = SDL_CreateWindow ("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MDL_WINDOW_WIDTH, MDL_WINDOW_HEIGHT, MDL_OUTPUT_FLAGS); // empty header

    if(!graphics->window) {

        LOG_MESSAGE ("Window could not be created!\n");
        return nullptr;
    }


    graphics->renderer = SDL_CreateRenderer(graphics->window, -1, SDL_RENDERER_ACCELERATED);
    if (!graphics->renderer) {

        LOG_MESSAGE ("Renderer could not be created!");
        return nullptr;
    }


    return graphics;
}


Return_code mdl_output_dtor (Mdl_output* graphics) {

    if (!graphics) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_DestroyRenderer (graphics->renderer);
    SDL_DestroyWindow   (graphics->window);


    free (graphics);


    return SUCCESS;
}


Return_code mdl_dtor (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mdl_output_dtor     (mdl->output);


    free (mdl);


    TTF_Quit ();
    Mix_CloseAudio ();
    Mix_Quit ();
    IMG_Quit ();
    SDL_Quit ();


    return SUCCESS;
}


Return_code mdl_work (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Fps_handler* fps_counter = fps_handler_ctor ();


    while(true) {

    //--------------------------------------------------
        mdl_update_conditions (mdl);

        if (mdl->conditions.exit) break;

        mdl_handle_conditions (mdl);
    //--------------------------------------------------


        for (size_t i = 0; i < PICTURE_GENERATIONS_COUNT; i++) {

            mdl_generate_picture (mdl);
        }


        SDL_RenderClear       (mdl->output->renderer);
        mdl_render_picture_v2 (mdl);


        SDL_RenderPresent (mdl->output->renderer);


        fps_handler_next_frame (fps_counter);



        if (fps_counter->frame_number == 1 || fps_counter->frame_number == 10 || fps_counter->frame_number == 1000) {

            printf ("cur fps = %10lf, cur delay = %lf\n", fps_handler_get_fps (fps_counter), fps_handler_get_delay_ms (fps_counter));
            printf ("frame number = %zd, avg fps = %10lf, avg delay = %lf\n", fps_counter->frame_number, fps_handler_get_avg_fps  (fps_counter), fps_handler_get_avg_delay (fps_counter));
        }
    }


    fps_handler_dtor (fps_counter);


    return SUCCESS;
}


Return_code mdl_generate_picture (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    double x0 = mdl->data.x_left;
    double y0 = mdl->data.y_top;


    calculate_mdl_colors_ver4 (mdl->picture.pixels, x0, y0, MDL_WINDOW_WIDTH, MDL_WINDOW_HEIGHT, mdl->data.unit_distance);


    return SUCCESS;
}


Return_code mdl_render_picture (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (size_t x = 0; x < MDL_WINDOW_WIDTH; x++) {

        for (size_t y = 0; y < MDL_WINDOW_HEIGHT; y++) {

            mdl_render_pixel (mdl, x, y);
        }
    }


    return SUCCESS;
}


//--------------------------------------------------

#define MY_RENDERER mdl->output->renderer

//--------------------------------------------------

Return_code mdl_render_pixel (Mdl* mdl, size_t x, size_t y) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Pixel_color color = mdl_get_pixel_color (mdl, x, y);


    SDL_SetRenderDrawColor (MY_RENDERER, color.red, color.green, color.blue, color.transparency);
    SDL_RenderDrawPoint    (MY_RENDERER, (int) x, (int) y);


    return SUCCESS;
}


Pixel_color mdl_get_pixel_color (Mdl* mdl, size_t x, size_t y) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return { 0, 0, 0, 255 }; }


    size_t index = y * MDL_WINDOW_WIDTH + x;


    return mdl->picture.pixels [index];
}


Return_code mdl_set_start_data (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mdl->data.unit_distance = 4 / my_min (MDL_WINDOW_WIDTH, MDL_WINDOW_HEIGHT);


    mdl->data.x_left = -1 * ((double) MDL_WINDOW_WIDTH   / 2) * mdl->data.unit_distance;
    mdl->data.y_top  =      ((double) MDL_WINDOW_HEIGHT  / 2) * mdl->data.unit_distance;


    return SUCCESS;
}


Return_code mdl_zoom (Mdl* mdl, double new_unit_distance) { // updates coords when unit_distance changed

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mdl->data.x_left -= -1 * ((double) MDL_WINDOW_WIDTH   / 2) * (mdl->data.unit_distance - new_unit_distance);
    mdl->data.y_top  -=      ((double) MDL_WINDOW_HEIGHT  / 2) * (mdl->data.unit_distance - new_unit_distance);


    mdl->data.unit_distance = new_unit_distance;


    return SUCCESS;
}


Return_code mdl_set_start_conditions (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mdl->conditions.exit       = false;

    mdl->conditions.move_left  = false;
    mdl->conditions.move_right = false;
    mdl->conditions.move_up    = false;
    mdl->conditions.move_down  = false;

    mdl->conditions.zoom_in    = false;
    mdl->conditions.zoom_out   = false;


    return SUCCESS;
}


double my_min (double val1, double val2) {

    if (val1 > val2) return val1;


    return val2;
}


Return_code mdl_update_conditions (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Event event = {};


    while (SDL_PollEvent (&event)) {

        if (event.type == SDL_QUIT) {

            mdl->conditions.exit = true;
            break;
        }


        if (event.type == SDL_KEYDOWN) mdl_update_conditions_keydown (mdl, event);


        if (event.type == SDL_KEYUP)   mdl_update_conditions_keyup   (mdl, event);
    }


    return SUCCESS;
}


Return_code mdl_update_conditions_keydown (Mdl* mdl, SDL_Event event) {

    if (!mdl)                      { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (event.type != SDL_KEYDOWN) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    switch (event.key.keysym.sym) {

        case SDLK_UP:
        case SDLK_w:        mdl->conditions.move_up    = true; break;
        case SDLK_LEFT:
        case SDLK_a:        mdl->conditions.move_left  = true; break;
        case SDLK_DOWN:
        case SDLK_s:        mdl->conditions.move_down  = true; break;
        case SDLK_RIGHT:
        case SDLK_d:        mdl->conditions.move_right = true; break;
        case SDLK_KP_PLUS:
        case SDLK_SPACE:    mdl->conditions.zoom_in    = true; break;
        case SDLK_KP_MINUS:
        case SDLK_q:        mdl->conditions.zoom_out   = true; break;

        default: break;
    }


    return SUCCESS;
}


Return_code mdl_update_conditions_keyup (Mdl* mdl, SDL_Event event) {

    if (!mdl)                    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (event.type != SDL_KEYUP) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    switch (event.key.keysym.sym) {

        case SDLK_UP:
        case SDLK_w:        mdl->conditions.move_up    = false; break;
        case SDLK_LEFT:
        case SDLK_a:        mdl->conditions.move_left  = false; break;
        case SDLK_DOWN:
        case SDLK_s:        mdl->conditions.move_down  = false; break;
        case SDLK_RIGHT:
        case SDLK_d:        mdl->conditions.move_right = false; break;
        case SDLK_KP_PLUS:
        case SDLK_SPACE:    mdl->conditions.zoom_in    = false; break;
        case SDLK_KP_MINUS:
        case SDLK_q:        mdl->conditions.zoom_out   = false; break;

        default: break;
    }


    return SUCCESS;
}


Return_code mdl_handle_conditions (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (mdl->conditions.zoom_in)   { mdl_zoom (mdl, mdl->data.unit_distance / MDL_ZOOM_FACTOR); /* return SUCCESS; */ }
    if (mdl->conditions.zoom_out)  { mdl_zoom (mdl, mdl->data.unit_distance * MDL_ZOOM_FACTOR); /* return SUCCESS; */ }

    if (mdl->conditions.move_left)  mdl->data.x_left -= MDL_WINDOW_WIDTH  * mdl->data.unit_distance * MDL_MOVE_FACTOR;
    if (mdl->conditions.move_right) mdl->data.x_left += MDL_WINDOW_WIDTH  * mdl->data.unit_distance * MDL_MOVE_FACTOR;
    if (mdl->conditions.move_up)    mdl->data.y_top  += MDL_WINDOW_HEIGHT * mdl->data.unit_distance * MDL_MOVE_FACTOR;
    if (mdl->conditions.move_down)  mdl->data.y_top  -= MDL_WINDOW_HEIGHT * mdl->data.unit_distance * MDL_MOVE_FACTOR;


    return SUCCESS;
}


Fps_handler* fps_handler_ctor (void) {

    Fps_handler* result = (Fps_handler*) calloc (1, FPS_HANDLER_SIZE);


    result->cur_frame_time = clock ();
    result->old_frame_time = 0;


    result->frame_number = 0;


    result->sum_delays_ms = 0;
    result->sum_fps       = 0;


    return result;
}


Return_code fps_handler_dtor (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    free (handler);


    return SUCCESS;
}


Return_code fps_handler_next_frame (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    handler->old_frame_time = handler->cur_frame_time;
    handler->cur_frame_time = clock ();


    handler->frame_number += 1;


    handler->sum_delays_ms += fps_handler_get_delay_ms (handler);
    handler->sum_fps       += fps_handler_get_fps      (handler);


    return SUCCESS;
}


double fps_handler_get_delay_ms (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return (1000 * ( (double) handler->cur_frame_time - (double) handler->old_frame_time) / CLOCKS_PER_SEC);
}


double fps_handler_get_fps (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return 1000 / fps_handler_get_delay_ms (handler);
}


double fps_handler_get_avg_delay (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return handler->sum_delays_ms / (double) handler->frame_number;
}


double fps_handler_get_avg_fps (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return handler->sum_fps / (double) handler->frame_number;
}


//--------------------------------------------------
//    new rendering using surfaces



Return_code mdl_render_picture_v2 (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Texture* result_texture = mdl_generate_result_texture (mdl);


    SDL_RenderCopy (mdl->output->renderer, result_texture, nullptr, nullptr);


    SDL_DestroyTexture (result_texture);


    return SUCCESS;
}


SDL_Surface* mdl_generate_result_surface (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return nullptr; }


    void* buffer = mdl->picture.pixels;

    int width  = MDL_WINDOW_WIDTH;
    int height = MDL_WINDOW_HEIGHT;

    int depth = 32;
    int pitch = width * 4; // 4 = bytes per color

    Uint32 rmask = 0; // default mask
    Uint32 gmask = 0;
    Uint32 bmask = 0;
    Uint32 amask = 0;


    SDL_Surface* result_surface = SDL_CreateRGBSurfaceFrom (buffer, width, height, depth, pitch, 
                                                            rmask,  gmask, bmask,  amask);


    if (!result_surface) {

        LOG_MESSAGE ("Unable to create surface!");
        return nullptr;
    }


    return result_surface;
}


SDL_Texture* mdl_generate_result_texture (Mdl* mdl) {

    if (!mdl) { LOG_ERROR (BAD_ARGS); return nullptr; }


    SDL_Surface* temp_surface = mdl_generate_result_surface (mdl);
    if (!temp_surface) { LOG_ERROR (CRITICAL); return nullptr; }


    SDL_Texture* result_texture = SDL_CreateTextureFromSurface (mdl->output->renderer, temp_surface);
    SDL_FreeSurface (temp_surface);


    if (!result_texture) {

        LOG_MESSAGE ("Unable to create texture!");
        return nullptr;
    }


    return result_texture;
}


