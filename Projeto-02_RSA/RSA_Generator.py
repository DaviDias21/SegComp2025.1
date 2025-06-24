from sympy import randprime
import hashlib
import base64

#LIMIT = 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216
LIMIT = 1024
E = 65537

class ExtendedMessage:
    def __init__(self, encryptedOriginalMessage, digitalSignature):
        self.encryptedOriginalMessage = encryptedOriginalMessage
        self.digitalSignature = digitalSignature

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def cipherMessage(originalMessage : str, keySet : KeySet):
    print("Encrypting message...")
    print("\n[CONVERTING STRING TO BYTES THEN BASE64]")
    messageInB64 = StringToBase64(originalMessage)
    print("[STRING CONVERTED TO BASE64]\n")
    messageIntFromB64 = int.from_bytes(messageInB64)
    print("Message int from Base64 (before encryption): ", messageIntFromB64)
    encryptedMessage = (messageIntFromB64)**keyset.privateKey[0] % keyset.privateKey[1]
    print("Message int after encryption: ", encryptedMessage)
    messageHash = hashFunction(originalMessage)
    print("Message hash (bytes): ", messageHash)
    digitalSignature = (int.from_bytes(messageHash))**keySet.privateKey[0] % keySet.privateKey[1]
    print("[SENT MESSAGE] Encrypted message (B64 to int): ", encryptedMessage)
    print("[SENT MESSAGE] Encrypted hash (digital signature): ", digitalSignature)
    extMessageSent = ExtendedMessage(encryptedMessage, digitalSignature)
    print("Extended message formed.")
    return extMessageSent

def decipherMessage(extMessageRecieved : ExtendedMessage, keySet : KeySet):
    print("[RECIEVED MESSAGE] Encrypted message: ", extMessageRecieved.encryptedOriginalMessage)
    print("[RECIEVED MESSAGE] Encrypted hash (digital signature): ", extMessageRecieved.digitalSignature)
    decryptedMessageInt = (extMessageRecieved.encryptedOriginalMessage)**keySet.publicKey[0] % keySet.publicKey[1]
    print("[RECIEVED MESSAGE] Decrypted message (int from Base64): ", decryptedMessageInt)
    decryptedMessageB64 = (decryptedMessageInt).to_bytes(8)
    print("[RECIEVED MESSAGE] Decrypted message (back to Base64): ", decryptedMessageB64)
    decryptedMessageString = Base64ToString(decryptedMessageB64)
    msgTemp = (extMessageRecieved.digitalSignature)**keySet.publicKey[0] % keySet.publicKey[1]
    decryptedHashB64 = (msgTemp).to_bytes(8)
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
    stringInBytes = someString.encode()
    print("[StoB64] String in Bytes: ", stringInBytes)
    stringInB64 = base64.b64encode(stringInBytes, altchars=None)
    print("[StoB64] String in Base64: ", stringInB64)
    return stringInB64

def Base64ToString(someB64 : bytes):
    print("SomeB64: ", someB64)
    B64ToBytes = base64.b64decode(someB64)
    print("[B64BacktoS] Base64 back to Bytes: ", B64ToBytes)
    B64ToString = B64ToBytes.decode()
    print("[B64BacktoS] Base64 back to String: ", B64ToString)
    return B64ToString

def hashFunction(plaintextMsg: str):
    print("[INSIDE HASHFUNCTION]")
    messageInB64 = StringToBase64(plaintextMsg)
    messageHash = hashlib.sha3_256()
    messageHash.update(messageInB64)
    print("[HASHFUNCTION RETURNED]")
    return messageHash.digest()

keyset = RSA_Generator()

originalMessage = "a"
print("Original message: ", originalMessage)

extendedMessage = cipherMessage(originalMessage, keyset)
decipheredMessage = decipherMessage(extendedMessage, keyset)
