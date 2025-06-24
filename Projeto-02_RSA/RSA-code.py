import hashlib
import base64

def hashFunction(plaintextMsg: str):
    plaintextBytes = str.encode(plaintextMsg)
    plaintextB64 = base64.b64encode(plaintextBytes, altchars=None)
    messageHash = hashlib.sha3_256()
    messageHash.update(plaintextB64)
    print(messageHash.digest())

test = "message"
print(test)
msgBytes = test.encode()
print(msgBytes)
msgB64 = base64.b64encode(msgBytes)
print(msgB64)
msgBackBytes = base64.b64decode(msgB64)
print(msgBackBytes)
msgBackString = msgBackBytes.decode()
print(msgBackString)