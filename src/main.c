#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {0.f, 0.f, -5.f};
const float fov_factor = 640.f;

bool is_running = false;
Uint32 previous_frame_time = 0;

void setup(void) {
    color_buffer =
        (uint32_t*)SDL_malloc(sizeof(uint32_t) * window_width * window_height);
    if (!color_buffer) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Error allocating color buffer.\n");
    }

    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width, window_height);

    // Loads the cube values in the mesh data structure
    load_obj_file_data("../assets/cube.obj");
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
    vec2_t projected_point = {.x = (fov_factor * point.x) / point.z,
                              .y = (fov_factor * point.y) / point.z};

    return projected_point;
}

void update(void) {
    int time_to_wait =
        FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    // Initialize the array of triangles to render
    triangles_to_render = NULL;

    mesh.rotation.x += 0.01f;
    mesh.rotation.y += 0.01f;
    mesh.rotation.z += 0.01f;

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; ++i) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        triangle_t projected_triangle;

        // Loop all three vertices of this current face and apply
        // transformations
        for (int j = 0; j < 3; ++j) {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex =
                vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex =
                vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex =
                vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertex away from the camera
            transformed_vertex.z -= camera_position.z;

            // Project the current vertex
            vec2_t projected_point = project(transformed_vertex);

            // Scale and translate the projected points to the middle of the
            // screen
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);

            projected_triangle.points[j] = projected_point;
        }

        // Save the projected triangle in the array of triangles to render
        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void) {
    draw_grid();

    // Loop all projected triangles and render them
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; ++i) {
        triangle_t triangle = triangles_to_render[i];

        // Draw unfilled triangle
        draw_triangle(triangle.points[0].x, triangle.points[0].y,
                      triangle.points[1].x, triangle.points[1].y,
                      triangle.points[2].x, triangle.points[2].y, 0xFFFFFF00);

        // Draw vertex points
        draw_rectangle(triangle.points[0].x, triangle.points[0].y, 4, 4,
                       0xFFFF0000);
        draw_rectangle(triangle.points[1].x, triangle.points[1].y, 4, 4,
                       0xFFFF0000);
        draw_rectangle(triangle.points[2].x, triangle.points[2].y, 4, 4,
                       0xFFFF0000);
    }

    array_free(triangles_to_render);

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

// Free the memory that was dynamically allocated by the program
void free_resources(void) {
    SDL_free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

#define DISCARD_UNUNSED_PARAMETER(x) (void)x

int main(int argc, char* argv[]) {
    DISCARD_UNUNSED_PARAMETER(argc);
    DISCARD_UNUNSED_PARAMETER(argv);

    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

    return 0;
}
