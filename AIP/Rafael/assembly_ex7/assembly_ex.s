	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE,  0x6120
	.equ SEQ_LENGTH, 10
	.equ CALIBRATION_VALUE, 6000
	.data
seq:.byte 0x1, 0x2, 0x3, 0x4, 0x5, 0xF, 0xD, 0xA, 0x0, 0xC
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xF0
	ori $t1, $t1, 0xF0
	sw $t1, TRISE($t0)
	#configuracao feita

	li $t1, 0 #contador
if:
	blt $t1, SEQ_LENGTH, endif
	li $t1, 0
endif:
	lb $t2, seq($t1)
	lw $t3, LATE($t0)
	andi $t3, $t3, 0xFFF0
	or $t2, $t2, $t3
	sw $t2, LATE($t0)
	#delay
	li $a0, 300
for_1:
	beq $a0, 0, endfor_1
	li $a1, CALIBRATION_VALUE
for_2:
	beq $a1, 0, endfor_2
	add $a1, $a1, -1
	b for_2
endfor_2:
	add $a0, $a0, -1
	b for_1
endfor_1:
	#delay
	addiu $t1, $t1, 1
	b if
	jr $ra
