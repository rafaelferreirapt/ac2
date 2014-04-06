	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ CALIBRATION_VALUE, 6000
	.data
seq:.byte 0xF, 0x0, 0xF, 0x0
buf:.space 21
	.text
	.globl main
	#hertz:
	#Conversion base : 1 ms(p) = 1000 Hz
	#1000/25hz = 40ms
main:
	#configurar as saidas e as entradas
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	ori $t1, $t1, 0xF0
	andi $t1, $t1, 0xF0
	sw $t1, TRISE($t0)
while:
	#ler a sequencia de caracteres
	la $v0, 9
	la $a0, buf
	li $a1, 20
	syscall
	#
	li $a0, '\n'
	li $v0, 3
	syscall
	#pisca pisca
	li $t1, 0
pisca:
	bge $t1, 4, endpisca
	lb $t2, seq($t1)
	sw $t2, LATE($t0)
	#delay
	li $a0, 1000
for_1:
	beqz $a0, endfor_1
	li $a1, CALIBRATION_VALUE
for_2:
	beqz $a1, endfor_2
	addiu $a1, $a1, -1
	b for_2
endfor_2:
	addiu $a0, $a0, -1
	b for_1
endfor_1:
	#delay
	addiu $t1, $t1, 1
	b pisca
endpisca:
	#ler do dipswitch
	lw $t1, PORTE($t0)
	srl $t1, $t1, 4
	#alterar a sequencia
	la $t2, buf
for:
	lb $t3, 0($t2)
	beq $t3, $zero, endfor
	#
	addu $t4, $t1, $t3
	sb $t4, 0($t2)
	#
	addiu $t2, $t2, 1
	b for
endfor:
	la $a0, buf
	li $v0, 8
	syscall
	#
	li $a0, '\n'
	li $v0, 3
	syscall
	b while
	#
	jr $ra
