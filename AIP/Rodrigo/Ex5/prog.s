	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120

seq: .byte 0x0, 0xF, 0x0, 0xF
	.data
	.text
	.globl main

main:
	#conf entradas e saidas#
	lui $t0,SFR_BASE_HI
	lw $t1,TRISE($t0)
	andi $t1, $t1, 0xFFF0		#RE0-RE3, saidas
	ori $t1,$t1, 0x00F0			#RE4-RE7, entradas
	sw $t1, TRISE($t0)
	#configurado

while:
	lw $t2,PORTE($t0)
	andi $t2,0x00F0
	srl $t2,$t2,4

	bne $t2, 0xF, end

	li $t3,0

piscar:
	bge $t3, 4, end_pisca
	lw $t4, LATE($t0)
	andi $t4,$t4, 0xFFF0
	lb $t5,seq($t3)
	or $t4,$t4,$t5
	sw $t4,LATE($t0)

	#delay

	li $t6,1000

for_1:
	beqz $t6, endfor_1
	li $t7, 6000
for_2:
	beqz $t7, endfor_2
	addi $t7,$t7,-1
	b for_2
endfor_2:
	addi $t6,$t6,-1
	b for_1
endfor_1:
	addi $t3,$t3,1
	b piscar

end:
	lw $t4, LATE($t0)
	andi $t4,$t4,0xFFF0
	sw $t4,LATE($t0)
end_pisca:
	
	b while

	jr $ra





