#include "easycl.hpp"
#include "easysdl.hpp"


int main(int argc, char** argv) {
    auto easysdl = EasySDL()
        .init()
        //.set_window(500, 500);
        .set_window_full_screen();

    auto easycl = EasyCL()
        .load_device(0, 1)
        .load_src("shader.cl")
        .load_kernel("start")
        .set_arg(0, easysdl.screen_buffer, easysdl.screen_buffer_size)
        .set_arg(1, &easysdl.width)
        .set_arg(2, &easysdl.height)
        .set_arg(3, &easysdl.timer)
        .run(easysdl.screen_buffer_size)
        .read_buffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);

    while (!easysdl.key_down(SDLK_ESCAPE)) {
        easycl
            .update_arg(3, &easysdl.timer)
            .run(easysdl.screen_buffer_size)
            .read_buffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);

        easysdl
            .tick()
            .render();
    }
    easysdl.destroy();

    return 0;
}