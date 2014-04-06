	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.text
	.globl main
main:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	ori $t1, $t1, 0xF0
	andi $t1, $t1, 0xF0
	sw $t1, TRISE($t0)

while:
	lw $t1, PORTE($t0)
	srl $t1, $t1, 4

	li $v0, 5
	syscall
if:
	bgt $v0, 9, endif
	blt $v0, 1, endif
	add $t1, $t1, $v0
endif:
	sw $t1, LATE($t0)

	b while
	jr $ra