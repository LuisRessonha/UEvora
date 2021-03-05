def cod_hamming(bin_A):
    temp = ""
    lista_mod = []
    list_temp = []

    # convert:  010000010100001001000011 -> [0100, 0001, 0100, 0010, 0100, 0011]
    for i in range( len(bin_A) ):
        if i % 4 == 0:
            lista_mod.append(temp)
            temp = bin_A[i]
        else:
            temp += bin_A[i]

    lista_mod.append(temp) #da' append ao ultimo conjunto de bits
    lista_mod.pop(0) #retira o 1o append (que estava vazio)  (  )


    # convert: [0100, 0001, 0100, 0010, 0100, 0011] -> [010-0--, 000-1--, 010-0--, 001-0--, 010-0--, 001-1--] 
    for i in range( len(lista_mod) ):
        list_temp = lista_mod[i]
        lt_len = len(list_temp) 
        P1 = str(  int(list_temp[lt_len-1], 2) ^ int(list_temp[lt_len-2], 2) ^ int(list_temp[lt_len-4], 2)  )
        P2 = str(  int(list_temp[lt_len-1], 2) ^ int(list_temp[lt_len-3], 2) ^ int(list_temp[lt_len-4], 2)  )
        P3 = str(  int(list_temp[lt_len-2], 2) ^ int(list_temp[lt_len-3], 2) ^ int(list_temp[lt_len-4], 2)  )
        list_temp = list_temp[lt_len-4] + list_temp[lt_len-3] + list_temp[lt_len-2] + P3 + list_temp[lt_len-1] + P2 + P1
        
        lista_mod[i] = list_temp
    """    
    bin_A = ""
    # convert: [010-0--, 000-1--, 010-0--, 001-0--, 010-0--, 001-1--] -> "010-0--000-1--010-0--001-0--010-0--001-1--"
    for i in range( len(lista_mod) ):
        bin_A = bin_A + lista_mod[i]

    return bin_A 
    """
    return lista_mod

#####################################################################################

def ham_char(lista_mod):
    mensagem = ""
    for i in lista_mod:
        mensagem += chr(int(i, 2))

    return mensagem


#####################################################################################

def v_erro(lista):

    for i in range( len(lista) ):   
        data = lista[i]

        D7 = int( data[0], 2 )
        D6 = int( data[1], 2 )
        D5 = int( data[2], 2 )
        P3 = int( data[3], 2 )
        D3 = int( data[4], 2 )
        P2 = int( data[5], 2 )
        P1 = int( data[6], 2 )

        PP3 = P3 ^ D5 ^ D6 ^ D7
        PP2 = P2 ^ D3 ^ D6 ^ D7
        PP1 = P1 ^ D3 ^ D5 ^ D7

        erro = ""
        erro = erro + str(PP3) + str(PP2) + str(PP1)
        erro_int = int(erro, 2)
        
        if erro_int == 1:
            P1 = (P1 + 1) % 2
        
        if erro_int == 2:
            P2 = (P2 + 1) % 2

        if erro_int == 3:
            D3 = (D3 + 1) % 2

        if erro_int == 4:
            P3 = (P3 + 1) % 2

        if erro_int == 5:
            D5 = (D5 + 1) % 2

        if erro_int == 6:
            D6 = (D6 + 1) % 2

        if erro_int == 7:
            D7 = (D7 + 1) % 2
            
        lista[i] = str(D7) + str(D6) + str(D5) + str(P3) + str(D3) + str(P2) + str(P1) 
        
    return lista

#####################################################################################

def decod_hamming(bin_A):
    temp = ""
    lista_mod = []
    list_temp = []

    # convert:  "010-0--000-1--010-0--001-0--010-0--001-1--" -> [010-0--, 000-1--, 010-0--, 001-0--, 010-0--, 001-1--]
    for i in range( len(bin_A) ):
        if i % 7 == 0:
            lista_mod.append(temp)
            temp = bin_A[i]
        else:
            temp = temp + bin_A[i]

    lista_mod.append(temp) #da' append ao ultimo conjunto de bits
    lista_mod.pop(0) #retira o 1o append (que estava vazio) 

    # verifica se existe 1 erro e corrige
    lista_mod = v_erro(lista_mod)

    # convert:  [010-0--, 000-1--, 010-0--, 001-0--, 010-0--, 001-1--] -> [0100, 0001, 0100, 0010, 0100, 0011]
    for i in range( len(lista_mod) ):
        list_temp = lista_mod[i]
        lt_len = len(list_temp)  

        list_temp = list_temp[lt_len-7] + list_temp[lt_len-6] + list_temp[lt_len-5] + list_temp[lt_len-3] 
        
        lista_mod[i] = list_temp

    # convert:  [0100, 0001, 0100, 0010, 0100, 0011] -> 010000010100001001000011
    bin_A  = ""
    for i in lista_mod:
        bin_A = bin_A + i
        
    return bin_A



def char_ham(abc):
    mensagem = ""
    for i in abc:
        mensagem += format(ord(i), '07b')

    return mensagem