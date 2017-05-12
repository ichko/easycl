#include "cl-context.hpp"


int main() {
    int A[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };
    int C[10];

    auto context = CLContext()
        .load_device()
        .load_src("sum.cl")
        .load_kernel("simple_add")
        .set_arg(0, A, 10)
        .set_arg(1, B, 10)
        .set_arg(2, C, 10)
        .run(10)
        .read_buffer(2, C, 10);

    std::cout << " result: ";
    for (int i = 0; i<10; i++) {
        std::cout << C[i] << " ";
    }

    return 0;
}