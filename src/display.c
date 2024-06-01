#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 0;
int window_height = 0;

bool initialize_window(void) {    
    SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_DEBUG);

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
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void destroy_window(void) {    
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
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    // DDA algorithm
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = SDL_abs(delta_x) >= SDL_abs(delta_y) ? SDL_abs(delta_x) : SDL_abs(delta_y);

    // Find how much we should increment in both x and y each step
    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; ++i) {
        draw_pixel( (int)SDL_roundf(current_x), (int)SDL_roundf(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
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