section .text
	[bits 32]
	[extern kernel_main]
	call kernel_main
	jmp $

%include "src/idt.asm"

global isr_stub_table
isr_stub_table:
%assign i 0
%rep 32
	dd isr_stub_%+i
%assign i i+1
%endrep
