#pragma once

#include <fstream>

#include "canvas-context.hpp"
#include "utils.hpp"


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
        context.screen_buffer = new Uint32[context.screen_buffer_size];

        return *this;
    }

    void Cleanup() {
        if (context.screen_buffer != nullptr) {
            delete[] context.screen_buffer;
        }
    }

};

struct ImageCanvas : public Canvas {

    std::ofstream image;
    std::string file_name;

    ImageCanvas(size_t width, size_t height,  std::string _file_name) {
        SetSize(width, height);
        file_name = _file_name;
    }

    Canvas& Render() {
        image.open(file_name);
        image << "P3" << std::endl;
        image << context.width << " " << context.height << std::endl;
        image << 255 << std::endl;
        for (size_t y = 0; y < context.height; y++) {
            for (size_t x = 0; x < context.width; x++) {
                int pixel = context.screen_buffer[y * context.width + x];
                int r = 0x000000ff & pixel;
                int g = (0x0000ff00 & pixel) >> 8;
                int b = (0x00ff0000 & pixel) >> 16;
                image << r << " " << g << " " << b << std::endl;
            }
        }
        image.close();

        return *this;
    }

};
