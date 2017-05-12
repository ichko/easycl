#include "cl-context.hpp";

int main() {
    auto default_device = = get_device();
    cl::Context context({ default_device });

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code =
        "   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
        "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 "
        "   }                                                                               ";
    sources.push_back({ kernel_code.c_str(),kernel_code.length() });

    cl::Program program(context, sources);
    if (program.build({ default_device }) != CL_SUCCESS) {
        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);

    int A[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context, default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);


    //run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, C);

    std::cout << " result: \n";
    for (int i = 0; i<10; i++) {
        std::cout << C[i] << " ";
    }

    return 0;
}