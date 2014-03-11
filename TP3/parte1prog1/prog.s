	.equ SFR_BASE_HI, 0xBF88			#16 Msbits of SFR area
	.equ TRISE, 0x6100					#TRISE address is 0xBF886100
	.equ PORTE, 0x6110					#PORTE address is 0xBF886110
	.equ LATE, 0x6120					#LATE address is 0xBF886120

	.text

	.globl main

main:
	lui $t1, SFR_BASE_HI				#
	lw $t2, TRISE($t1)					#Mem_addr = 0xBF880000 + 0x6100
	andi $t2, $t2, 0xFFFE				#bit0 = 0 PORT RE0
	ori $t2, $t2, 0x0040				#bit6 = 1 PORT RE6
	sw $t2, TRISE($t1)

for:
	lw $t2, PORTE($t1)
	andi $t2, $t2, 0x0040				# 0000 0000 0100 0000  só nos interessa o 6bit = RE6
	srl $t2, $t2, 6 					# colocar para RE0 = 0bit
	#
	lw $t3, LATE($t1)					# carregar o que está no LAT E
	andi $t3, $t3, 0xFFFE				# coloca o bit0 a 0 (que é o bit de saida), precisamos dele limpo, a 0
	or $t3, $t3, $t2					# bit0 = bit6 , RE0 = RE6
	sw $t3, LATE($t1)					# configurou o valor da saida

	b for

	jr $ra