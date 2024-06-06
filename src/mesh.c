#include <stdio.h>
#include <string.h>

#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_log.h>

#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { .0f, .0f, .0f }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 },
    { .x = -1, .y =  1, .z = -1 },
    { .x =  1, .y =  1, .z = -1 },
    { .x =  1, .y = -1, .z = -1 },
    { .x =  1, .y =  1, .z =  1 },
    { .x =  1, .y = -1, .z =  1 },
    { .x = -1, .y =  1, .z =  1 },
    { .x = -1, .y = -1, .z =  1 }
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

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

int count_lines(char *buffer) {
    const char ln = '\n';
    const char zero = '\0';

    int count = 0;
    for (char *c = buffer; *c != zero; ++c) {
        if ((*c) == ln) {
            count++;
        }
    }

    return count;
}

typedef struct {
    char *begin;
    int length;
} slice_t;

void slice_string(char *buffer, slice_t *lines) {
    const char zero = '\0';

    int line = 0;
    int length = 0;
    for (char *c = buffer; *c != zero; ++c) {
        if (*c == '\n') {
            lines[ line ].begin = c;
            lines[ line ].length = length;
            length = 0;
            line++;
        } else {
            length++;
        }
    }
}

void parse_obj(char *buffer, size_t size) {

    SDL_strtokr(); // !!!!!

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        size_t len = SDL_strlen(line);
        char copy_line[len];
        SDL_strlcpy(line, copy_line, len);
        char *token = strtok(copy_line, " ");
        while (token != NULL) {
            SDL_LogDebug(SDL_LOG_CATEGORY_CUSTOM, "Token: %s", token);
            token = strtok(copy_line, " ");
        }
        line = strtok(buffer, "\n");
    }
}

void load_obj_file_data(char *filename) {
    SDL_RWops* file = SDL_RWFromFile(filename, "r");
    if (file) {        
        SDL_LogDebug(SDL_LOG_CATEGORY_CUSTOM, "Filename: %s, File size: %llu\n", filename, SDL_RWsize(file));

        // Read file to buffer
        size_t file_size;
        char *buffer = (char*)SDL_LoadFile_RW(file, &file_size, 1);

        parse_obj(buffer, file_size);

        if (buffer) {
            SDL_free(buffer);
        }
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_CUSTOM, "%s\n", SDL_GetError());        
    }
}