



bochs: floppy.img
	bochs -f bochsrc.txt -rc bochsconfig.txt

floppy.img: sysBoot/stage1/boot1.bin 
	sudo losetup /dev/loop20 floppy.img
	sudo dd if=sysBoot/stage1/boot1.bin of=/dev/loop20
	sudo losetup -d /dev/loop20

sysBoot/stage1/boot1.bin: sysBoot/stage1/boot1.asm
	nasm -f bin $^ -o $@ 

