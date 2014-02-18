#	Grupo - Rafel e Rodrigo
#	2014
	.equ GETCHAR, 2
	.equ PUTCHAR, 3
	.data
	.text
	.globl main
main:

while:
	li $v0, GETCHAR
	syscall
	move $t0,$v0

if:	beq $t0,'\n', return
	move $a0,$t0
	li $v0, PUTCHAR
	syscall
	b while
return:
	li $v0, 1
	jr $ra
