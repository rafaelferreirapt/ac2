	#https://github.com/gipmon/ac2
	.equ SFR_BASE_HI, 0xBF88			#16 Msbits of SFR area
	.equ TRISE, 0x6100				#TRISE address is 0xBF886100
	.equ PORTE, 0x6110				#PORTE address is 0xBF886110
	.equ LATE, 0x6120				#LATE address is 0xBF886120

	.text

	.globl main
main:
	#endereco
	lui $t2, SFR_BASE_HI
	lw $t3, TRISE($t2)
	andi $t3, $t3, 0xFFF0 # saida => 0
	ori $t3, $t3, 0x00C0 # RE7 e RE6
	sw $t3, TRISE($t2)

	#inicialização
	li $v0, 12
	syscall 		#reset ao timer
	li $t0, 0		#contador
while:
	li $v0, 11
	syscall			# read core timer
	blt $v0, 5000000, while # esperar 0.25s
	# leitura
	lw $t3, PORTE($t2)
	move $t6, $t4 # save do $t4 => $t6
	andi $t4, $t3, 0x0040
	beq $t6, $t4, endif
	li $t0, 0
endif:
	andi $t5, $t3, 0x0080

	beqz $t4, normal
	bnez $t5, johnson_decrescente
johnson_crescente:
	#algoritmo johson crescente
	sll $t0, $t0, 1
	srl $t1, $t0, 4
	xor $t1, $t1, 0x1
	or  $t0, $t0, $t1
	and $t0, $t0, 0x0F
	b clean
johnson_decrescente:
	#algoritmo johson decrescente
	andi $t1, $t0, 0x1
	xor $t1, $t1, 0x1
	sll $t1, $t1, 4
	or $t0, $t0, $t1
	srl $t0, $t0, 1
	b clean
normal:
	bnez $t5, decrescente_normal
crescente_normal:
	addi $t0, $t0, 1
	andi $t0, $t0, 0x0F
	b clean
decrescente_normal:
	addi $t0, $t0, -1
	andi $t0, $t0, 0x0F
	b clean
clean:

	#impressao
	#lui $a1, 0x4		# 4 0000
	#ori $a1, $a1, 2		# base 2
	#move $a0, $t0		# valor
	#li $v0, 6		# $v0, valor do syscall
	#syscall			# syscall
	# !impressao

	#LAT E
	lw $t3, LATE($t2)
	andi $t3, $t3, 0xFFF0
	or $t3, $t3, $t0
	sw $t3, LATE($t2)
	#LAT E

	#li $a0, 0xD
	#li $v0, 3
	#syscall

	li $v0, 12
	syscall
	b while

	jr $ra
