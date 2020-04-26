GDB = gdb
LD = i686-elf-ld
CC = i686-elf-gcc
C_SOURCES=$(wildcard sysCore/kernel/*.c \
			sysCore/lib/*.c \
			)
OBJ=$(C_SOURCES:.c=.o)
CFLAGS=-g -ffreestanding -I sysCore/include



bochs: 
	bochs -f bochsrc.txt -rc bochsconfig.txt
qemu:
	qemu-system-i386 -fda floppy.img -monitor stdio

install: install-bootloader install-kernel

install-bootloader: sysBoot/stage1/boot1.bin sysBoot/stage2/stage2.bin
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	sudo losetup /dev/loop20 floppy.img
	sudo mkdosfs -F 12 /dev/loop20
	sudo mount /dev/loop20 /home/wenzapen/floppy/ -t msdos -o "fat=12"
	sudo dd if=sysBoot/stage1/boot1.bin of=/dev/loop20 
	sudo cp sysBoot/stage2/stage2.bin /home/wenzapen/floppy/
	
install-kernel: kernel.elf 
#	sudo rm -rf /home/wenzapen/floppy/stage2.bin
#	rm -rf sysBoot/stage2/stage2.bin 
#	sudo rm -rf /home/wenzapen/floppy/kernel.bin
#	sudo rm -rf /home/wenzapen/floppy/kernel.elf
#	rm -rf sysBoot/kernel/kernel.bin 
#	rm -rf kernel.elf 
#	nasm -f bin sysBoot/stage2/stage2.asm -o sysBoot/stage2/stage2.bin
#	nasm -f bin sysBoot/kernel/stage3.asm -o sysBoot/kernel/kernel.bin
#	sudo cp sysBoot/stage2/stage2.bin /home/wenzapen/floppy/
#	sudo cp sysBoot/kernel/kernel.bin /home/wenzapen/floppy/
	sudo cp kernel.elf /home/wenzapen/floppy/


sysBoot/stage1/boot1.bin: sysBoot/stage1/boot1.asm
	nasm -f bin $^ -o $@ 

sysBoot/stage2/stage2.bin: sysBoot/stage2/stage2.asm
	nasm -f bin $^ -o $@ 

sysBoot/kernel/kernel.bin: sysBoot/kernel/stage3.asm
	nasm -f bin $^ -o $@ 

kernel.elf: ${OBJ}
	${LD} -T linker.ld -melf_i386 -o $@ $^
#	${LD} -melf_i386 -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
clean:
	rm kernel.elf 
clean-all:
	rm sysBoot/stage1/boot1.bin sysBoot/stage2/stage2.bin 
	rm kernel.elf floppy.img
	sudo umount /home/wenzapen/floppy
	sudo losetup -d /dev/loop20

reset:
	sudo umount /home/wenzapen/floppy
	sudo losetup -d /dev/loop20

