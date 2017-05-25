#pragma once

#include <time.h>
#include <chrono>
#include <ctime>

#include <SDL.h>

#include "canvas.hpp"


struct EasySDL : public Canvas {

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Event event;

    float timer;
    clock_t program_start;
    float delta_t;
    clock_t frame_start;
    int fps;

    EasySDL() {
        program_start = clock();
        frame_start = clock();
        delta_t = 0.0f;
        fps = 0;
        event = SDL_Event();
        SDL_Init(SDL_INIT_VIDEO);
    }

    EasySDL& SetWindowFullscreen() {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        return SetSize(dm.w, dm.h);
    }

    bool KeyDown(int key_type) {
        return event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == key_type;
    }


    EasySDL& SetSize(size_t width, size_t height) {
        Canvas::SetSize(width, height);
        SDL_CreateWindowAndRenderer(int(width), int(height), 0, &window, &renderer);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, int(width), int(height));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetWindowFullscreen(window, false);

        return *this;
    }

    EasySDL& Render() {
        SDL_UpdateTexture(texture, NULL, context.screen_buffer,
                          int(context.width * sizeof(Uint32)));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        return *this;
    }

    float GetTime() {
        return (clock() - program_start) / 1000.0f;
    }

    EasySDL& SetTitle(std::string title = "") {
        SDL_SetWindowTitle(window, title.c_str());
        return *this;
    }

    EasySDL& Tick() {
        timer = GetTime();
        delta_t = (clock() - frame_start) / 1000.0f;
        frame_start = clock();
        fps = int(1000 / (delta_t * 1000));
        SDL_PollEvent(&event);

        return *this;
    }

    void Destroy() {
        Cleanup();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
        SDL_Quit();
    }

    ~EasySDL() {
        // Destroy();
    }

};
