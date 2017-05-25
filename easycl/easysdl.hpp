#pragma once

#include <time.h>
#include <chrono>
#include <ctime>

#include <SDL.h>

#include "renderer.hpp"


struct EasySDL : public Renderer {

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Event event;

    float timer;
    clock_t program_start;
    float delta_t;
    clock_t frame_start;
    int fps;

    ScreenContext screen_context;

    void Render() {
        
    }

    EasySDL& SetWindowFullscreen() {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        return SetWindow(dm.w, dm.h);
    }

    bool KeyDown(int key_type) {
        return event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == key_type;
    }

    EasySDL& SetWindow(size_t w, size_t h, bool full_screen = false) {
        screen_context.width = w;
        screen_context.height = h;
        screen_context.screen_buffer_size = w * h;
        screen_context.screen_buffer = new Uint32[screen_context.screen_buffer_size];

        SDL_CreateWindowAndRenderer(int(w), int(h), 0, &window, &renderer);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, int(w), int(h));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetWindowFullscreen(window, full_screen);


        return *this;
    }

    EasySDL& Init() {
        program_start = clock();
        frame_start = clock();
        delta_t = 0.0f;
        fps = 0;
        event = SDL_Event();
        SDL_Init(SDL_INIT_VIDEO);
        return *this;
    }

    EasySDL& Render() {
        SDL_UpdateTexture(texture, NULL, screen_context.screen_buffer,
                          int(screen_context.width * sizeof(Uint32)));
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
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
        delete[] screen_context.screen_buffer;
        SDL_Quit();
    }

    ~EasySDL() {
        // Destroy();
    }

};
