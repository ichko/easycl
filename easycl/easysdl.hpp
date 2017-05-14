#pragma once

#include <time.h>
#include <chrono>
#include <ctime>

#include <SDL.h>


struct EasySDL {

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Event event;

    size_t width;
    size_t height;
    size_t screen_buffer_size;
    Uint32* screen_buffer;

    float timer;
    clock_t program_start;
    float delta_t;
    clock_t frame_start;
    int fps;

    EasySDL& set_window_full_screen() {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        return set_window(dm.w, dm.h);
    }

    bool key_down(int key_type) {
        return event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == key_type;
    }

    EasySDL& set_window(size_t w, size_t h, bool full_screen = false) {
        width = w;
        height = h;
        screen_buffer_size = w * h;
        screen_buffer = new Uint32[screen_buffer_size];

        SDL_CreateWindowAndRenderer(int(w), int(h), 0, &window, &renderer);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, int(w), int(h));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetWindowFullscreen(window, full_screen);


        return *this;
    }

    EasySDL& init() {
        program_start = clock();
        frame_start = clock();
        delta_t = 0.0f;
        fps = 0;
        event = SDL_Event();
        SDL_Init(SDL_INIT_VIDEO);
        return *this;
    }

    EasySDL& render() {
        SDL_UpdateTexture(texture, NULL, screen_buffer, int(width * sizeof(Uint32)));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        return *this;
    }

    float get_time() {
        return (clock() - program_start) / 1000.0f;
    }

    EasySDL& set_title(std::string title = "") {
        SDL_SetWindowTitle(window, title.c_str());
        return *this;
    }

    EasySDL& tick() {
        timer = get_time();
        delta_t = (clock() - frame_start) / 1000.0f;
        frame_start = clock();
        fps = int(1000 / (delta_t * 1000));
        SDL_PollEvent(&event);

        return *this;
    }

    void destroy() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
        delete[] screen_buffer;
        SDL_Quit();
    }

};
