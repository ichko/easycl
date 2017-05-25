#pragma once

#define int32 int

typedef struct {

    size_t width;
    size_t height;
    size_t screen_buffer_size;
    Uint32* screen_buffer;

} CanvasContext;
