from sympy import randprime
import hashlib
import base64
import time
import sys
import random

#LIMIT = 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216
LIMIT = 128
E = 65537

class ExtendedMessage:
    def __init__(self, encryptedOriginalMessage:str, digitalSignature:str):
        self.encryptedOriginalMessage = encryptedOriginalMessage
        self.digitalSignature = digitalSignature

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def cipherMessage(originalMessage : str, keySet):
    encryptedMessage = originalMessage**keyset.privateKey[0] % keyset.privateKey[1]
    messageHash = hashFunction(originalMessage)
    digitalSignature = messageHash**keySet.privateKey[0] % keySet.privateKey[1]
    extMessage = ExtendedMessage(encryptedMessage, digitalSignature)
    return extMessage

def RSA_Generator():
    print("Generating RSA...")
    p = randprime(a=1, b=LIMIT)
    q = randprime(a=1, b=LIMIT)
    print("Values chosen:\np = ", p ,"\nq = ", q)
    n = p * q
    print("n (p*q) = ", n)
    phi = (p - 1) * (q - 1)
    print("phi (p-1)*(q-1) = ", phi)

    d = 0
    print("Determining 'd'...")
    while True:
        if (d * E) % phi == 1:
            break
        d += 1
    print("'d' has been chosen. 'd' = ", d)
    
    return KeySet(publicKey=(E, n), privateKey=(d, n))

def hashFunction(plaintextMsg: str):
    plaintextBytes = str.encode(plaintextMsg)
    plaintextB64 = base64.b64encode(plaintextBytes, altchars=None)
    messageHash = hashlib.sha3_256()
    messageHash.update(plaintextB64)
    return messageHash.digest()

keyset = RSA_Generator()

originalMessage = "message"
print("Plaintext: ", originalMessage)

messageHash = hashFunction(originalMessage)
digitalSignature = messageHash**keyset.publicKey[0] % keyset.publicKey[1]

cipherText = originalMessage**keyset.publicKey[0] % keyset.publicKey[1]
print("Ciphertext: ", cipherText)

translated = cipherText**keyset.privateKey[0] % keyset.privateKey[1]
print("Decrypted ciphertext: ", translated)