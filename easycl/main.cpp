#include <iostream>

#include "easycl.hpp"
#include "canvas.hpp"
//#include "easysdl.hpp"


int main(int argc, char** argv) {
    while (true) {
        std::cout << "# Parallel programming" << std::endl << std::endl;
        int platform_id = 0, device_id = 1,
            width = 150, height = 150,
            local_size_width = 0, local_size_heihgt = 0;
        auto local_size = cl::NullRange;

        std::cout << "Input size (width height): " << std::endl;
        std::cin >> width >> height;

        // std::cout << "Input local sizes (set -1 for default): " << std::endl;
        // std::cin >> local_size_width >> local_size_heihgt;
        if (local_size_width > 0 || local_size_heihgt > 0) {
            local_size = cl::NDRange(local_size_width, local_size_heihgt);
        }

        // List paltforms and devices
        std::cout << std::endl << std::endl << "Platforms and devices: " << std::endl;
        auto all_platforms = EasyCL::GetPlatforms();
        for (int i = 0; i < all_platforms.size(); i++) {
            std::cout << i << ") " <<
                all_platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;

            auto all_devices = EasyCL::GetDevices(all_platforms[i]);
            for (int j = 0; j < all_devices.size(); j++) {
                std::cout << "   " << j << ") "<<
                    all_devices[j].getInfo<CL_DEVICE_NAME>() << std::endl;
            }
        }

        std::cout << std::endl << "Choose platform and device id (ex.: 0 1): " << std::endl;
        std::cin >> platform_id >> device_id;

        auto canvas = ImageCanvas(width, height, "image.ppm");
        int timer = 1;

        try {
            auto easycl = EasyCL()
                .LoadDevice(platform_id, device_id)
                .LoadSrc("shader.cl")
                .LoadKernel("start")
                .SetArg(0, canvas.context.screen_buffer, canvas.context.screen_buffer_size)
                .SetArg(1, &canvas.context.width)
                .SetArg(2, &canvas.context.height)
                .SetArg(3, &timer)
                .Run(cl::NDRange(canvas.context.width, canvas.context.height), local_size)
                .ReadBuffer(0, canvas.context.screen_buffer, canvas.context.screen_buffer_size);

            canvas.Render();
        }
        catch (std::string error) {
            std::cout << "Failed with error: " << error << std::endl;
        }

        std::cout << "Press any key to try again..." << std::endl;
        std::cin.ignore();
        std::cin.get();
        system("cls");
    }

    return 0;
}


// OpenCL Example
//int main(int argc, char** argv) {
//    while (true) {
//        std::cout << "# Parallel programming" << std::endl << std::endl;
//        int platform_id = 0, device_id = 1,
//            width = 400, height = 400,
//            local_size_width = 0, local_size_heihgt = 0;
//        auto local_size = cl::NullRange;
//
//        //std::cout << "Input size (width height): " << std::endl;
//        //std::cin >> width >> height;
//
//        //// std::cout << "Input local sizes (set -1 for default): " << std::endl;
//        //// std::cin >> local_size_width >> local_size_heihgt;
//        //if (local_size_width > 0 || local_size_heihgt > 0) {
//        //    local_size = cl::NDRange(local_size_width, local_size_heihgt);
//        //}
//
//        //// List paltforms and devices
//        //std::cout << std::endl << std::endl << "Platforms and devices: " << std::endl;
//        //auto all_platforms = EasyCL::GetPlatforms();
//        //for (int i = 0; i < all_platforms.size(); i++) {
//        //    std::cout << i << ") " <<
//        //        EasyCL::GetName<CL_PLATFORM_NAME>(all_platforms[i]) << std::endl;
//
//        //    auto all_devices = EasyCL::GetDevices(all_platforms[i]);
//        //    for (int j = 0; j < all_devices.size(); j++) {
//        //        std::cout << "   " << j << ") " <<
//        //            EasyCL::GetName<CL_DEVICE_NAME>(all_devices[j]) << std::endl;
//        //    }
//        //}
//
//        //std::cout << std::endl << "Choose platform and device id (ex.: 0 1): " << std::endl;
//        //std::cin >> platform_id >> device_id;

//        auto easysdl = EasySDL(width, height);
//
//        try {
//            auto easycl = EasyCL()
//                .LoadDevice(platform_id, device_id)
//                .LoadSrc("shader.cl")
//                .LoadKernel("start")
//                .SetArg(0, easysdl.context.screen_buffer, easysdl.context.screen_buffer_size)
//                .SetArg(1, &easysdl.context.width)
//                .SetArg(2, &easysdl.context.height)
//                .SetArg(3, &easysdl.timer);
//
//            while (!easysdl.KeyDown(SDLK_ESCAPE)) {
//                easycl
//                    .UpdateArg(3, &easysdl.timer)
//                    .Run(cl::NDRange(easysdl.context.width, easysdl.context.height), local_size)
//                    .ReadBuffer(0, easysdl.context.screen_buffer, easysdl.context.screen_buffer_size);
//
//                easysdl
//                    .Tick()
//                    .Render()
//                    .SetTitle("FPS: " + std::to_string(easysdl.fps) + ", " +
//                              "Time: " + std::to_string(easysdl.timer));
//            }
//        }
//        catch (std::string error) {
//            std::cout << "Failed with error: " << error << std::endl;
//        }
//
//        std::cout << "Press any key to try again..." << std::endl;
//        easysdl.Destroy();
//        std::cin.ignore();
//        std::cin.get();
//        system("cls");
//    }
//
//    return 0;
//}
