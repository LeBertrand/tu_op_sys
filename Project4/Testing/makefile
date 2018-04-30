CC=gcc

2MBtest: 2mbmapping.c 
	$(CC) 2mbmapping.c -o 2MBtest.out -lm

getBlockSize: getpagesize.c
	$(CC) getpagesize.c -o sizetest.out -lm

wipeDrive: wipeDrive.c
	$(CC) wipeDrive.c -o wipeDrive.out

allocateTest: allocate_driver.c ../boot_process.o ../SOFAT_allocate_block.c
	make boot_process
	g++ ../boot_process.o ../SOFAT_allocate_block.c allocate_driver.c

boot_process: ../boot_process.cpp
	g++ ../boot_process.cpp -c ../boot_process.o

rw_dirlisting: rw_root_dirs.c
	gcc rw_root_dirs.c -I.. -o DirListingDemo.out
	cp ../Drive2MBBlankCopy Drive2MB
	./resetdrive
