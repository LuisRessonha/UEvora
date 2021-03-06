#Trabalho Pratico de Arquitetura de Sistemas e Computadores I
#Licenciatura em Engenharia Informatica
#Mariana Pedrosa-32490
#Raquel Gomes-31523
#Ano Lectivo 2013/2014
#Semestre Par


		.data
nome:		.asciiz "/home/raquel/Documentos/Trabalho/lenacolor.rgb"			#Local e nome onde se encontra a imagem a modificar
nomecriado:	.asciiz "/home/raquel/Documentos/Trabalho/lenapretoebranco.gray"		#Nome do ficheiro que vai ser criado apos ser convertido a preto e branco
cor:		.space 786432				#Buffer da imagem a cores
pretobranco:	.space 262144				#Buffer da imagem a preto e branco
SH:		.space 262144				#Buffer do sobel horizontal
SV:		.space 262144				#Buffer do sobel vertical
Sobelh:		.byte 1,0,-1,2,0,-2,1,0,-1		#Buffer da matriz Sh (A)
Sobelv:		.byte 1,2,1,0,0,0,-1,-2,-1		#Buffer da matriz Sv (B)
		.text
Main:
	la $a0, nome			#Faz load do adereco onde esta a imagem a cores rgb
	jal openfich			#Chama a funcao para abrir o ficheiro para leitura
	nop
	
	move $a0, $v0		
	li $a2, 786432			#a2=Numero de bytes para leitura
	la $a1, cor			#Faz o load em a1 do adereco do buffer correspondente a imagem a cores
	jal read_rgb_image		#Chama a funcao que vai ler a imagem no formato rgb para um array em memoria
	nop
	
	la $a0, pretobranco		#Faz o load do buffer da imagem gray
	la $a1, Sobelh			#Faz o load do buffer do sobel horizontal
	la $a2, SH			#Faz o load do buffer da matriz do sobel horizontal
	jal convolution			#Chama a funcao que vai fazer a convolucao horizontal
	nop
	
	la $a0, pretobranco		#Faz o load do buffer da umagem gray
	la $a1, Sobelv			#Faz o load do buffer do sobel vertical
	la $a2, SV			#Faz o load do buffer da matrzi do sobel vertical
	jal convolution			#Chama a funcao que vai fazer a concolucao vertical
	nop
	
	la $a0, nomecriado		#Faz o load em a0 do adresso do buffer da imagem que vai ser escrita
	la $a1, pretobranco		#Faz o load em a1 do adereco do buffer correspondente a imagem a preto e branco 
	li $a2, 262144			#a2=Numero de bytes para leitura
	jal write_gray_image		#Chama a funcao que vai escrever uma imagem em formato GRAY num ficheiro
	nop
	
	j end
	nop
	
	
#Funcao para abrir o ficheiro para leitura	
openfich:	

		addi $sp,$sp,-4		#Incrementa espaco na pilha
		sw $ra,0($sp)		#Guarda na pilha o valor do registo ra	
	
		li $a1,0	
		li $a2,0	
		li $v0,13 		#Abre a imagem a cores em RGB	
		syscall
	
		lw $ra,0($sp) 		#Faz o load do valor do registo ra guardado anteriormente na pilha
		addi $sp,$sp,4		#Liberta espaco na pilha
	
		jr $ra			#Retorna para a funcao main	
		nop	
	
#Funcao que le uma imagem no formato RGB para um array em memoria
#A funcao recebe como argumentos uma string com o nome do cheiro a ler e devolve um buffer com a imagem RGB lida
#A funcao assume dimensao 512 x512.	
read_rgb_image:

	li $v0, 14		#Le a imagem a cores em RGB
	syscall
	li $v0, 16		#Close da imagem a cores
	jr $ra			#Volta para a funcao main
	nop


#Funcao que escreve uma imagem em formato GRAY num ficheiro
#A funcao recebe como argumentos o nome de um ficheiro, um buffer com a imagem e o comprimento do buffer
write_gray_image:

	move $t0, $a1		#Guarda o valor de a1 para o registo t0
	move $t1, $a2		#Guarda o valor do t2 para o registo t1
	li $a1, 1
	li $a2, 0
	li $v0, 13		#Abre a imagem a cores
	syscall
	move $a0, $v0		#Guarda o valor do v0 para o registo a0
	move $a1, $t0		#Guarda o valor do t0 para o registo a1
	move $a2, $t1		#Guarda o valor do t1 para o registo a2
	li $v0, 15		#Escreve o ficheiro da imagem a preto e branco
	syscall
	
	jr $ra			#Volta para a funcao main
	nop



#Funcao que converte uma imagem a cores RGB para uma imagem em tons de cinzento GRAY
#A funcao recebe como argumentos um buffer com a imagem RGB e um buffer onde deve ser colocada a imagem em formato Gray
#A funcao assume internamente que a iamgem tem dimensao 512x512
rgb_to_gray:
	la $t0, cor			#Faz o load do buffer da imagem a cores
	move $t5, $a2			#Guarda o valor do a2, para o registo t5
	
	COMECA:	beqz $t5, ACABA			#Compara o valor do t5 com zero e quando for igual salta para a label
		nop
	
		lbu $t1, ($t0)			#Load do primeiro byte para o registo t1
		lbu $t2, 1($t0)			#Load do segundo byte para o registo t2
		lbu $t3, 2($t0)			#Load do terceiro byte para o registo t3
	
		#I = 0:30Red + 0:59Green + 0:11Blue
		#Formula para obter cada pixel da imagem em tons de cinzento correspondente ao da imagem em RGB.
		mul $t1, $t1, 30		#multiplica por 30 o pixel referente a cor vermelha e guarda o valor em t1
		div $t1, $t1, 100		#divide por 100 para corresponder aos 0.30
	
		mul $t2, $t2, 59		#multiplica por 59 o pixel referente a cor verde e guarda o valor em t2
		div $t2, $t2, 100		#divide por 100 para corresponder aos 0.59
	
		mul $t3, $t3, 11		#multiplica por 11 o pixel referente a cor azul e guarda o valor em t3
		div $t3, $t3, 100		#divide por 100 para corresponder aos 0.11
	
		#Soma da codificacao de cada cor para cada pixel 
		add $t4, $t1, $t2		#Guarda no t4 a soma dos valores das conversoes R e G 
		add  $t4, $t4, $t3		#Adiciona em t4, o valor da soma obtida anteriormente juntamente com o pixel B
	
		sb $t4, ($a1)			#Guarda o o valor do endereco a0, no registo t4
	
		addi $t0, $t0, 3		#Adiciona 3 ao a0, para avan?ar para os tres bytes seguintes
		addi $a1, $a1, 1		#Adiciona 1 ao a1 para avan?ar
		addi $t5, $t5, -1		#Decrementa um ao valor do t5
		j COMECA			#Salta para a fun??o comeca
		nop
	
	ACABA:
		li $v0, 15			#cria o ficheiro da imagem correspondente a preto e branco
		syscall
		jr $ra				#Volta para o local onde foi chamada
		nop
	

#Funcao que calcula a convolucao de uma imagem A com um operador Sobel (matriz 3x3) e coloca o resultado numa matriz B
#Os valores da matriz B devem estar em valor absoluto
#A funcao recebe como argumentos um buffer com a matriz A, um buffer com um dos operadores Sobel e um  buffer que vai converter a imagem filtrada B
#A funcao assume internamente que a imagem tem dimenssao 512 x512.
convolution:

	addi $sp, $sp, -28		#Aloca espaco na pilha para os 8 valores dos registos s
	sw $ra, 0($sp)

	sw $s0, 0($sp)			#Guarda o valor so registo s0 na pilha
	lbu $s0, 0($a1)			#Faz o load do valor do sobel na posicao 0
	
	sw $s1, 4($sp)			#Guarda o valor do registo s1 na pilha
	lbu $s1, 1($a1)			#Faz o load do valor do sobel na posicao 1
	
	sw $s2, 8($sp)			#Guarda o valor do registo s2 na pilha
	lbu $s2, 2($a1)			#Faz o load do valor do sobel na posicao 2
	
	sw $s3, 12($sp)			#Guarda o valor do registo s3 na pilha
	lbu $s3, 3($a1)			#Faz o load do valor do sobel na posicao 3
	
	#Saltamos a posicao 4 porque ? a que se encontra no centro que e de onde partimos
	
	sw $s4, 16($sp)			#Guarda o valor do registo s4 na pilha
	lbu $s4, 5($a1)			#Faz o load do valor do sobel na posicao 5
	
	sw $s5, 20($sp)			#Guarda o valor do registo s5 na pilha
	lbu $s5, 6($a1)			#Faz o load do valor do sobel na posicao 6
	
	sw $s6, 24($sp)			#Guarda o valor do registo s6 na pilha
	lbu $s6, 7($a1)			#Faz o load do valor do sobel na posicao 7
	
	sw $s7, 28($sp)			#Guarda o valor do registo s7 na pilha
	lbu $s7, 8($a1)			#Faz o load do valor do sobel na posicao 8
	
	li $t0, 262144 			#t0=numero de bytes para a convolucao
	
	conv:
		addi $a0, $a0, 1
		beqz $t0, finalconv
		
		lbu $t3, -513($a0)		#Faz o load da primeira posicao da matriz e guarda em t3
		mul $t3, $t3, $s0		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t3, zero
		
		lbu $t3, -512($a0)		#Faz o load da segunda posicao da matriz e guarda em t3
		mul $t3, $t3, $s1		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, -511($a0)		#Faz o load da terceira posicao da matriz e guarda em t3
		mul $t3, $t3, $s2		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, -1($a0)		#Faz o load da quarta posicao da matriz e guarda em t3
		mul $t3, $t3,$s3		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, 1($a0)			#Faz o load da sexta posicao da matriz e guarda em t3
		mul $t3, $t3, $s4		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, 511($a0)		#Faz o load da setima posicao da matriz e guarad em t3
		mul $t3, $t3, $s5		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, 512($a0)		#Faz o load da oitava posicao da matriz e guarda em t3
		mul $t3, $t3, $s6		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		lbu $t3, 513($a0)		#faz o load da nona posicao da matriz e guarda em t3
		mul $t3, $t3, $s7		#Multiplica o valor da posicao pelo valor do sobel correspondente
		add $t4, $t4, $t3
		
		
		addi $t0, $t0, -1
		
		j conv			#Chamada para a funcao conv
		nop
		
	
		
	finalconv:
	
		lw $s0, 0($sp)		#Repoe o valor do registo s0
		lw $s1, 4($sp)		#Repoe o valor do registo s1
		lw $s2, 8($sp)		#Repoe o valor do registo s2
		lw $s3, 12($sp)		#Repoe o valor do registo s3
		lw $s4, 16($sp)		#Repoe o valor do registo s4
		lw $s5, 20($sp)		#Repoe o valor do registo s5
		lw $s6, 24($sp)		#Repoe o valor do registo s6
		lw $s7, 28($sp)		#Repoe o valor do registo s7
		
		
	lw $ra, 0($sp)
	addi $sp, $sp, 28		#Liberta o espa?o da pilha que foi anteriormente ocupado 


#Funcao que calcula a imagem final combinando duas imagens filtradas
#A funcao recebe como argumentos os 2 buffers com as imagens a combinar e 1 buffer que vai ter o resultado final
#A funcao assume que a imagem tem dimensao 512x512
contour:
	


end:		#Fim do programa