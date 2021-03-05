import LZW_bin_Convert
import hamming_7_4

#-> Mensagem depois do canal
mensagem_a = input()


##########################################################################
##########################################################################

# 'ABC' -> hammig | ABC -> 010-0--000-1--010-0--001-0--010-0--001-1--
mensagem = hamming_7_4.char_ham(mensagem_a)




##########################################################################
##########################################################################


#-> Hamming Canal
ham_B = hamming_7_4.decod_hamming(mensagem)


# bin -> dec    |   convert:  010000010100001001000011 -> [01000001, 01000010, 01000011] -> [65, 66, 67] 
lista_mod = LZW_bin_Convert.bin_dec(ham_B)


# decompress: [65, 66, 67] -> 'ABC' 
decompressed = LZW_bin_Convert.decompress_B(lista_mod)
print('++++++++++++++++++++++++++++++++++')
print (decompressed)