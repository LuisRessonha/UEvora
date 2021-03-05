# Maximo divisor comum

def mdc(a, b):
    if a == b:
        return a
    elif b > a:
        return mdc(a, b-a)
    
    return mdc(a-b, b)

def main():
    print(mdc(24, 9))
