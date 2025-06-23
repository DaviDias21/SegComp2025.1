from sympy import randprime

LIMIT = 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216
E = 65537

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def RSA_Generator():
    p = randprime(a=1, b=LIMIT)
    q = randprime(a=1, b=LIMIT)
    n = p * q
    phi = (p - 1) * (q - 1)

    d = 0
    while True:
        if d * E % phi == 1:
            break
        d += 1
    
    return KeySet(publicKey=(E, n), privateKey=(d, n))

keyset = RSA_Generator()

plaintext = 5

cypher = plaintext**keyset.publicKey[0] % keyset.publicKey[1]
print(cypher)

translated = cypher**keyset.privateKey[0] % keyset.privateKey[1]
print(translated)