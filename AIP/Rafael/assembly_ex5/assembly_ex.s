	#https://github.com/gipmon/ac2

	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xF0
	ori $t1, $t1, 0xF0
	sw $t1, TRISE($t0)
	#configurado
	lw $t2, PORTE($t0)
	srl $t2, 4
	andi $t2, $t2, 0x0F
	#isolar bits
	#bit 0
	andi $t3, $t2, 0x1
	#bit 1
	andi $t4, $t2, 0x2
	srl $t4, $t4, 1
	#bit 2
	andi $t5, $t2, 0x4
	srl $t5, $t5, 2
	#bit 3
	andi $t6, $t2, 0x8
	srl $t6, $t6, 3
if_1:
	beqz $t3, if_2
	li $a0, '0'
	li $v0, 3
	syscall
if_2:
	beqz $t4, if_3
	li $a0, '1'
	li $v0, 3
	syscall
if_3:
	beqz $t5, if_4
	li $a0, '2'
	li $v0, 3
	syscall
if_4:
	beqz $t6, endif
	li $a0, '3'
	li $v0, 3
	syscall
endif:
	lw $t3, LATE($t0)
	andi $t3, $t3, 0xFFF0
	or $t2, $t3, $t2
	sw $t2, LATE($t0)
	#fim de execucao
	jr $ra
