#include "easycl.hpp"
#include "easysdl.hpp"


int main(int argc, char** argv) {
    while (true) {
        int width = 500, height = 500,
            local_size_width = 0, local_size_heihgt = 0;

        std::cout << "Input size (width height): " << std::endl;
        std::cin >> width >> height;
        std::cout << "Input local sizes (set -1 for default): " << std::endl;
        std::cin >> local_size_width >> local_size_heihgt;

        auto local_size = cl::NDRange(local_size_width, local_size_heihgt);
        if (local_size_width < 0 || local_size_heihgt < 0) {
            local_size = cl::NullRange;
        }

        auto easysdl = EasySDL()
            .Init()
            .SetWindow(width, height);
            //.SetWindowFullscreen();

        try {
            auto easycl = EasyCL()
                .LoadDevice(0, 1)
                .LoadSrc("shader.cl")
                .LoadKernel("start")
                .SetArg(0, easysdl.screen_buffer, easysdl.screen_buffer_size)
                .SetArg(1, &easysdl.width)
                .SetArg(2, &easysdl.height)
                .SetArg(3, &easysdl.timer);

            while (!easysdl.KeyDown(SDLK_ESCAPE)) {
                easycl
                    .UpdateArg(3, &easysdl.timer)
                    .Run(cl::NDRange(easysdl.width, easysdl.height), local_size)
                    .ReadBuffer(0, easysdl.screen_buffer, easysdl.screen_buffer_size);

                easysdl
                    .Tick()
                    .Render()
                    .SetTitle("FPS: " + std::to_string(easysdl.fps) + ", " +
                              "Time: " + std::to_string(easysdl.timer));
            }
            easysdl.Destroy();
        }
        catch (std::string error) {
            std::cout << "Failed with error: " << error << std::endl
                << "Press any key to try again...\n";
            easysdl.Destroy();

            // Block console
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}
