								#Ex 1 a)
#addi $t1, $t1, 3
#addi $t0, $t1, 4

								#Ex 1 c)
#addi $t3, $t3, 3
#addi $t2, $t2, 4
#add $t1, $t3, $t2
#addi $t0, $t1, 5
 
 								#Ex 1 e)
#sll $t1, $t0, 1
#addi $t1, $t1, 1

								#Ex 1 f)
#addi $t0, $t0, 2
#add $t1, $t1, $t0
#sll $t0, $t0, 3
#add $t1, $t0, $t1
#addi $t1, $t1, 5

								#Ex 1 g)
#addi $t0, $t0, 4
#addi $t1, $t0, -1

								#Ex 1 h)
#addi $t0, $t0, 2		#$t0 = 2
#addi $t1, $t1, 3		#$t1 = 3
#sll $t3, $t0, 1			#$t3 = 2 * 2
#sub $t4, $t0, $t1		#$t4 = $t0 - $t1
#sll $t5, $t4, 1			#$t5 = ($t0 - $t1) * 2
#add $t6, $t5, $t4		#$t6 = ($t0 - $t1) * ($t0 - $t1)
#add $t2, $t3, $t6		#$t2 = 4 + (($t0 - $t1) * 3)

								#Ex 2 a)
#addi $t0, $t0, 2		#$t0 = 2
#sll $t2, $t0, 1			#$t2 = 2 * 2
#sub $t1, $t0, $t2		#$t1 = 2 - 4

								#Ex 2 b)
