	.equ N_INT, 5
	.equ TRUE, 1
	.equ FALSE, 0
	.equ PRINT_INT, 6
	.equ PRINT_STR, 8
	.equ READ_INT, 4
	.equ PUT_CHAR, 3
	#
	.data
str1:	.asciiz "\nLeitura e ordenacao de inteiros em base 10\n"
str2:	.asciiz "Introduza 5 Inteiros: "
str3:	.asciiz "\nNumero de trocas realizado: "
str4:	.asciiz "\nResultado da ordenacao: "
lista:	.space 20 # static unsigned int lista[N_INT]
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
	li $v0, PRINT_STR
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
###
sequentialSort:
	#n_val => $a0 => $s0
	#array => $a1 => $s1
	#n_trocas => $s2
	#i => $s3
	#j => $s4

	addiu $sp,$sp,-24
	sw $ra,0($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)
	sw $s3, 16($sp)
	sw $s4, 20($sp)

	move $s0,$a0
	move $s1,$a1
	li $s2, 0		#n_trocas = 0
	li $t3, 0		#i = 0
	sub $t2, $s0, 1 #n_val -1
sequential_for1:
	bge $s3,$t2, end_sequential_for1
	addi $s4, $s3,1 #j = i+1
sequential_for2:
	bge	$s4, $s0, end_sequential_for2
sequential_if:
	sll $t3, $s3, 2 
	add $t3,$s1,$t3
	lw $t5, 0($t3)	#array[i]
	sll $t4, $s4, 2
	add $t4, $s1, $t4
	lw $t6,0($t4)	#array[j]
	ble $t3,$t4, end_sequential_if
	move $a0, $t3
	move $a1, $t4

	jal troca
	addi $s2,$s2,1
end_sequential_if:
	addi $s4,$s4,1
	b sequential_for2
end_sequential_for2:
	addi $s3,$s3,1
	b sequential_for1
end_sequential_for1:

	move $v0,$s2

	lw $s4, 20($sp)
	lw $s3, 16($sp)
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addiu $sp,$sp,24

	jr $ra
#########################################
troca:
	lw $t7, 0($a0)
	lw $t8, 0($a1)
	sw $t7, 0($a1)
	sw $t8, 0($a0)

	jr $ra