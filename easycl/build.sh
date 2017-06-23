build_dir=dist
include_dir=/usr/local/cuda-8.0/include
lib_dir=/usr/local/cuda-8.0/lib64

rm -rf ./$build_dir
mkdir ./$build_dir

g++ -std=c++14 main.cpp -o ./$build_dir/res.out -I$include_dir -L$lib_dir -lOpenCL 

