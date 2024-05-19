#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = { 0.f, 0.f, -5.f};
vec3_t cube_rotation = { .x = 0.f, .y = 0.f, .z = 0.f };

const float fov_factor = 640.f;

bool is_running = false;
Uint32 previous_frame_time = 0;

void setup(void) {
    color_buffer = (uint32_t*)SDL_malloc(sizeof(uint32_t) * window_width * window_height);    
    if (!color_buffer) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error allocating color buffer.\n");
    }

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    // Fill the cube_points array
    int point_count = 0;
    for (float x = -1.f; x <= 1.f; x += .25f) {
        for (float y = -1.f; y <= 1.f; y += .25f) {
            for (float z = -1.f; z <= 1.f; z += .25f) {
                vec3_t new_point = { .x = x, .y = y, .z = z };
                cube_points[point_count] = new_point;
                ++point_count;
            }
        }
    }
}

void process_input(void) {
    SDL_Event event;

    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
        break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
            }
        break;
    }
}

vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };

    return projected_point;
}

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;

    for (int i = 0; i < N_POINTS; ++i) {
        vec3_t point = cube_points[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        transformed_point.z -= camera_position.z;
        projected_points[i] = project(transformed_point);
    }
}

void render(void) {
    draw_grid();
    
    for (int i = 0; i < N_POINTS; ++i) {
        vec2_t projected_point = projected_points[i];
        draw_rectangle(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFFFFF00
        );
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}