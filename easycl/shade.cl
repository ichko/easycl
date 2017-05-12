#include "utils.cl"

vec3 shade(const float u, const float v) {
    return v3(1, 2, 3);
}

void kernel init(
    global vec3* screen_buffer,
    global const int* width,
    global const int* height
) {
    int global_id = get_global_id(0);
    float ar = (float) *width / (float) *height;
    int x = global_id % *width;
    int y = global_id / *width;
    screen_buffer[global_id] = shade(1.0, 1.0);
}
