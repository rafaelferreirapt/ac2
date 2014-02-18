#------------------------------------
strcat:
	#*dst => $a0 => $s0
	#*src => $a1 => $s1
	#*rp = *dst = $s2
	addiu $sp, $sp, -16
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	#
	mv $s0, $a0 # dst
	mv $s1, $a1 # src
	mv $s2, $a0 # rp  
	#
strcat_for:
	lb $t0, $s0
	beqz $t0, strcat_endfor
	addiu $s0, $s0, 1
	b strcat_for
strcat_endfor:
	#
	mv $a0, $s0
	mv $a1, $s1
	jal strcpy
	mv $v0, $s2
	#
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	addiu $sp, $sp, 16
	jr $ra
#---------------------------------
