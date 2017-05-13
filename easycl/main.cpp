#include "easycl.hpp"
#include "sdl.hpp"
#include <time.h>
#include <chrono>
#include <ctime>

clock_t tStart = clock();
float get_time() {
    return (clock() - tStart) / 2000.0;
}

int main(int argc, char** argv) {
    const size_t width = 600;
    const size_t height = 600;
    const size_t pixels_cnt = width * height;
    cl_float3* screen_buffer = new cl_float3[width * height];

    auto sdl_context = SDLContext().init(width, height);
    float time = get_time();

    auto cl = EasyCL()
        .load_device(0, 1)
        .load_src("shader.cl")
        .load_kernel("start")
        .set_arg(0, screen_buffer, pixels_cnt)
        .set_arg(1, &width)
        .set_arg(2, &height)
        .set_arg(3, &time)
        .run(pixels_cnt)
        .read_buffer(0, screen_buffer, pixels_cnt);

    while (true) {
        time = get_time();
        cl
            .update_arg(3, &time)
            .run(pixels_cnt)
            .read_buffer(0, screen_buffer, pixels_cnt);
        sdl_context.render(screen_buffer);
    }
    sdl_context.destroy();
    delete[] screen_buffer;

    //for (size_t i = 0; i < width; i++) {
    //    for (size_t j = 0; j < height; j++) {
    //        std::cout << "[" <<
    //            screen_buffer[i * j].x << "," <<
    //            screen_buffer[i * j].y << "," <<
    //            screen_buffer[i * j].z << "]";
    //    }
    //    std::cout << std::endl;
    //}

    return 0;
}