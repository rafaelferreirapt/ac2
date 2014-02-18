	.eqv N_INT, 5
	.eqv TRUE, 1
	.eqv FALSE, 0
	.eqv PRINT_INT, 10
	.eqv PRINT_STR, 8
	.eqv READ_INT, 4
	.eqv PUT_CHAR, 3
	#
	.data
str1:	.asciiz "\nLeitura e ordenacao de inteiros em base 10\n"
str2:	.asciiz "Introduza 5 Inteiros: "
str3:	.asciiz "\nNumero de trocas realizado: "
str4:	.asciiz "\nResultado da ordenacao: "
lista:	.space N_INT*4 # static unsigned int lista[N_INT]
	.text
	.globl main
main:	
	addiu $sp, $sp, -16
	sw $ra, 0($sp) 
	sw $s0, 4($sp) # unsigned int i
	sw $s1, 8($sp) # unsigned int n_trocas
	sw $s2, 12($sp) # unsigned lista
	#
	# unsigned int *ptr;
	#
	la $a0, str1
	li $v0, PRINT_STR
	syscall # printStr()
	#
	la $a0, str2
	li $v0, PRINT_STr
	syscall # printStr()
	#
	li $s0, 0 # i=0
	la $s2, lista # load lista
main_for_1:
	bge $s0, N_INT, main_endfor_1
	sll $t0, $s0, 2 # i *2
	addu $t0, $s2, $t0 # *lista[i]
	#
	li $v0, READ_INT
	li $a0, 10
	syscall
	sw $v0, 0($t0)
	#
	li $v0, PUT_CHAR
	li $a0, ' '
	syscall
	#
	addiu $s0, $s0, 1
	b main_for_1
main_endfor_1:
	#
	li $a0, N_INT
	la $a1, lista
	jal sequentialSort
	move $s1, $v0 #n_trocas
	#
	li $v0, PRINT_STR
	la $a0, str3
	syscall
	#
	move $a0, $s1
	li $a1, 10
	li $v0, PRINT_INT
	syscall
	#
	la $a0, str4
	li $v0, PRINT_STR
	syscall
	#
	la $t0, lista #ptr = lista
	addiu $t1, $t0, N_INT # lista + N_INT
main_for_2:
	bge $t0, $t1, main_endfor_2
	#
	lw $a0, 0($t0)
	li $v0, PRINT_INT
	li $a1, 10
	syscall
	#
	li $v0, PUT_CHAR
	li $a0, ' '
	syscall
	#
	addiu $t0, $t0, 4
	b main_for_2
main_endfor_2:
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	addiu $sp, $sp, 16
	jr $ra
