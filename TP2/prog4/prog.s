	.text
	.globl main
main:
	li $v0,12
	syscall

while:	
	li $v0,11
	syscall

	move $a0,$v0
	li $v0,7
	syscall

	li $a0, '\n'
	li $v0,3
	syscall
	
	b while

	li $v0,0

	jr $ra