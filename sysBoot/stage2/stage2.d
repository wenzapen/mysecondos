
stage2.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:	e9 45 01 60 ac       	jmp    0xac60014a
   5:	08 c0                	or     %al,%al
   7:	74 06                	je     0xf
   9:	b4 0e                	mov    $0xe,%ah
   b:	cd 10                	int    $0x10
   d:	eb f5                	jmp    0x4
   f:	61                   	popa   
  10:	c3                   	ret    
  11:	00 00                	add    %al,(%eax)
  13:	60                   	pusha  
  14:	bf 00 80 0b 00       	mov    $0xb8000,%edi
  19:	31 c0                	xor    %eax,%eax
  1b:	b9 a0 00 00 00       	mov    $0xa0,%ecx
  20:	a0 12 05 00 00       	mov    0x512,%al
  25:	f7 e1                	mul    %ecx
  27:	50                   	push   %eax
  28:	a0 11 05 00 00       	mov    0x511,%al
  2d:	b1 02                	mov    $0x2,%cl
  2f:	f6 e1                	mul    %cl
  31:	59                   	pop    %ecx
  32:	01 c8                	add    %ecx,%eax
  34:	01 c7                	add    %eax,%edi
  36:	80 fb 0a             	cmp    $0xa,%bl
  39:	74 18                	je     0x53
  3b:	88 da                	mov    %bl,%dl
  3d:	b6 0f                	mov    $0xf,%dh
  3f:	66 89 17             	mov    %dx,(%edi)
  42:	fe 05 11 05 00 00    	incb   0x511
  48:	80 3d 11 05 00 00 50 	cmpb   $0x50,0x511
  4f:	74 02                	je     0x53
  51:	eb 0d                	jmp    0x60
  53:	c6 05 11 05 00 00 00 	movb   $0x0,0x511
  5a:	fe 05 12 05 00 00    	incb   0x512
  60:	61                   	popa   
  61:	c3                   	ret    
  62:	60                   	pusha  
  63:	53                   	push   %ebx
  64:	5f                   	pop    %edi
  65:	8a 1f                	mov    (%edi),%bl
  67:	80 fb 00             	cmp    $0x0,%bl
  6a:	74 08                	je     0x74
  6c:	e8 a2 ff ff ff       	call   0x13
  71:	47                   	inc    %edi
  72:	eb f1                	jmp    0x65
  74:	8a 3d 12 05 00 00    	mov    0x512,%bh
  7a:	8a 1d 11 05 00 00    	mov    0x511,%bl
  80:	e8 02 00 00 00       	call   0x87
  85:	61                   	popa   
  86:	c3                   	ret    
  87:	60                   	pusha  
  88:	31 c0                	xor    %eax,%eax
  8a:	b9 50 00 00 00       	mov    $0x50,%ecx
  8f:	88 f8                	mov    %bh,%al
  91:	f7 e1                	mul    %ecx
  93:	00 d8                	add    %bl,%al
  95:	89 c3                	mov    %eax,%ebx
  97:	b0 0f                	mov    $0xf,%al
  99:	66 ba d4 03          	mov    $0x3d4,%dx
  9d:	ee                   	out    %al,(%dx)
  9e:	88 d8                	mov    %bl,%al
  a0:	66 ba d5 03          	mov    $0x3d5,%dx
  a4:	ee                   	out    %al,(%dx)
  a5:	31 c0                	xor    %eax,%eax
  a7:	b0 0e                	mov    $0xe,%al
  a9:	66 ba d4 03          	mov    $0x3d4,%dx
  ad:	ee                   	out    %al,(%dx)
  ae:	88 f8                	mov    %bh,%al
  b0:	66 ba d5 03          	mov    $0x3d5,%dx
  b4:	ee                   	out    %al,(%dx)
  b5:	61                   	popa   
  b6:	c3                   	ret    
  b7:	60                   	pusha  
  b8:	a2 11 05 00 00       	mov    %al,0x511
  bd:	88 25 12 05 00 00    	mov    %ah,0x512
  c3:	61                   	popa   
  c4:	c3                   	ret    
  c5:	60                   	pusha  
  c6:	fc                   	cld    
  c7:	bf 00 80 0b 00       	mov    $0xb8000,%edi
  cc:	66 b9 d0 07          	mov    $0x7d0,%cx
  d0:	b4 0f                	mov    $0xf,%ah
  d2:	b0 20                	mov    $0x20,%al
  d4:	f3 66 ab             	rep stos %ax,%es:(%edi)
  d7:	c6 05 11 05 00 00 00 	movb   $0x0,0x511
  de:	c6 05 12 05 00 00 00 	movb   $0x0,0x512
  e5:	61                   	popa   
  e6:	c3                   	ret    
  e7:	fa                   	cli    
  e8:	60                   	pusha  
  e9:	0f 01 16             	lgdtl  (%esi)
  ec:	09 06                	or     %eax,(%esi)
  ee:	fb                   	sti    
  ef:	61                   	popa   
  f0:	c3                   	ret    
	...
  f9:	ff                   	(bad)  
  fa:	ff 00                	incl   (%eax)
  fc:	00 00                	add    %al,(%eax)
  fe:	9a cf 00 ff ff 00 00 	lcall  $0x0,$0xffff00cf
 105:	00 92 cf 00 17 00    	add    %dl,0x1700cf(%edx)
 10b:	f1                   	icebp  
 10c:	05 00 00 fa 50       	add    $0x50fa0000,%eax
 111:	b0 dd                	mov    $0xdd,%al
 113:	e6 64                	out    %al,$0x64
 115:	58                   	pop    %eax
 116:	c3                   	ret    
 117:	e4 64                	in     $0x64,%al
 119:	a8 02                	test   $0x2,%al
 11b:	75 fa                	jne    0x117
 11d:	c3                   	ret    
 11e:	e4 64                	in     $0x64,%al
 120:	a8 01                	test   $0x1,%al
 122:	74 fa                	je     0x11e
 124:	c3                   	ret    
 125:	0a 53 65             	or     0x65(%ebx),%dl
 128:	61                   	popa   
 129:	72 63                	jb     0x18e
 12b:	68 69 6e 67 20       	push   $0x20676e69
 130:	66 6f                	outsw  %ds:(%esi),(%dx)
 132:	72 20                	jb     0x154
 134:	4f                   	dec    %edi
 135:	70 65                	jo     0x19c
 137:	72 61                	jb     0x19a
 139:	74 69                	je     0x1a4
 13b:	6e                   	outsb  %ds:(%esi),(%dx)
 13c:	67 20 53 79          	and    %dl,0x79(%bp,%di)
 140:	73 74                	jae    0x1b6
 142:	65 6d                	gs insl (%dx),%es:(%edi)
 144:	2e 2e 2e 00 fa       	cs cs cs add %bh,%dl
 149:	31 c0                	xor    %eax,%eax
 14b:	8e d8                	mov    %eax,%ds
 14d:	8e c0                	mov    %eax,%es
 14f:	b8 00 90 8e d0       	mov    $0xd08e9000,%eax
 154:	bc ff ff be 25       	mov    $0x25beffff,%esp
 159:	06                   	push   %es
 15a:	e8 a6 fe e8 87       	call   0x87e90005
 15f:	ff                   	(bad)  
 160:	e8 ac ff fa 0f       	call   0xffb0111
 165:	20 c0                	and    %al,%al
 167:	66 83 c8 01          	or     $0x1,%ax
 16b:	0f 22 c0             	mov    %eax,%cr0
 16e:	ea 73 06 08 00 66 b8 	ljmp   $0xb866,$0x80673
 175:	10 00                	adc    %al,(%eax)
 177:	8e d8                	mov    %eax,%ds
 179:	8e d0                	mov    %eax,%ss
 17b:	8e c0                	mov    %eax,%es
 17d:	bc 00 00 09 00       	mov    $0x90000,%esp
 182:	e8 3e ff ff ff       	call   0xc5
 187:	bb 93 06 00 00       	mov    $0x693,%ebx
 18c:	e8 d1 fe ff ff       	call   0x62
 191:	fa                   	cli    
 192:	f4                   	hlt    
 193:	0a 0a                	or     (%edx),%cl
 195:	0a 20                	or     (%eax),%ah
 197:	20 20                	and    %ah,(%eax)
 199:	20 20                	and    %ah,(%eax)
 19b:	20 20                	and    %ah,(%eax)
 19d:	20 3c 5b             	and    %bh,(%ebx,%ebx,2)
 1a0:	20 4f 53             	and    %cl,0x53(%edi)
 1a3:	20 44 65 76          	and    %al,0x76(%ebp,%eiz,2)
 1a7:	65 6c                	gs insb (%dx),%es:(%edi)
 1a9:	6f                   	outsl  %ds:(%esi),(%dx)
 1aa:	70 6d                	jo     0x219
 1ac:	65 6e                	outsb  %gs:(%esi),(%dx)
 1ae:	74 20                	je     0x1d0
 1b0:	53                   	push   %ebx
 1b1:	65 72 69             	gs jb  0x21d
 1b4:	65 73 20             	gs jae 0x1d7
 1b7:	54                   	push   %esp
 1b8:	75 74                	jne    0x22e
 1ba:	6f                   	outsl  %ds:(%esi),(%dx)
 1bb:	72 69                	jb     0x226
 1bd:	61                   	popa   
 1be:	6c                   	insb   (%dx),%es:(%edi)
 1bf:	20 31                	and    %dh,(%ecx)
 1c1:	30 20                	xor    %ah,(%eax)
 1c3:	5d                   	pop    %ebp
 1c4:	3e 0a 0a             	or     %ds:(%edx),%cl
 1c7:	20 20                	and    %ah,(%eax)
 1c9:	20 20                	and    %ah,(%eax)
 1cb:	20 20                	and    %ah,(%eax)
 1cd:	20 20                	and    %ah,(%eax)
 1cf:	42                   	inc    %edx
 1d0:	61                   	popa   
 1d1:	73 69                	jae    0x23c
 1d3:	63 20                	arpl   %sp,(%eax)
 1d5:	33 32                	xor    (%edx),%esi
 1d7:	20 62 69             	and    %ah,0x69(%edx)
 1da:	74 20                	je     0x1fc
 1dc:	67 72 61             	addr16 jb 0x240
 1df:	70 68                	jo     0x249
 1e1:	69 63 73 20 64 65 6d 	imul   $0x6d656420,0x73(%ebx),%esp
 1e8:	6f                   	outsl  %ds:(%esi),(%dx)
 1e9:	20 69 6e             	and    %ch,0x6e(%ecx)
 1ec:	20 41 73             	and    %al,0x73(%ecx)
 1ef:	73 65                	jae    0x256
 1f1:	6d                   	insl   (%dx),%es:(%edi)
 1f2:	62 6c 79 20          	bound  %ebp,0x20(%ecx,%edi,2)
 1f6:	4c                   	dec    %esp
 1f7:	61                   	popa   
 1f8:	6e                   	outsb  %ds:(%esi),(%dx)
 1f9:	67 75 61             	addr16 jne 0x25d
 1fc:	67                   	addr16
 1fd:	65                   	gs
	...
