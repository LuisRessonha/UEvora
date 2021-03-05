###############################################################################
#		   Arquitectura de Sistemas e Computadores I		      #
#		    Licenciatura em Engenharia Informatica                    #
# 		Programado por Ricardo Benedito e Rui Rodrigues		      #
#	      Programa para a detecao de contornos de uma imagem    	      #
#		       Total de linhas utilizadas: 288			      #
###############################################################################

.data
	input : .asciiz "/home/ricardo/Documentos/rui/lena.rgb"
	outGray : .asciiz "/home/ricardo/Documentos/rui/lenaCinza.GRAY"
	
	buffer1 : .space 786432				#RGB
	buffer2 : .space 262144				#GRAY
	bufferh : .space 262144				#sobel horizontal
	bufferv : .space 262144				#sobel vertical
	bufferc : .space 262144				#sobel countorn
	
	sobelh : .byte 1, 0, -1, 2, 0, -2, 1, 0, -1
	sobelv : .byte 1, 2, 1, 0, 0, 0, -1, -2, -1

.text

Main:
	la $a0, input
	jal read_rgb_image
	nop
	
	move $a0, $v0
	la $a1, buffer2
	jal rgb_to_gray
	nop
	
	la $a0, buffer2
	la $a1, sobelh
	la $a2, bufferh
	jal convolution
	nop
	
	la $a0, buffer2
	la $a1, sobelv
	la $a2, bufferv
	jal convolution
	nop
	
	la $a0, bufferh
	la $a1, bufferv
	la $a2, bufferc
	jal contour
	nop
	
	la $a0, outGray
	la $a1, bufferc
	li $a2, 262144
	jal write_gray_image
	nop
	
	j KILL
	nop
	
read_rgb_image:
###############################################################################
#Argumentos:	a0 = caminho para o ficheiro
#Retorno:	v0 = buffer com a leitura do RGB
###############################################################################
#Leitura de ficheiro RGB com escrita num buffer de 512x512x3 bytes, que será 
#utilizado no resto do programa.
###############################################################################

	#prepara leitura do ficheiro, OPEN
	li $a1, 0
	li $a2, 0		#read only
	li $v0, 13		#open
	syscall			#syscall para v0 = file descriptor
	
	move $t0, $v0		#guardar o file descriptor
	
	#leitura do ficheiro, READ
	move $a0, $v0		#colocar o retorno do syscall em a0
	la $a1, buffer1		#buffer1
	li $a2, 786432		#numero de bytes ocupados
	li $v0, 14		#read
	syscall
	
	#fecho do ficheiro, CLOSE
	move $a0, $t0		#carregar o file descriptor
	li $v0, 16 		#close
	syscall
	
	la $v0, buffer1		#retorno da funçao
	
	jr $ra
	nop
	
write_gray_image:
###############################################################################
#Argumentos:	a0 = caminho para a escrita do ficheiro
#		a1 = buffer com o conteudo para escrita
#		a2 = comprimento do buffer
###############################################################################
#Escrita para um ficheiro RGB de um buffer em argumento.
###############################################################################
	
	move $t0, $a1
	move $t1, $a2
	
	#Abertura de um ficheiro para escrita
	li $a1, 1		#flags
	li $a2, 0		#permissoes
	li $v0, 13		#open
	syscall			#syscall para v0 = file descriptor
	
	move $a0, $v0		#colocar o retorno do syscall em a0
	move $a1, $t0		#endereco do buffer
	move $a2, $t1		#tamanho (numero de bytes para escrever)
	li $v0, 15		#escrever
	syscall

	jr $ra
	nop
	
	
rgb_to_gray:
###############################################################################
#Argumentos:	a0 = buffer com os bytes RGB
#		a1 = buffer vazio para GRAY
###############################################################################
#Conversao de RGB para GRAY, calcula o valor de um pixel colorido na respetiva
#escala de cinzento, utilizando a seguinte formula: I = 0.30 R + 0.59G + 0.11B
###############################################################################

	addi $t0, $zero, 262144
	
	L:
		beqz $t0, end
		addi $t0, $t0, -1		#decrementar o contador
		
		lbu $t1, 0($a0)			#RED
		lbu $t2, 1($a0)			#GREEN
		lbu $t3, 2($a0)			#BLUE	

		mul $t1, $t1, 30	
		mul $t2, $t2, 59
		mul $t3, $t3, 11
	
		add $t1, $t1, $t2
		add $t1, $t1, $t3

		div $t1, $t1, 100
		sb $t1, 0($a1)

		addi $a0, $a0, 3		#avancar buffer rgb
		addi $a1, $a1, 1		#avancar buffer gray
	
		j L
		nop
	
convolution:
###############################################################################
#Argumentos:	a0 = buffer com a imagem em GrayScale
#		a1 = sobel a aplicar
# 		a2 = buffer para colocar o resultado
###############################################################################
#Calcula a convulcao de uma imagem atraves de um operador sobel em matriz de 
#3x3, e coloca o resultado num buffer previamente criado. Assume-se uma imagem
#com tamanho de 512x512.
#Tentando optimizar a funcao ignora-se a primeira e a ultima linha.
###############################################################################

	addi $sp, $sp, 28
	
	sw $s0, 0($sp)			#Guardar os valores dos registos S
	sw $s1, 4($sp)
	sw $s2, 8($sp)
	sw $s3, 12($sp)
	sw $s4, 16($sp)
	sw $s5, 20($sp)
	sw $s6, 24($sp)
	sw $s7, 28($sp)
	
	lb $s0, 0($a1)			#carregar os valores do sobel
	lb $s1, 1($a1)
	lb $s2, 2($a1)
	lb $s3, 3($a1)
	lb $s4, 5($a1)
	lb $s5, 6($a1)
	lb $s6, 7($a1)
	lb $s7, 8($a1)				
	
	li $t0, 261120			#262144-512, ignorar a ultima linha
	addi $a0, $a0, 512		#ignorar primeira linha
	
	C:
		addi $a0, $a0, 1
		beqz $t0, Cend
		
		lbu $t1, -513($a0)		#aritmetica 
		mul $t1, $t1, $s0
		add $t2, $zero, $t1
		
		lbu $t1, -512($a0)
		mul $t1, $t1, $s1
		add $t2, $t2, $t1

		lbu $t1, -511($a0)
		mul $t1, $t1, $s2
		add $t2, $t2, $t1
		
		lbu $t1, -1($a0)
		mul $t1, $t1, $s3
		add $t2, $t2, $t1
		
		lbu $t1, 1($a0)
		mul $t1, $t1, $s4
		add $t2, $t2, $t1
		
		lbu $t1, 511($a0)
		mul $t1, $t1, $s5
		add $t2, $t2, $t1
		
		lbu $t1, 512($a0)
		mul $t1, $t1, $s6
		add $t2, $t2, $t1
		
		lbu $t1, 513($a0)
		mul $t1, $t1, $s7
		add $t2, $t2, $t1			#fim da aritmetica
		
		abs $t2, $t2
		div $t2, $t2, 4
		
		sb $t2, 0($a2)
		
		addi $a2, $a2, 1
		addi $t0, $t0, -1
		j C
		nop	
		
	Cend:
		lw $s0, 0($sp)			#Repor os registos S
		lw $s1, 4($sp)
		lw $s2, 8($sp)
		lw $s3, 12($sp)
		lw $s4, 16($sp)
		lw $s5, 20($sp)
		lw $s6, 24($sp)
		lw $s7, 28($sp)
		
		addi $sp, $sp, -28
		j end
		nop

contour:
###############################################################################
#Argumentos:	a0 = buffer com aplicacao do sobel horizontal
#		a1 = buffer com aplicacao do sobel vertical
#		a2 = buffer para conter o resultado
###############################################################################
#Calcula os contornos da imagem utilizando uma convulsao horizontal e outra
#vertical, a funçao assume um tamanho de imagem de 512x512.
###############################################################################
	
	li $t0, 262144
	
	Bh:
		beqz $t0, end
		
		lbu $t1, 0($a0)
		lbu $t2, 0($a1) 
	
		add $t1, $t1, $t2
		addiu $t2, $zero, 255
		subu $t1, $t2, $t1
		sb $t1, 0($a2)
		
		addi $a0, $a0, 1
		addi $a1, $a1, 1
		addi $a2, $a2, 1
		
		addi $t0, $t0, -1
		
		j Bh
		nop

end:
	jr $ra
	nop
KILL:		#fim do programa