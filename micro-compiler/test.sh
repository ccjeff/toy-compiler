#! /bin/sh
make clean
make micro
./micro.out ./testfiles/test1.in > a.txt
python3 mips_translate.py > output.asm
python3 mips_simulator.py
