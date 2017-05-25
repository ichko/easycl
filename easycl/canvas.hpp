#pragma once

#include "canvas-context.hpp"


struct Canvas {

    CanvasContext context;

    virtual Canvas& Render() = 0;

    Canvas() {
        context.screen_buffer = nullptr;
    }

    Canvas& SetSize(size_t width, size_t height) {
        Cleanup();
        context.width = width;
        context.height = height;
        context.screen_buffer_size = width * height;
        context.screen_buffer = new int32[context.screen_buffer_size];

        return *this;
    }

    void Cleanup() {
        if (context.screen_buffer != nullptr) {
            delete[] context.screen_buffer;
        }
    }

};

struct ImageFile : public Canvas {

    Canvas& Render() {
        
    }

};
