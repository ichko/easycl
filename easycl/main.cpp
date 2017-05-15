#include "easycl.hpp"
#include "easysdl.hpp"


int main(int argc, char** argv) {
    auto easysdl = EasySDL()
        .Init()
        .SetWindow(512, 512);
        //.set_window_full_screen();

    auto easycl = EasyCL()
        .LoadDevice(0, 0)
        .LoadSrc("shader.cl")
        .LoadKernel("start")
        .SetArg(0, easysdl.screen_buffer, easysdl.screen_buffer_size)
        .SetArg(1, &easysdl.width)
        .SetArg(2, &easysdl.height)
        .SetArg(3, &easysdl.timer)
        .Run(easysdl.screen_buffer_size)
        .ReadBuffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);

    while (!easysdl.KeyDown(SDLK_ESCAPE)) {
        easysdl
            .Tick()
            .Render()
            .SetTitle("FPS: " + std::to_string(easysdl.fps));

        easycl
            .UpdateArg(3, &easysdl.timer)
            .Run(easysdl.screen_buffer_size)
            .ReadBuffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);
    }
    easysdl.Destroy();

    return 0;
}