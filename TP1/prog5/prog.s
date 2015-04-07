#https://github.com/gipmon/ac2
<<<<<<< HEAD
	.eqv STR_MAX_SIZE, 20
	.eqv PRINT_STR, 8
	.eqv READ_STR, 9
	.eqv PRINT_INT, 6
	.eqv PRINT_INT_10, 7
	#
	.data
str_int:.asciiz "Introduza 2 strings: "
str_res:.asciiz "Resultados:\n"
=======
#	Grupo - Rafel e Rodrigo
#	2014
#	Não está completamente correto.
	.equ STR_MAX_SIZE, 20
	.equ PRINT_STR, 8
	.equ READ_STR, 9
	.equ PRINT_INT, 6
	.equ PRINT_INT_10, 7
	#
	.data
str_int:.asciiz "Introduza 2 strings: "
str_res:.asciiz "\nResultados:\n"
>>>>>>> 33ed5bd2e5352c7b5af2dca60301183dbc86ba94
str1:	.space 21
str2:	.space 21
str3:	.space 41
	#
	.text
	.globl main
main:
	addiu $sp, $sp, -4
	sw $ra, 0($sp)
	#----------------
	la $a0, str_int
	li $v0, PRINT_STR
	syscall
	#
	la $a0, str1
	li $a1, STR_MAX_SIZE
	li $v0, 9
	syscall
	#
	la $a0, str2
	li $a1, STR_MAX_SIZE
	li $v0,9
	syscall
	#
	la $a0, str_res
	li $v0, PRINT_STR
	syscall
	#
	la $a0, str1
	jal strlen
	li $a1,10
	move $a0, $v0
	li $v0, PRINT_INT
	syscall
	#
	la $a0, str2
	jal strlen
	li $a1,10
	move $a0, $v0
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
	#-----------
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra
####################################
strcat:
	#*dst => $a0 => $s0
	#*src => $a1 => $s1
	#*rp = *dst = $s2
	addiu $sp, $sp, -16
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	#
	move $s0, $a0 # dst
	move $s1, $a1 # src
	move $s2, $a0 # rp
	#
strcat_for:
	lb $t0, 0($s0)
	beqz $t0, strcat_endfor
	addiu $s0, $s0, 1
	b strcat_for
strcat_endfor:
	#
	move $a0, $s0
	move $a1, $s1
	jal strcpy
	move $v0, $s2
	#
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	addiu $sp, $sp, 16
	jr $ra
##################################
strcpy:
	#dst => $a0
	#src => $a1
	#*rp => $t0

	move $t0, $a0	#char *rp = dst;

forcpy:		#for(; (*dst = *src) != 0; dst++, src++);
	lb $t1, 0($a1)
	sb $t1, 0($t0)		#*dst = *src;

	beqz $t1, end_forcpy	#(*dst = *src) != 0
	addi $a1,$a1,1 		#dst++
	addi $t0,$t0,1		#src++

	b forcpy

end_forcpy:
	move $v0, $t0 		#return rp
	jr $ra

######################################
strlen:
	#len = 0 => $t0 = 0
	li $t0, 0
strlen_for:
	lb $t1, 0($a0)
	beqz $t1, strlen_endfor
	addiu $t0, $t0, 1
	addiu $a0, $a0, 1
	b strlen_for
strlen_endfor:
	move $v0, $t0
	jr $ra

#######################################

strcmp:
	#s1 => $a0
	#s2 => $a1
	#*s1 => $t0
	#*s2 => $t1

forcmp:
	lb $t0, 0($a0)
	lb $t1, 0($a1)

	bne $t0, $t1, end_forcmp
	beqz $t0, end_forcmp

	addi $a0,$a0,1
	addi $a1,$a1,1

	b forcmp

end_forcmp:
	sub $v0, $t0,$t1

	jr $ra
