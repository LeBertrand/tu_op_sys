#!/bin/bash
oldfile='../Drive2MBBlankCopy'
newfile='Drive2MB'
cp oldfile newfile
includes='Documents/Temple University/Operating Systems/Project4'
gcc fill_root_dirs.c -Iincludes
./a.out > dirlisting_rw.txt
return 0