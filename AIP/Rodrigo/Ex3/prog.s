.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

seq1: .byte 0x0, 0xF, 0xE, 0xD, 0xA, 0x3
seq2: .byte 0xC, 0x3, 0x6, 0x9, 0x6, 0x1

.text
.globl main

main:
	#### definição entradas saidas
	lui $t0,SFR_BASE_HI
	lw $t1, TRISE($t0)
	andi $t1, $t1, 0xFFF0 	#RE0-RE3--saidas = 0
	ori $t1,$t1, 0x00F0		#RE4-RE7--entradas = 1
	sw $t1, TRISE($t0)

	#inicialização

	li $v0,12			#reset core timer
	syscall

	li $t7, 0				#contador = 0

	#########

while:

	li $v0,11			#read core timer
	syscall

	blt $v0,5000000, while			#wait 0,04 segundos

	lw $t2, PORTE($t0)
	andi $t3, $t2, 0x0010 	#bit 1 dipswitch
	andi $t4, $t2, 0x0020	#bit 2
	andi $t5, $t2, 0x0040	#bit 3
	andi $t6, $t2, 0x0080	#bit 4

if:
	bnez $t3, seq1normal
	bnez $t4, seq1decrescente
	bnez $t5, seq2crescente
	bnez $t6, seq2decrescente

	li $t7, 0

	b final

seq1normal:
	lb $t8, seq1($t7)
	b endcrescente

seq1decrescente:
	lb $t8, seq1($t7)
	b enddecrescente

seq2crescente:
	lb $t8, seq2($t7)
	b endcrescente

seq2decrescente:
	lb $t8, seq2($t7)
	b enddecrescente

endcrescente:
	addi $t7, $t7, 1
	bgt $t7,5, zero
	b final

enddecrescente:
	addi $t7, $t7, -1
	blt $t7,0, cinco
	b final

zero:
	li $t7,0
	b final

cinco:
	li $t7,5
final:

	lw $t9, LATE($t0)
	andi $t9,$t9,0xFFF0
	or $t9,$t9, $t8
	sw $t9, LATE($t0)

	li $v0,12
	syscall

	b while

jr $ra

