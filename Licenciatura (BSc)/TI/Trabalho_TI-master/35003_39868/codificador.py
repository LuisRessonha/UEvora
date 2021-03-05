import LZW_bin_Convert
import hamming_7_4


mensagem = ""
while True:
    try:
        mensagem += input() + "\n"
    except EOFError:
        break


# compress: 'ABC' -> [65, 66, 67]
compressed = LZW_bin_Convert.compress(mensagem)

# dec -> bin    |   convert: [65, 66, 67] -> 010000010100001001000011
bin_A = LZW_bin_Convert.dec_bin(compressed)

# bin -> hamming    |   |   convert: 010000010100001001000011 -> 010-0--000-1--010-0--001-0--010-0--001-1--
ham_A = hamming_7_4.cod_hamming(bin_A)
#print(ham_A)


##########################################################################
##########################################################################

# hammig -> 'ABC' | 010-0--000-1--010-0--001-0--010-0--001-1--  ->  ABC
ham_char = hamming_7_4.ham_char(ham_A)
print(ham_char)

