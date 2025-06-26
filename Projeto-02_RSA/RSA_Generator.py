from sympy import randprime
import hashlib
import base64
import random

LIMIT = 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084095
E = 65537

class ExtendedMessage:
    def __init__(self, encryptedMessage_, encryptedHash_):
        self.encryptedMessage = encryptedMessage_
        self.encryptedHash = encryptedHash_

class KeySet:
    def __init__(self, publicKey: tuple, privateKey: tuple):
        self.publicKey = publicKey
        self.privateKey = privateKey

def GreatestCommonDivider(a, b):
    #Returns the gcd of a and b
    if (b == 0):
        return a
    else:
        return GreatestCommonDivider(b, a % b)

def ExtendedGCD(a, b):
    #Returns the gcd, coefficient of a, and coefficient of b
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
    print("Values chosen:\n'p' = ", primeNo1 ,"\n'q' = ", primeNo2)
    primesProduct = primeNo1 * primeNo2
    print("'n' (p*q) = ", primesProduct)
    phi = (primeNo1 - 1) * (primeNo2 - 1)
    print("phi (p-1)*(q-1) = ", phi)
    gcd, tempD, y = ExtendedGCD(E, phi)
    if (tempD < 0):
        d = tempD + phi
    else:
        d = tempD

    print("'d' has been chosen: d = ", d)
    return KeySet(publicKey=(E, primesProduct), privateKey=(d, primesProduct))

def squareAnd_Multiply(base, exponent, modulus):

    result = 1
    current_base = base

    while exponent > 0:
        if exponent % 2 == 1:
            result = result * current_base
            result %= modulus
        
        current_base = current_base * current_base
        current_base %= modulus
        
        exponent //= 2

    return result

def applyKey(input, type, keySet: KeySet):
    if type=="private":
        #return (input**keySet.privateKey[0]) % keySet.privateKey[1]
        return squareAnd_Multiply(input, keySet.privateKey[0], keySet.privateKey[1])
    elif type=="public":
        #return (input**keySet.publicKey[0]) % keySet.publicKey[1]
        return squareAnd_Multiply(input, keySet.publicKey[0], keySet.publicKey[1])

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
    print("\n[INTEGRITY CHECK]\nHash Attempt: ", hashCheckAttempt,
          "\nRecieved Hash: ", messageHash, "\n\n")
    if(hashCheckAttempt == messageHash):
        return True
    else:
        return False
    
def cipherMessage(originalMessage : str, keySet : KeySet):
    print("Encrypting message...")
    messageInt = MessageBytesToInt(originalMessage.encode())
    print("\nMessage int: ", messageInt)
    encryptedMsgInt = applyKey(messageInt, "private", keySet)
    print("Encrypted message int: ", encryptedMsgInt)
    print("\nParsing to Base64...")
    encryptedMsgB64 = base64.b64encode((encryptedMsgInt).to_bytes(128))
    print("\nCalculating hash...")
    messageHash = GetMessageHash(originalMessage)
    print("\nMessage hash: ", messageHash)
    encgryptedHashInt = int.from_bytes(messageHash)
    encgryptedHashInt = applyKey(int.from_bytes(messageHash), "private", keySet)
    print("Encrypted Hash int: ", encgryptedHashInt)
    encryptedHashB64 = base64.b64encode((encgryptedHashInt).to_bytes(128, "big"))
    print("Encrypted Hash B64: ", encryptedHashB64)
    digSignedMessage = ExtendedMessage(encryptedMsgB64, encryptedHashB64)
    print("\n[SENT MESSAGE] Encrypted message (Base64): ", encryptedMsgB64)
    print("[SENT MESSAGE] Encrypted hash (Base64): ", encryptedHashB64)
    return digSignedMessage

def decipherMessage(extMessageRecieved : ExtendedMessage, keySet : KeySet):
    # Convert both message and signature from Base64 to bytes
    print("\n[RECIEVED MESSAGE] Encrypted message (Base64): ", extMessageRecieved.encryptedMessage)
    print("[RECIEVED MESSAGE] Encrypted hash (Base64): ", extMessageRecieved.encryptedHash)

    print("\nConverting from Base64 and decrypting...")
    decryptedMessageInt = applyKey(int.from_bytes(base64.b64decode(extMessageRecieved.encryptedMessage)), "public", keySet)
    decryptedHashInt = int.from_bytes(base64.b64decode(extMessageRecieved.encryptedHash))
    decryptedHashInt = applyKey(decryptedHashInt, "public", keySet)
    decryptedMessage = MessageBytesToString((decryptedMessageInt).to_bytes(128, "big"))
    cleanedDecryptedMessage = decryptedMessage.strip('\x00')
    decryptedHash = (decryptedHashInt).to_bytes(32, "big")

    if(HashComparison(cleanedDecryptedMessage, decryptedHash)):
        print("INTEGRITY CHECK: TRUE")
    else:
        print("INTEGRITY CHECK: FALSE")
    return cleanedDecryptedMessage

usedKeyset = GenerateKeySet()

originalMessage = "This is a message"
print("Original message: ", originalMessage)

extendedMessage = cipherMessage(originalMessage, usedKeyset)
decipheredMessage = decipherMessage(extendedMessage, usedKeyset)
print("Decrypted message: ", decipheredMessage)
input()
