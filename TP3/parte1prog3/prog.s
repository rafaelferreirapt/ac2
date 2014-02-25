	.equ SFR_BASE_HI, 0xBF88			#16 Msbits of SFR area
	.equ TRISE, 0x6100					#TRISE address is 0xBF886100
	.equ PORTE, 0x6110					#PORTE address is 0xBF886110
	.equ LATE, 0x6120					#LATE address is 0xBF886120

	.text

	.globl main

main:
	lui $t1, SFR_BASE_HI				#
	lw $t2, TRISE($t1)					#Mem_addr = 0xBF880000 + 0x6100
	andi $t2, $t2, 0xFFF0				#bit0 = bit1 = bit2 = bit3 = 0 PORT RE0, RE1, RE2, RE3
	ori $t2, $t2, 0x00C0				#bit6 = bit7 = 1 PORT RE6, RE7
	sw $t2, TRISE($t1)

for:	
	lw $t2, PORTE($t1)					#
	andi $t3, $t2, 0x0040				#Mask para o bit6
	andi $t4, $t2, 0x0080				#Mask para o bit7
	srl $t4, $t4, 1
	and $t5, $t3, $t4
	or $t2, $t3, $t4
	xor $t6, $t3, $t4
	xori $t7, $t2, 0x0040

	srl $t5, $t5,6
	srl $t2,$t2,5
	srl $t6, $t6,4
	srl $t7,$t7,3

	lw $t8, LATE($t1)
	andi $t8,$t8, 0xFFF0
