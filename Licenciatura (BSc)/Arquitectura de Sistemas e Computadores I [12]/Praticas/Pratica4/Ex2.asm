#2)

	.data
	.asciiz "TGPM"
	
	.text
main:
	lui $t0, 0x1001
	ori $t0, $t0, 0x0000
	addi $t2, $0, 0
	addi $t3, $0, 0
	
		
Num:	
	addi $t0, $t0, 1
	j Num
	nop