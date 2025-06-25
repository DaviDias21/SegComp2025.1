from sympy import randprime
import hashlib
import base64
import random

#LIMIT = 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216
LIMIT = 1024
E = 65537

class ExtendedMessage:
    def __init__(self, encryptedMessage_, digitalSignature_):
        self.encryptedMessage = encryptedMessage_
        self.digitalSignature = digitalSignature_

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def GreatestCommonDivider(a, b):
    """
    Performs the Euclidean algorithm and returns the gcd of a and b
    """
    if (b == 0):
        return a
    else:
        return GreatestCommonDivider(b, a % b)

def ExtendedGCD(a, b):
    """
    Performs the extended Euclidean algorithm
    Returns the gcd, coefficient of a, and coefficient of b
    """
    x, old_x = 0, 1
    y, old_y = 1, 0

    while (b != 0):
        quotient = a // b
        a, b = b, a - quotient * b
        old_x, x = x, old_x - quotient * x
        old_y, y = y, old_y - quotient * y

    return a, old_x, old_y
        
def GenerateKeySet():
    print("Generating Key Set...")
    primeNo1 = randprime(1, LIMIT)
    primeNo2 = randprime(1, LIMIT)
    print("Values chosen:\np = ", primeNo1 ,"\nq = ", primeNo2)
    primesProduct = primeNo1 * primeNo2
    print("n (p*q) = ", primesProduct)
    totient = (primeNo1 - 1) * (primeNo2 - 1)
    print("totient (p-1)*(q-1) = ", totient)
    GreatestCommonDivider, tempD, y = ExtendedGCD(E, totient)
    if (tempD < 0):
        d = tempD + totient
    else:
        d = tempD

    print("'d' has been chosen: d = ", d)
    return KeySet(publicKey=(E, primesProduct), privateKey=(d, primesProduct))

def MessageBytesToInt(someMessage : bytes):
    messageBytesToInt = int.from_bytes(someMessage)
    return messageBytesToInt

def MessageBytesToString(someMessage : bytes):
    messageBackToString = someMessage.decode()
    return messageBackToString

def GetMessageHash(plaintextMsg: str):
    messageInBytes = plaintextMsg.encode()
    messageHash = hashlib.sha3_256()
    messageHash.update(messageInBytes)
    return messageHash.digest()

def HashComparison(messageString : str, messageHash : bytes):
    hashCheckAttempt = GetMessageHash(messageString)
    print("Decrypted message: ", messageString, "\n\nHash Attempt: ", hashCheckAttempt, "\nOriginal Hash: ", messageHash)
    if(hashCheckAttempt == messageHash):
        return True
    else:
        return False
    
def cipherMessage(originalMessage : str, keySet : KeySet):
    print("Encrypting message...")
    messageInt = MessageBytesToInt(originalMessage.encode())
    print("\nMessage int: ", messageInt)
    encryptedInt = ((messageInt)**keySet.privateKey[0]) % keySet.privateKey[1]
    print("Encrypted message int: ", encryptedInt)
    print("\nParsing to Base64...")
    encryptedMessageB64 = base64.b64encode((encryptedInt).to_bytes(8))
    print("\nCalculating digital signature...")
    messageHash = GetMessageHash(originalMessage)
    print("\nMessage hash: ", messageHash)
    digitalSignatureInt = ((int.from_bytes(messageHash))**keySet.privateKey[0]) % keySet.privateKey[1]
    print("DigSign int: ", digitalSignatureInt)
    digitalSignatureB64 = base64.b64encode((digitalSignatureInt).to_bytes(16, "big"))
    print("DigSign B64: ", digitalSignatureB64)
    extMessageSent = ExtendedMessage(encryptedMessageB64, digitalSignatureB64)
    print("\n[SENT MESSAGE] Encrypted message (Base64): ", encryptedMessageB64)
    print("[SENT MESSAGE] Digital signature (Base64): ", digitalSignatureB64)
    return extMessageSent 


def decipherMessage(extMessageRecieved : ExtendedMessage, keySet : KeySet):
    # Convert both message and signature from Base64 to bytes
    print("\n[RECIEVED MESSAGE] Encrypted message (Base64): ", extMessageRecieved.encryptedMessage)
    print("[RECIEVED MESSAGE] Digital signature (Base64): ", extMessageRecieved.digitalSignature)

    print("\nConverting from Base64 and decrypting...")
    decryptedMessageInt = ((int.from_bytes(base64.b64decode(extMessageRecieved.encryptedMessage)))**keySet.publicKey[0]) % keySet.publicKey[1]
    decryptedSignatureInt = (int.from_bytes(base64.b64decode(extMessageRecieved.digitalSignature)))
    print("DigSign int: ", decryptedSignatureInt)
    decryptedMessage = MessageBytesToString((decryptedMessageInt).to_bytes(8, "big"))
    cleanedDecryptedMessage = decryptedMessage.rstrip('\x00')
    decryptedSignature = (decryptedSignatureInt).to_bytes(16, "big")

    if(HashComparison(cleanedDecryptedMessage, decryptedSignature)):
        print("AUTHENTICITY CHECK: TRUE")
    else:
        print("AUTHENTICITY CHECK: FALSE")
    return decryptedMessage

usedKeyset = GenerateKeySet()

originalMessage = "me"
print("Original message: ", originalMessage)

extendedMessage = cipherMessage(originalMessage, usedKeyset)
decipheredMessage = decipherMessage(extendedMessage, usedKeyset)
print("Original message: ", decipheredMessage)

input()