ENTRY(main)

SECTIONS {
    . = 0xc0000000;

    .text ALIGN(0X1000): {
        *(.text)
    }

    .rodata ALIGN(0x1000): {
        *(.rodata*)
    }

    .data ALIGN(0x1000): {
	*(.data)
    }

    .bss ALIGN(0x1000): {
	*(.bss)
    }
    kernel_end = .;
}
