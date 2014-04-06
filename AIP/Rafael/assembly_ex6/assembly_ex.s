	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE,  0x6120
	.equ SEQ_SYMBOLS, 8
	.equ CALIBRATION_VALUE, 6000
	.data
seq:.byte 0x1, 0x7, 0x4, 0x5, 0x7, 0x3, 0x4, 0x7
	.text
	.globl main
main:
	#configuracao
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xF0
	ori $t1, $t1, 0xF0
	sw $t1, TRISE($t0)
	#programa
	li $t1, 0
while:
	blt $t1, SEQ_SYMBOLS, not_zero
	li $t1, 0
not_zero:
	#obter e colocar
	lb $t2, seq($t1)
	lw $t3, LATE($t0)
	andi $t3, $t3, 0xFFF0
	or $t2, $t2, $t3
	sw $t2, LATE($t0)
	#delay
	li $a0, 300
for_1:
	beqz $a0, endfor_1
	addiu $a0, $a0, -1
	li $a1, CALIBRATION_VALUE
for_2:
	beqz $a1, endfor_2
	addiu $a1, $a1, -1
	b for_2
endfor_2:
	b for_1
endfor_1:
	addiu $t1, $t1, 1
	b while
	jr $ra
