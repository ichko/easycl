#include "marcher.cl"

float3 shade(float2 uv, float time) {
    ray r = calc_ray(uv, time);
    float i = march(r, time);
    return (float3)(i, 0, i / 3);
}

void kernel start(
    global int* screen_buffer,
    constant const int* width,
    constant const int* height,
    constant float* time
) {
    float2 pos = { get_global_id(0), get_global_id(1) };
    //float2 pos = (float2)(pixel_id % *width, pixel_id / *width);

    float ar = (float)*width / (float)*height;
    float2 uv = pos / min(*width, *height) - (float2)(ar * 0.5, 0.5);

    float3 fcolor = shade(uv, *time) * 255;
    int icolor = ((int) fcolor.z << 16) + ((int) fcolor.y << 8) + (int) fcolor.x;
    screen_buffer[(int)(pos.y * *width + pos.x)] = icolor;
}
