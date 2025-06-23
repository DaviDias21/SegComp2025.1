import hashlib
import base64

def hashFunction(plaintextMsg: str):
    plaintextBytes = str.encode(plaintextMsg)
    plaintextB64 = base64.b64encode(plaintextBytes, altchars=None)
    messageHash = hashlib.sha3_256()
    messageHash.update(plaintextB64)
    print(messageHash.digest())

hashFunction("plaintext")
