#! /bin/sh
make clean
make micro
./micro.out ./testfiles/micro_test.in > a.txt
python3 mips_translate.py > output.asm
