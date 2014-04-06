.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

.text
.globl main

main:

	lui $t1, SFR_BASE_HI
	lw $t2,TRISE($t1)
	andi $t2, $t2, 0xFFF0
	ori $t2, $t2, 0x00F0
	sw $t2, TRISE($t1)

for:
	lw $t2, PORTE($t1)
	andi $t2,$t2, 0x00F0
	srl $t2,$t2,4

	andi $t4, $t2, 0x0003
	srl $t5, $t4, 1
	or $t4, $t4, $t5
	andi $t4, $t4, 0x0001

	andi $t5, $t2, 0x000C
	srl $t6, $t5, 1
	or $t5, $t5, $t6
	andi $t5,$t5,0x0004

	srl $t5, $t5, 2

	or $t6, $t5, $t4
	xor $t7, $t5, $t4
	and $t8, $t5, $t4
	nor $t9, $t5, $t4

	sll $t7,$t7,1
	sll $t8, $t8,2
	sll $t9, $t9,3

	or $t6, $t6, $t7
	or $t6, $t6, $t8
	or $t6, $t6, $t9

	lw $t3,LATE($t1)
	andi $t3, $t3, 0xFFF0
	or $t3, $t3, $t6
	sw $t3, LATE($t1)

	b for

jr $ra