	.equ PRINT_STR, 8
	.data
	.globl msg
msg: 	.asciiz "AC2 - DETPIC32 primer\n"
	.text
	.globl main
main:
	la $a0,msg
	ori $v0,$0,PRINT_STR
	syscall # printStr("AC2 – DETPIC32 primer\n");
	ori $v0,$0,0 # return 0;
	jr $ra 
