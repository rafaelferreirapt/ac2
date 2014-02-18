sequentialSort:
	#n_val => $a0 => $s0
	#array => $a1 => $s1
	#n_trocas => $s2
	#i => $t0
	#j => $t1
	addiu $sp,$sp,-16
	sw $ra,0($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)

	move $s0,$a0
	move $s1,$a1
	li $s2, 0		#n_trocas = 0
	li $t0, 0		#i = 0
	subi $t2, $s0,1 #n_val -1
sequential_for1:
	bge $t0,$t2, end_sequential_for1
	addi $t1, $t0,1 #j = i+1
sequential_for2:
	bge	$t1, $s0, end_sequential_for2
sequential_if:
	sll $t3, $t0, 2 
	add $t3,$s0,$t3
	lw $t5,0($t3)	#array[i]
	sll $t4, $t1, 2
	add $t4, $s0, $t4
	lw $t6,0($t4)	#array[j]
	ble $t3,$t4, end_sequential_if
	move $a0, $t3
	move $a1, $t4

	jal troca
	addi $s2,$s2,1
end_sequential_if:
	b sequential_for2
end_sequential_for2:
	b sequential_for1
end_sequential_for1:

	move $v0,$s2

	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addiu $sp,$sp,16

	jr $ra


#########################################
troca:
	lw $t7, 0($a0)
	lw $t8, 0($a1)
	sw $t7, 0($a1)
	sw $t8, 0($a0)

	jr $ra


	


