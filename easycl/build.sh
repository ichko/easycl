build_dir=dist
include_dir=/usr/local/cuda-8.0/include/

rm -rf ./$build_dir
mkdir ./$build_dir

clang -std=c++14 main.cpp -o ./$build_dir -I$include_dir -lOpenCL -Wall 

