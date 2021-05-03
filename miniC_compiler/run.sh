mkdir build ; cd build
cmake ..
make
./miniC_compiler ../testfiles/test_simple.in >> test.txt
