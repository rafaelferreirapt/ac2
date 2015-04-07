# https://github.com/gipmon/ac2
#	2014
	.equ INKEY, 1
	.equ STRING, 8
	.data
str:	.asciiz "Key pressed\n"
	.text
	.globl main
main:
	#char c => $t0
while:
	li $v0, INKEY
	syscall
	move $t0, $v0
	beqz $t0, while
if:
	bne $t0, '\n', endif
	b return
endif:
	li $v0, STRING
	la $a0, str
	syscall
	b while
return:
	li $v0, 0
	jr $ra
