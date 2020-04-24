



bochs: 
	bochs -f bochsrc.txt -rc bochsconfig.txt
qemu:
	qemu-system-i386 -fda floppy.img -monitor stdio

install: install-bootloader install-kernel

install-bootloader: sysBoot/stage1/boot1.bin
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	sudo losetup /dev/loop20 floppy.img
	sudo mkdosfs -F 12 /dev/loop20
	sudo mount /dev/loop20 /home/wenzapen/floppy/ -t msdos -o "fat=12"
	sudo dd if=sysBoot/stage1/boot1.bin of=/dev/loop20 
	
install-kernel: 
	sudo rm -rf /home/wenzapen/floppy/stage2.bin
	rm -rf sysBoot/stage2/stage2.bin 
	nasm -f bin sysBoot/stage2/stage2.asm -o sysBoot/stage2/stage2.bin
	sudo cp sysBoot/stage2/stage2.bin /home/wenzapen/floppy/


sysBoot/stage1/boot1.bin: sysBoot/stage1/boot1.asm
	nasm -f bin $^ -o $@ 

sysBoot/stage2/stage2.bin: sysBoot/stage2/stage2.asm
	nasm -f bin $^ -o $@ 
clean:
	rm sysBoot/stage2/stage2.bin 
clean-all:
	rm sysBoot/stage1/boot1.bin sysBoot/stage2/stage2.bin floppy.img
	sudo umount /home/wenzapen/floppy
	sudo losetup -d /dev/loop20
