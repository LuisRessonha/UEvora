# abrir o ficheiro
import sys

fileName = sys.argv[1]
testFile = open(fileName, "r")

# dicionário de palavras
words = dict()
nOfLines = 0

for line in testFile:
        # remove os espaços em braco, i.e espaços e new-line
        line = line.strip()

        if line in words:
            words[line] += 1
        else:
            words[line] = 1
        
        nOfLines +=1

# percentagem da occurencia de uma palavra
def percentage(nOfOccurence):
    decimalValue = nOfOccurence/nOfLines
    return decimalValue*100

for word in words.keys():
    print(f"Number of occurrences of {word} on file: {words[word]}\n"
          f"Percentage of occurrences of {word} on file: {percentage(words[word])}%")