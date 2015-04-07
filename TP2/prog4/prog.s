	#https://github.com/gipmon/ac2
	.text
	.globl main
main:
	li $v0,12
	syscall # reset timer
	li $t0, 0 # s contador
while:
	li $v0,11
	syscall # read core timer
	div $t1, $v0, 20000000

	#start if
	beq $t1, $t0, while
	#
	move $a0, $t0
	li $v0, 7
	syscall # print contador
	#
	addiu $t0, $t0, 1
	#
	li $a0, '\n'
        li $v0,3
        syscall
	#endif

	b while

	li $v0,0
	jr $ra
