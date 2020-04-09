



bochs: floppy.img
	bochs -f bochsrc.txt -rc bochsconfig.txt

floppy.img: sysBoot/stage1/boot1.bin sysBoot/stage2/stage2.bin
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	sudo losetup -o 512 /dev/loop20 floppy.img
	sudo mkfs.vfat -F 12 /dev/loop20
	dd if=sysBoot/stage1/boot1.bin of=floppy.img bs=512 count=1 conv=notrunc
	sudo mount /dev/loop20 /home/wenzapen/floppy/
	sudo cp sysBoot/stage2/stage2.bin /home/wenzapen/floppy/


sysBoot/stage1/boot1.bin: sysBoot/stage1/boot1.asm
	nasm -f bin $^ -o $@ 

sysBoot/stage2/stage2.bin: sysBoot/stage2/stage2.asm
	nasm -f bin $^ -o $@ 
clean:
	rm sysBoot/stage1/boot1.bin sysBoot/stage2/stage2.bin floppy.img
	sudo umount /home/wenzapen/floppy
	sudo losetup -d /dev/loop20
