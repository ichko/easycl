#pragma once

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


struct EasyCL {

    cl::Device device;
    cl::Context context;
    cl::Program program;
    cl::Kernel kernel;
    cl::CommandQueue queue;
    std::map<size_t, cl::Buffer> buffers;
    int error;

    void AssertSuccess(int result, std::string at = "") {
        if (result != CL_SUCCESS) {
            error = result;
            throw "[" + at + "] " + std::to_string(result);
        }
    }

    EasyCL& Run(cl::NDRange global_range, cl::NDRange local_range = cl::NullRange) {
        AssertSuccess(queue.enqueueNDRangeKernel(kernel, cl::NullRange, global_range, local_range), "queue.enqueueNDRangeKernel");
        AssertSuccess(queue.finish(), "queue.finish");

        return *this;
    }

    template <typename T> EasyCL& ReadBuffer(size_t arg_id, T* out, size_t size) {
        AssertSuccess(queue.enqueueReadBuffer(buffers[arg_id], CL_TRUE, 0, sizeof(T) * size, out), "queue.enqueueReadBuffer");
        return *this;
    }

    template <typename T> EasyCL SetArg(
        cl_uint arg_id,
        T* data,
        size_t size = 1,
        int buffer_type = CL_MEM_READ_WRITE
    ) {
        cl::Buffer buffer(context, buffer_type, sizeof(T) * size);
        AssertSuccess(queue.enqueueWriteBuffer(buffer, CL_TRUE, 0, sizeof(T) * size, data), "SetArg:queue.enqueueWriteBuffer");
        AssertSuccess(kernel.setArg(arg_id, buffer), "kernel.setArg");
        buffers[arg_id] = buffer;

        return *this;
    }

    template <typename T> EasyCL UpdateArg(
        cl_uint arg_id,
        T* data,
        size_t size = 1,
        int buffer_type = CL_MEM_READ_WRITE
    ) {
        AssertSuccess(queue.enqueueWriteBuffer(buffers[arg_id], CL_TRUE, 0, sizeof(T) * size, data), "UpdateArg:queue.enqueueWriteBuffer");

        return *this;
    }

    EasyCL& LoadKernel(std::string function_name) {
        kernel = cl::Kernel(program, function_name.c_str());
        return *this;
    }

    EasyCL& LoadSrc(std::string src_file) {
        cl::Program::Sources sources;

        std::string kernel_code = ReadFile(src_file);
        sources.push_back({ kernel_code.c_str(), kernel_code.length() });

        program = cl::Program(context, sources);
        auto build_result = program.build({ device });
        AssertSuccess(build_result, "Error building kernel: " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));

        std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        return *this;
    }

    EasyCL& LoadDevice(size_t platform_id = 0, size_t device_id = 0) {
        error = 0;

        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);
        AssertSuccess(all_platforms.size() == 0 ? -1 : 0, "No platforms found. Check OpenCL installation!");

        cl::Platform platform = all_platforms[platform_id];
        //std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";

        std::vector<cl::Device> all_devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
        AssertSuccess(all_devices.size() == 0 ? -1 : 0, "No devices found. Check OpenCL installation!");

        device = all_devices[device_id];
        //std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";

        context = cl::Context({ device });
        queue = cl::CommandQueue(context, device);

        return *this;
    }

    static std::string ReadFile(std::string file_name) {
        std::ifstream file;
        file.open(file_name);
        std::stringstream str_stream;
        str_stream << file.rdbuf();
        file.close();

        return str_stream.str();
    }

};
