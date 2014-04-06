.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

.text
.globl main

main:

	lui $t1, SFR_BASE_HI
	lw $t2, TRISE($t1)
	andi $t2, $t2, 0xFFF0
	ori $t2, $t2, 0x00F0
	sw $t2, TRISE($t1)

	#######
for:
	lw $t2, PORTE($t1)
	
	andi $t2, $t2, 0x00F0
	srl $t2,$t2,4

	li $v0, 5
	syscall

	bgt $v0, 9, end
	blt $v0, 1, end

	add $t2,$t2, $v0

end: 
	lw $t3, LATE($t1)
	andi $t3, $t3, 0xFFF0
	or $t3, $t3, $t2
	sw $t3, LATE($t1)

	li $v0,7
	move $a0,$t2
	syscall

	b for

jr $ra
