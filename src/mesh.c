#include "mesh.h"

#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rwops.h>
#include <stdio.h>
#include <string.h>

#include "array.h"

mesh_t mesh = {.vertices = NULL, .faces = NULL, .rotation = {.0f, .0f, .0f}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, {.x = -1, .y = 1, .z = -1},
    {.x = 1, .y = 1, .z = -1},   {.x = 1, .y = -1, .z = -1},
    {.x = 1, .y = 1, .z = 1},    {.x = 1, .y = -1, .z = 1},
    {.x = -1, .y = 1, .z = 1},   {.x = -1, .y = -1, .z = 1}};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4}};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; ++i) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; ++i) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void parse_obj(char *buffer) {
    char *line = NULL;
    char *endline = buffer;

    // Parse every line
    while ((line = SDL_strtokr(endline, "\n", &endline))) {
        char *token = NULL;
        char *endtoken = line;

        token = SDL_strtokr(endtoken, " ", &endtoken);

        // Get the vertexes row
        if (SDL_strncmp(token, "v", 255) == 0) {
            char *v1 = SDL_strtokr(endtoken, " ", &endtoken);
            char *v2 = SDL_strtokr(endtoken, " ", &endtoken);
            char *v3 = SDL_strtokr(endtoken, " ", &endtoken);

            vec3_t vec = {.x = SDL_strtod(v1, NULL),
                          .y = SDL_strtod(v2, NULL),
                          .z = SDL_strtod(v3, NULL)};

            SDL_LogDebug(SDL_LOG_CATEGORY_CUSTOM, "x: %f, y: %f, z: %f", vec.x,
                         vec.y, vec.z);
        }

        // Get the faces row
        if (SDL_strncmp(token, "f", 255) == 0) {
            // Get 3 index groups, eg. 1/1/1
            char *f1 = SDL_strtokr(endtoken, " ", &endtoken);
            char *f2 = SDL_strtokr(endtoken, " ", &endtoken);
            char *f3 = SDL_strtokr(endtoken, " ", &endtoken);

            // Get the first number in the index group, this is Vertex index
            char *a = SDL_strtokr(f1, "/", &f1);
            char *b = SDL_strtokr(f2, "/", &f2);
            char *c = SDL_strtokr(f3, "/", &f3);

            face_t face = {
                .a = SDL_atoi(a), .b = SDL_atoi(b), .c = SDL_atoi(c)};

            SDL_LogDebug(SDL_LOG_CATEGORY_CUSTOM, "a: %d, b: %d, c: %d", face.a,
                         face.b, face.c);
        }
    }
}

void load_obj_file_data(char *filename) {
    SDL_RWops *file = SDL_RWFromFile(filename, "r");
    if (file) {
        SDL_LogDebug(SDL_LOG_CATEGORY_CUSTOM, "Filename: %s, File size: %llu\n",
                     filename, SDL_RWsize(file));

        // Read file to buffer
        size_t file_size;
        char *buffer = (char *)SDL_LoadFile_RW(file, &file_size, 1);

        parse_obj(buffer);

        if (buffer) {
            SDL_free(buffer);
        }
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_CUSTOM, "%s\n", SDL_GetError());
    }
}
