#pragma once

#define int32 int

struct CanvasContext {

    size_t width;
    size_t height;
    size_t screen_buffer_size;
    int32* screen_buffer;

};
