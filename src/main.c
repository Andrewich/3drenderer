#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];

bool is_running = false;

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

void update(void) {

}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();
    draw_rectangle(100, 100, 300, 200, 0xFFA00080);

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