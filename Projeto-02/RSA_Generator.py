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
    def __init__(self, encryptedOriginalMessage:int, digitalSignature:int):
        self.encryptedOriginalMessage = encryptedOriginalMessage
        self.digitalSignature = digitalSignature

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def cipherMessage(originalMessage : str, keySet : KeySet):
    print("Encrypting message...")
    messageInB64 = StringToBase64(originalMessage)
    encryptedMessage = int.from_bytes(messageInB64)**keyset.privateKey[0] % keyset.privateKey[1]
    print("Encrypted message: ", encryptedMessage)
    messageHash = hashFunction(originalMessage)
    print("Message hash: ", messageHash)
    digitalSignature = int.from_bytes(messageHash)**keySet.privateKey[0] % keySet.privateKey[1]
    print("Encrypted hash (digital signature): ", digitalSignature)
    extMessage = ExtendedMessage(encryptedMessage, digitalSignature)
    print("Extended message formed.")
    return extMessage

def decipherMessage(extMessage : ExtendedMessage, keySet : KeySet):
    msgTemp = extMessage.encryptedOriginalMessage**keySet.publicKey[0] % keySet.publicKey[1]
    decryptedMessageB64 = msgTemp.to_bytes(64)
    decryptedMessageString = Base64ToString(decryptedMessageB64)
    msgTemp = extMessage.digitalSignature**keySet.publicKey[0] % keySet.publicKey[1]
    decryptedHashB64 = msgTemp.to_bytes(64)
    decryptedHashString = Base64ToString(decryptedHashB64)

    if(HashCheck(decryptedMessageString, decryptedHashString)):
        print("AUTHENTICITY CHECK: TRUE")
    else:
        print("AUTHENTICITY CHECK: FALSE")
    print("Original message: ", decryptedMessageString)
    return decryptedMessageString
    
def HashCheck(originalMessage : str, messageHash : str):
    hashCheckAttempt = hashFunction(originalMessage)
    if(hashCheckAttempt == messageHash):
        return True
    else:
        return False

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

def StringToBase64(someString : str):
    stringInBytes = str.encode(someString)
    print("[StoB64] String in Bytes: ", stringInBytes)
    stringInB64 = base64.b64encode(stringInBytes, altchars=None)
    print("[StoB64] String in Base64: ", stringInB64)
    return stringInB64

def Base64ToString(someB64 : bytes):
    B64ToBytes = base64.b64decode(someB64, altchars=None)
    print("[B64BacktoS] Base64 back to Bytes: ", B64ToBytes)
    B64ToString = B64ToBytes.decode('utf-8')
    print("[B64BacktoS] Base64 back to String: ", B64ToString)
    return B64ToString

def hashFunction(plaintextMsg: str):
    messageInB64 = StringToBase64(plaintextMsg)
    messageHash = hashlib.sha3_256()
    messageHash.update(messageInB64)
    return messageHash.digest()

keyset = RSA_Generator()

originalMessage = "message"
print("Original message: ", originalMessage)

extendedMessage = cipherMessage(originalMessage, keyset)

decipheredMessage = decipherMessage(extendedMessage, keyset)