#include "easycl.hpp"
#include "easysdl.hpp"


void init() {
    auto easysdl = EasySDL()
        .Init()
        .SetWindow(500, 500);
    //.SetWindowFullscreen();

    auto easycl = EasyCL()
        .LoadDevice(0, 0)
        .LoadSrc("shader.cl")
        .LoadKernel("start")
        .SetArg(0, easysdl.screen_buffer, easysdl.screen_buffer_size)
        .SetArg(1, &easysdl.width)
        .SetArg(2, &easysdl.height)
        .SetArg(3, &easysdl.timer);

    while (!easysdl.KeyDown(SDLK_ESCAPE)) {
        easycl
            .UpdateArg(3, &easysdl.timer)
            .Run(cl::NDRange(easysdl.width, easysdl.height))
            .ReadBuffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);

        easysdl
            .Tick()
            .Render()
            .SetTitle("FPS: " + std::to_string(easysdl.fps) + ", " +
                      "Time: " + std::to_string(easysdl.timer));
    }
    easysdl.Destroy();
}

int main(int argc, char** argv) {
    try {
        init();
    }
    catch (std::string error) {
        std::cout << "Failed with error: " << error << std::endl;
    }

    return 0;
}
