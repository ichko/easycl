build_dir=dist
rm -rf ./$build_dir
mkdir ./$build_dir
g++ *.cpp -o ./$build_dir
