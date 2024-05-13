#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

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

void destroy_window(void) {
    SDL_free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_pixel(int x, int y, uint32_t color) {
    if ((x < window_width && y < window_height) && (x >= 0 && y >= 0)) {
        color_buffer[(y * window_width) + x] = color;
    }    
}

void draw_grid(void) {
    const uint32_t gray_color = 0xFF555555;

    for (int x = 7; x < window_width; x+=10) {
        for (int y = 9; y < window_height; y+=10) {            
            draw_pixel(x, y, gray_color);
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    for (int w = x; w <= width; ++w) {
        for (int h = y; h <= height; ++h) {            
            draw_pixel(w, h, color);
        }
    }
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