	.text
	.globl main
main:
	li $v0,12 
	syscall # reset timer
	li $t0, 57 # seconds countr
	li $t1, 59 # minutes count
	li $t2, 23 # hours count
while:	
	li $v0,11
	syscall # read core timer
	
	# START IF
	blt $v0, 20000000, while
		
	# IMPRESSAO DO CONTADOR
	lui $a1, 0x2 # 0x2 tamanho da impressao
	ori $a1, $a1, 10 # base de impressao
	move $a0, $t2 # valor a imprimir
	li $v0, 6
	syscall
	#
	li $a0, ':'
	li $v0, 3
	syscall
	# 
	move $a0, $t1
	li $v0, 6
	syscall
	#
	li $a0, ':'
	li $v0, 3
	syscall
	#
	move $a0, $t0
	li $v0, 6
	syscall
	# IMPRESSAO DO CONTADOR
	
	# INCREMENTAR
	addiu $t0, $t0, 1
if1:	bne $t0, 60, endif1
	li $t0, 0
	addiu $t1, $t1, 1
if2:	bne $t1, 60, endif1
	li $t1, 0
	addiu $t2, $t2, 1
if3:	bne $t2, 24, endif1
	li $t2, 0
endif1:
	# INCREMENTAR
	# PRINT BARRA N
	li $a0, 0xD
        li $v0,3
        syscall
	# ENDIF
	
	li $v0, 12
	syscall # reset timer
	
	b while
	
	li $v0,0
	jr $ra
