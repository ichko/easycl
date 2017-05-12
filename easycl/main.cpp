#include "easycl.hpp"

struct Color {
    cl_float r, g, b;
};

int main() {
    const int width = 10;
    const int height = 10;
    const int pixels_cnt = width * height;
    Color screen_buffer[width * height];

    auto context = EasyCL()
        .load_device(0, 1)
        .load_src("shade.cl")
        .load_kernel("init")
        .set_arg(0, screen_buffer, pixels_cnt)
        .set_arg(1, &width)
        .set_arg(2, &height)
        .run(pixels_cnt)
        .read_buffer(0, screen_buffer, pixels_cnt);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            std::cout << "[" <<
                screen_buffer[i * j].r << "," <<
                screen_buffer[i * j].g << "," <<
                screen_buffer[i * j].b << "]";
        }
        std::cout << std::endl;
    }

    return 0;
}