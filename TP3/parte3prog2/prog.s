	#https://github.com/gipmon/ac2
	.equ CALIBRATION_VALUE, 6000
	.data
str: 	.asciiz "\r\n"
	.text

	.globl main
main:
	addiu $sp, $sp, -4
	sw $ra, 0($sp)
	#
while:
	li $v0, 12
	syscall # reset core timer
	li $a0, 1
	jal delay
	#
	li $v0, 11
	syscall
	move $a0, $v0
	#
	li $a1, 10
	sll $a1, $a1, 16
	addi $a1, $a1, 10
	#
	li $v0, 7
	syscall
	#
	la $a0, str
	li $v0, 8
	syscall
	#
	b while
	#
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra
delay:
for_1:
	beqz $a0, endfor_1
	li $t0, CALIBRATION_VALUE
for_2:
	beqz $t0, endfor_2
	addi $t0, $t0, -1
	b for_2
endfor_2:
	addiu $a0, $a0, -1
	b for_1
endfor_1:
	jr $ra
