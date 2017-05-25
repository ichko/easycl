#pragma once

#include "canvas-context.hpp"


struct Canvas {

    CanvasContext canvas_context;

    virtual Canvas& Init() = 0;

    virtual Canvas& SetWindow(size_t width, size_t height) = 0;

    virtual Canvas& Render() = 0;

};

struct ImageRenderer : public Canvas {

    Canvas& Render() {
        
    }

};
