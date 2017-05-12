void kernel simple_add(global const int* a, global const int* b, global int* result) {
    result[get_global_id(0)] = a[get_global_id(0)] + b[get_global_id(0)];
}
