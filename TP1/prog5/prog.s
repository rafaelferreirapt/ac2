	.equ STR_MAX_SIZE, 20
	.equ PRINT_STR, 8
	.equ READ_STR, 9
	.equ PRINT_INT, 6
	.equ PRINT_INT_10, 7 
	#	
	.data
str_int:.asciiz "Introduza 2 strings: "
str_res:.asciiz "Resultados:\n"
str1:	.space STR_MAX_SIZE+1
str2:	.space STR_MAX_SIZE+1
str3:	.space 2*STR_MAX_SIZE+1
	#
	.text
	.globl main
main:
	la $a0, str_int
	li $v0, READ_STR
	#
	la $a0, str1
	li $a1, STR_MAX_SIZE
	syscall
	#
	la $a0, str2
	li $a1, STR_MAX_SIZE
	syscall
	#
	la $a0, str_res
	li $v0, READ_STR
	#
	la $a0, str1
	jal strlen
	move $a0, $v0
	li $a1, 10
	li $v0, PRINT_INT
	syscall
	#
	la $a0, str2
	jal strlen
	move $a0, $v0
	li $a1, 10
	li $v0, PRINT_INT
	syscall
	#
	la $a0, str3
	la $a1, str1
	jal strcpy
	#
	la $a0, str3
	la $a1, str2
	jal strcat
	move $a0, $v0
	li $v0, PRINT_STR
	syscall	
	#
	la $a0, str1
	la $a1, str2
	jal strcmp
	move $a0, $v0
	li $v0, PRINT_INT_10
	syscall
	#
	li $v0, 0
	jr $ra
strcat:
	
	jr $ra
strcpy:
	#*rp => $t0

	move $v0, $t0
	jr $ra
######################################
strlen:
	#len = 0 => $t0 = 0
	li $t0, 0
strlen_for:
	lb $t1, 0($a0)
	beqz $t1, '\0', strlen_endfor
	addiu $t0, $t0, 1
	addiu $a0, $a0, 1
	b strlen_for
strlen_endfor:
	move $v0, $t0
	jr $ra
