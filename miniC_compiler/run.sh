mkdir build ; cd build
rm test.txt
cmake ..
make
./miniC_compiler ../testfiles/test_simple.in >> test.txt
cd ../src
rm output.asm
python3 mips_translate.py >> output.asm
