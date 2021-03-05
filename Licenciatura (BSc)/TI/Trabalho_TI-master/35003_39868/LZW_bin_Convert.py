def compress(uncompressed):
    """Compress a string to a list of output symbols."""
 
        # Build the dictionary.
#    dict_size = 123
#    dictionary = dict((chr(i), i) for i in range(dict_size))
    
    dict_size = 11
    letras = ['b','e','f','k','n','o','r','u','w','-','\n']
    dictionary = dict((letras[i], i) for i in range(dict_size))


 
    w = ""
    result = []
    for c in uncompressed:
        wc = w + c
        if wc in dictionary:
            w = wc
        else:
            result.append(dictionary[w])
            # Add wc to the dictionary.
            dictionary[wc] = dict_size
            dict_size += 1
            w = c
 
    # Output the code for w.
    if w:
        result.append(dictionary[w])
    return result

#####################################################################################

def decompress_B(compressed):

        # Build the dictionary.
#    dict_size = 123
#    dictionary = dict((i, chr(i)) for i in range(dict_size))
    
    dict_size = 11
    letras = ['b','e','f','k','n','o','r','u','w','-','\n']
    dictionary = dict((i, letras[i]) for i in range(dict_size))
        
 
#########
    resultado = ""
    string = ""

    for code in compressed:
        if not (code in dictionary):
            dictionary[code] = string + (string[0])
        
        resultado += dictionary[code]
        
        if not(len(string) == 0):
            dictionary[dict_size] = string + (dictionary[code][0])
            dict_size += 1
        
        string = dictionary[code]
#########
    """
    actual = chr(compressed.pop(0))
    resultado += actual
    for l in compressed:
        if l in dictionary:
            temp_l = dictionary[l]
        elif l == dict_size:
            temp_l = actual + actual[0]
        else:
            raise print("\n", "algo nao esta' no dict", "\n l: ", l, "\n temp_l: ", temp_l, "\n actual: ", actual) 

        resultado += temp_l

        dictionary[dict_size] = actual + temp_l[0]
        dict_size +=1

        actual = temp_l
    """


    return resultado

#####################################################################################

# dec -> bin
def dec_bin(compressed):
    bin_A  = ""

    for i in compressed:
        bin_A = bin_A + format(i, '08b')

    return bin_A

#####################################################################################

# bin -> dec
def bin_dec(bin_A):
    temp = ""
    lista_mod = []

    # convert:  010000010100001001000011 -> [01000001, 01000010, 01000011]
    for i in range( len(bin_A) ):
        if i % 8 == 0:
            #print(temp)
            lista_mod.append(temp)
            temp = bin_A[i]
        else:
            temp = temp + bin_A[i]

    lista_mod.append(temp) #da' append ao ultimo conjunto de bits
    lista_mod.pop(0) #retira o 1o append (que estava vazio)

    # convert: [01000001, 01000010, 01000011] -> [65, 66, 67] 
    for i in range( len(lista_mod) ):
        lista_mod[i] = int(lista_mod[i], 2)

    return lista_mod
