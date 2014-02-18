strcpy:
	#dst => $a0
	#src => $a1
	#*rp => $t0

	move $t0, $a0	#char *rp = dst;

forcpy:		#for(; (*dst = *src) != 0; dst++, src++);
	lb $t1, 0($a1)
	sb $t1, 0($t0)		#*dst = *src;

	beqz $t1, end_forcpy	#(*dst = *src) != 0
	addi $a1,$a1,1 		#dst++
	addi $t0,$t0,1		#src++

	b forcpy

end_forcpy:
	move $v0, $t0 		#return rp
	jr $ra


strcmp:
	#s1 => $a0
	#s2 => $a1
	#*s1 => $t0
	#*s2 => $t1

forcmp:
	lb $t0, 0($a0)
	lb $t1, 0($a1)

	bne $t0, $t1, end_forcmp
	beqz $t0, end_forcmp

	addi $a0,$a0,1
	addi $a1,$a1,1

	b forcmp

end_forcmp:
	sub $v0, $t0,$t1

	jr $ra

