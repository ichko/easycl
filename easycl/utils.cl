typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y;
} vec2;

vec3 v3(float x, float y, float z) {
    vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;

    return result;
}
