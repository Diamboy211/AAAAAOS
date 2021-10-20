[bits 16]
[org 0x7c00]

CODE_SEG equ code_desc - gdt_start
DATA_SEG equ data_desc - gdt_start
KERNEL_LOC equ 0x1000

start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov bp, 0x8000
	mov sp, bp

	mov bx, KERNEL_LOC
	mov ah, 0x02
	mov al, 15
	mov ch, 0
	mov cl, 2
	mov dh, 0
	int 0x13
	jc oof
	cmp al, 15
	je good
oof:
	mov bx, 0xB800
	mov es, bx
	mov si, derr
	mov bx, 0
.print:
	mov al, [si]
	mov [es:bx], al
	inc bx
	mov byte [es:bx], 15
	inc bx
	inc si
	cmp si, (derr + 20)
	jb .print
.loop:
	hlt
	jmp .loop
derr:	db 'disk read error     '

good:
	call check_a20
	test ax, ax
	jnz A20_on
	call enable_A20
.wait:
	call check_a20
	test ax, ax
	jz .wait
A20_on:
	mov ah, 0x00
	mov al, 0x13
	int 0x10
	cli
	lgdt [gdt_desc]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp CODE_SEG:pro

gdt_start:
	dd 0
	dd 0
code_desc:
	dw 0xFFFF ; first 2 bytes of limit
	dw 0      ;
	db 0      ; first 24 bits of base
	db 0b10011010
	db 0b11001111 ; other flags + last 4 bits of limit
	db 0      ; last 8 bits of base
data_desc:
	dw 0xFFFF ; first 2 bytes of limit
	dw 0      ;
	db 0      ; first 24 bits of base
	db 0b10010010
	db 0b11001111 ; other flags + last 4 bits of limit
	db 0      ; last 8 bits of base
	; what a fucking mess
gdt_end:

gdt_desc:
	dw gdt_end - gdt_start - 1
	dd gdt_start

enable_A20:
        cli
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    ax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     ax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        sti
        ret
 
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret

check_a20:
	pushf
	push ds
	push es
	push di
	push si

	cli

	xor ax, ax ; ax = 0
	mov es, ax

	not ax ; ax = 0xFFFF
	mov ds, ax

	mov di, 0x0500
	mov si, 0x0510

	mov al, byte [es:di]
	push ax

	mov al, byte [ds:si]
	push ax

	mov byte [es:di], 0x00
	mov byte [ds:si], 0xFF

	cmp byte [es:di], 0xFF

	pop ax
	mov byte [ds:si], al

	pop ax
	mov byte [es:di], al

	mov ax, 0
	je check_a20__exit

	mov ax, 1

check_a20__exit:
	pop si
	pop di
	pop es
	pop ds
	popf

	ret

[bits 32]
pro:
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, 0x600
	mov cr4, eax   ; no checking
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ebp, 0xE00000
	mov esp, ebp
	jmp KERNEL_LOC

times (510 - ($ - $$)) db 0x00
dw 0xAA55
