#1a)

	.data
	.word 3,1,-2,0,3,-10,-1,3
	
	.text
main:
	lui $t0, 0x1001
	ori $t0, $t0, 0x0000
	addi $t4, $0, 0

	
inv:
	beq $t4, 8 END
	addi $t4, $t4, 1
	lw $t1,	0($t0)
	slt $t2, $t1, $0
	bne $t2, 1, ELSE
	sw $t1, 0($t0)
#	nop
	sub $t1, $0, $t1
	sw $t1, 0($t0)
ELSE:	j inv
	addi $t0, $t0, 4
	
END:	
