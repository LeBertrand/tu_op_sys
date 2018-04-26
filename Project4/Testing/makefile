CC=gcc

2MBtest: 2mbmapping.c 
	$(CC) 2mbmapping.c -o 2MBtest.out -lm

getBlockSize: getpagesize.c
	$(CC) getpagesize.c -o sizetest.out -lm

wipeDrive: wipeDrive.c
	$(CC) wipeDrive.c -o wipeDrive.out

allocateTest: allocate_driver.c ../boot_process.cpp ../SOFAT_allocate_block.c
	g++ ../boot_process.o ../SOFAT_allocate_block.c allocate_driver.c