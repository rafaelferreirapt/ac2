	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.text
	.globl main
main:
	#confgIN
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	ori $t1, $t1, 0xF0
	andi $t1, $t1, 0xF0
	sw $t1, TRISE($t0)

while:
	#while
	#bit0 => $t2
	#bit1 => $t3
	#bit2 => $t4
	#bit3 => $t5
	#bit0+bit1 => $t2
	#bit2+bit3 => $t3

	lw $t1, PORTE($t0)
	andi $t1, $t1, 0x00F0
	srl $t1, $t1, 4
	#bits estao no sitio correto

	srl $t5, $t1, 3
	srl $t4, $t1, 2
	andi $t4, $t4, 0x1
	srl $t3, $t1, 1
	andi $t3, $t3, 0x1
	andi $t2, $t1, 0x1
	#bits obtidos

	or $t2, $t2, $t3
	or $t3, $t4, $t5

	lw $t1, LATE($t0)
	andi $t1, $t1, 0xF0

	or $t4, $t2, $t3
	or $t1, $t1, $t4

	xor $t4, $t2, $t3
	sll $t4, $t4, 1
	or $t1, $t1, $t4

	mul $t4, $t2, $t3
	sll $t4, $t4, 2
	or $t1, $t1, $t4

	xor $t4, $t2, $t3
	xor $t4, $t4, 1
	sll $t4, $t4, 3
	or $t1, $t1, $t4

	sw $t1, LATE($t0)
b while

	li $v0, 0
	jr $ra