#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_running = false;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 0;
int window_height = 0;

bool initialize_window(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {        
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Error creating SDL window.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {        
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error creating SDL renderer.\n");
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

void destroy_window(void) {
    SDL_free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
    SDL_memset4(color_buffer, color, window_width * window_height);
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);

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