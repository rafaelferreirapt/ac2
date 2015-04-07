	#https://github.com/gipmon/ac2

	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.equ CALIBRATION_VALUE, 6000
	.data
seq1: .byte 0x0, 0xF, 0xE, 0xD, 0xA, 0x3
seq2: .byte 0xC, 0x3, 0x6, 0x9, 0x6, 0x1
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	ori $t1, $t1, 0xF0
	andi $t1, $t1, 0xF0
	sw $t1, TRISE($t0)
	#configurado
	li $t8, 0
while:
	bge $t8, 5, zero
	blt $t8, 0, zero
	b delay
zero:	li $t8, 0
	#increment
delay:
	#delay
	li $a0, 250
for_1:
	beqz $a0, endfor_1
	li $a1, CALIBRATION_VALUE
for_2:
	beqz $a1, endfor_2
	addi $a1, $a1, -1
	b for_2
endfor_2:
	addiu $a0, $a0, -1
	b for_1
endfor_1:
	#delay

	lw $t1, PORTE($t0)
	srl $t1, $t1, 4
	andi $t1, $t1, 0x0F
if:
	beq $t1, 0x1, seq1_up
	beq $t1, 0x2, seq1_down
	beq $t1, 0x4, seq2_up
	beq $t1, 0x8, seq2_down
	b while
seq1_up:
	lb $t1, seq1($t8)
	sw $t1, LATE($t0)
	add $t8, $t8, 1
	b while
seq1_down:
	lb $t1, seq1($t8)
	sw $t1, LATE($t0)
	add $t8, $t8, -1
	b while
seq2_up:
	lb $t1, seq2($t8)
	sw $t1, LATE($t0)
	add $t8, $t8, 1
	b while
seq2_down:
	lb $t1, seq2($t8)
	sw $t1, LATE($t0)
	add $t8, $t8, -1
	b while
	jr $ra
