	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120

.data
buf: .space 21
seq: .byte 0x0, 0xF, 0x0, 0xF
.text
.globl main

main:
	lui $t0, SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xFFF0
	ori $t1,$t1, 0x00F0
	sw $t1,TRISE($t0)

	#configurado

while:
	#lr caracteres
	la $a0,buf
	li $a1, 20
	li $v0,9
	syscall

	li $a0, '\n'
	li $v0,3
	syscall

	#piscar
	li $t3,0
piscar:
	bge $t3,4,fimpisca
	lw $t4, LATE($t0)
	andi $t4,$t4,0xFFF0
	lb $t5, seq($t3)		#vai buscar codigo desligado/ligado
	or $t4,$t4,$t5	
	sw $t4,LATE($t0)

	#delay
	li $t6, 1000

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

fimpisca:
	lw $t2, PORTE($t0)
	andi $t2,$t2, 0x00F0
	srl $t2,$t2,4

	la $t3, buf
	
for:
	lb $t4, 0($t3)
	beq $t4, $zero, end_for

	add $t5,$t4,$t2

	sb $t5,0($t3)

	addi $t3,$t3,1

	b for

end_for:
	la $a0, buf
	li $v0,8
	syscall

	li $a0, '\n'
	li $v0,3
	syscall

	b while

	jr $ra
