#1a)
#	addi $t0, $0, 2
#	addi $t1, $0, 3
#	bne $t0, $t1, Else
#	nop
#	add $t0, $0, $0
#	j End
#Else:	add $t1, $0, $t0
#End:

#b)
#	addi $t0, $0, 14
#	addi $t1, $0, 1
#	slt $t2, $t0, $0
#	bne $t2, 1, Else
#	nop
#	sub $t1, $0, $t0
#	j End
#Else:	add $t1, $0, $t0
#End:

#c)
#	addi $t1, $0, 0
#	addi $t0, $0, 1
#Else:	beq $t0, 11, End
#	nop
#	add $t1, $t1, $t0
#	addi $t0, $t0, 1
#	j Else
#End:	

#2a)
	#Se o t1 for menor do que zero, o processo termina. Caso contrário, inverte o valor de t1.

#b)
	#Se t1 for 0, o processo vai entrar num loop infinito, para o corrijir basta:
	#A:	slt $t2, $t1, $zero
	#	bne $t2, $zero, B
	#	nop
	#	sub $t1, $zero, $t1
	#	nop
	#B:

#3a)
	#Mete o t1 a zero e o t0 a 1, enquanto o t0 for menor ou igual do que 11, adiciona o t0 ao t1 e o t0 incrementa, quando o t0 chega a 11, termina.
#b)

#c)
	