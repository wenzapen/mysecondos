%ifndef COMMON_H
%define COMMON_H

%define IMAGE_PMODE_BASE 0x100000  ; where kernel is to be loaded to in p mode
%define IMAGE_RMODE_BASE 0x3000 ; where kernel to be loaded to in real mode

imageName db "KERNEL  ELF"
imageSize db 0 


%endif
