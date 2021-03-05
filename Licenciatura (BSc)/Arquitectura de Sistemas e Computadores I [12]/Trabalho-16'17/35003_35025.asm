.data 

imagem_gray:		.asciiz "/home/sena/Documentos/lena512colorfinal.gray"
imagem_rgb:		.asciiz "/home/sena/Documentos/imagem.rgb"
original:	.space 786432				# Espaço reservado para a imagem
grey:		.space 262144				# Espaço reservado para a imagem cinzenta
final:		.space 262144


.text

Main:

	la $a0, imagem_rgb		# Ler Ficheiro
	jal read_rgb_image	
	nop
				
	la $a0, original
	la $a1, grey				
	jal rgb_to_gray
	nop
	
	
	la $a0, imagem_gray				
	la $a1, final				# Buffer 
	li $a2, 262144				# Buffer
	jal write_gray_image
	nop
	
	
	j end
	nop
	











###############################################################################
# Abre o ficheiro e coloca-o no buffer
# Argumento: localização do ficheiro ($a0)
# Retorno: número de bits para ler ($v0)

read_rgb_image:	

	li $a1, 0					# Ler e abrir o ficheiro na memória
	li $a2, 0					
	li $v0, 13					
	syscall						
	

	move $a0, $v0
	la $a1, original				#Ler a imagem no espaço reservado			
	li $a2, 786432				
	li $v0, 14					
	syscall
				
	li $v0, 16					
	syscall

	jr $ra						
	nop

###############################################################################

# Converte a imagem rgb para cinzento
# Argumento: buffer do ficheiro ($a0), buffer para gravar
# Ficheiro convertido ($a1)

rgb_to_gray:
	
	li $t1, 786432
	
formula:
	
	beqz $t3, fim_formula
	nop
	
	# Formula  I = 0.30R + 0.59G + 0.11B	
	
	lbu $t0, 0($a0)				# Load do vermelho do pixel atual
	mul $t0, $t0, 30			
	div $t0, $t0, 100			
	lbu $t1, 1($a0)				# Load do verde
	mul $t1, $t1, 59			
	div $t1, $t1, 100			
	lbu $t2, 2($a0)      			#Load do azul
	mul $t2, $t2, 11			
	div $t2, $t2, 100			

	add $t1, $t1, $t2			# Somar tudo no mesmo byte e guardar
	add $t0, $t0, $t1			
	sb $t0, 0($a1)				
		
	addi $a0, $a0, 3			# Anda de 3 em 3 (cores)
	addi $a1, $a1, 1			# As 3 cores = 1 (cinzento) guardado em a1
											
											
				
	j formula				#contador
	addi $t3, $t3, -3			
				
fim_formula:		
	
	jr $ra
	nop
###############################################################################


###############################################################################
# Guarda a imagem cizenta no ficheiro
# Argumentos : localização do ficheiro ($a0),  buffer ($a1), tamanho do buffer ($a2)

write_gray_image:

addi $sp, $sp, -12			#guarda os valores dos registos na pilha
sw $s0, 0($sp)				
sw $s1, 4($sp)				
sw $s2, 8($sp)				
				
move $s1, $a1				# Guarda buffer
move $s2, $a2				# Guarda buffer size
				
li $a1, 1				
li $a2, 0				
li $v0, 13				# Syscall para criar ficheiro
syscall					
				
move $s0, $v0				# Store file descri
move $a0, $s0				# Move it to $a0
move $a1, $s1				# Buffer to write
move $a2, $s2				# Buffer size
li $v0, 15
syscall
				
move $a0, $s0				# Load file descriptor to close
li $v0, 16				#Syscall para fechar o ficheiro
syscall	
				
lw $s0, 0($sp)				# Restore registers to
lw $s1, 4($sp)				# the original values...
lw $s2, 8($sp)				#
addi $sp, $sp, 12
				
jr $ra
nop




end: