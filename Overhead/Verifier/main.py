import serial
from time import sleep
import time
from random import randbytes
import hmac
import sys
import time
from Crypto.Hash import HMAC, SHA256
from nacl.signing import SigningKey
from nacl.encoding import HexEncoder
from hashlib import sha256
import ecdsa

from pwn import *

# --------------------------- CRYTO Option ------------------------------------
CRYPTO_HMAC_SHA256 = 0
CRYPTO_ED25519 = 1
#CRYPTO_SPHNICS = 2
CRYPTO_ECC = 3
CRYPTO = CRYPTO_ECC
# -----------------------------------------------------------------------------

# --------------------------- KEYS ------------------------------------
HMAC_KEY = b'\x00'*16

ECC_SK_STR = b'\x89\x8b\x0c\xde\xab\xb5\x86\x97\x3c\x0d\xa9\x15\x8e\x01\x84\xe7\x43\xf7\x7c\x35\xb9\xc2\xb8\xf7\x93\x62\x92\x87\xca\x59\x8b\xb2'
ECC_SK = ecdsa.SigningKey.from_string(ECC_SK_STR, curve=ecdsa.NIST256p, hashfunc=sha256)

# pk_str = b"t/\xbe\xafO\xf4\x1d'H\xadN#\x84y\xa4Bh\x8b\xe6\x18\x98E\x99\xf1\x10\x03N\xf6^\xe1\x1eOI\xd0\x8e\xe0S\xa0\xe2}\xb5\t\xf1\xfd.\xa5cp\xc0^ 1\xc5\xc0\xcbK\x01u\xd78\xeb+D\x1b"
ECC_PK_STR = b'\x74\x2F\xBE\xAF\x4F\xF4\x1D\x27\x48\xAD\x4E\x23\x84\x79\xA4\x42\x68\x8B\xE6\x18\x98\x45\x99\xF1\x10\x03\x4E\xF6\x5E\xE1\x1E\x4F\x49\xD0\x8E\xE0\x53\xA0\xE2\x7D\xB5\x09\xF1\xFD\x2E\xA5\x63\x70\xC0\x5E\x20\x31\xC5\xC0\xCB\x4B\x01\x75\xD7\x38\xEB\x2B\x44\x1B'
ECC_PK = ecdsa.VerifyingKey.from_string(ECC_PK_STR, curve=ecdsa.NIST256p, hashfunc=sha256)
assert(ECC_SK.get_verifying_key() == ECC_PK)

ED25519_SK = bytes([0xf5, 0xe9, 0xe2, 0x5e, 0x53, 0x60, 0xaa, 0xd2,
	0xb2, 0xd0, 0x85, 0xfa, 0x54, 0xd8, 0x35, 0xe8,
	0xd4, 0x66, 0x82, 0x64, 0x98, 0xd9, 0xa8, 0x87,
0x75, 0x65, 0x70, 0x5a, 0x8a, 0x3f, 0x62, 0x80])
ED25519_SK = SigningKey(ED25519_SK)
ED25519_PK = bytes([0x28, 0xed, 0xbd, 0xf2, 0x1a, 0x31, 0xfc, 0xff,
	0x25, 0x8a, 0x90, 0x2b, 0xb5, 0xfe, 0x89, 0x2d,
	0x48, 0x5f, 0x48, 0xee, 0xe7, 0xc3, 0x50, 0x1e,
0xf1, 0x77, 0x7e, 0x28, 0x89, 0x7a, 0x4f, 0x16])
assert(bytes(ED25519_SK.verify_key) == ED25519_PK)
ED25519_PK = ED25519_SK.verify_key

SPHINCS_PK = bytes([0x02, 0x34, 0x43, 0xfc, 0x28, 0xcc, 0xbc, 0x06, 0xdb,
                    0x41, 0x72, 0xce, 0xa4, 0x14, 0x27, 0x3c, 0x46, 0x88, 0x57,
                    0x35, 0x41, 0xf4, 0xc3, 0x1c, 0x18, 0xe8, 0x0d, 0x47, 0x44,
                    0x57, 0x35, 0x67])
SPHINCS_SK = bytes([0xcb, 0x7c, 0x7c, 0xfd, 0x6f, 0x9b, 0xc2, 0x4d, 0xd2,
                    0x7a, 0xe2, 0x0f, 0xb1, 0x90, 0x28, 0x6f, 0x94, 0x9c, 0x43,
                    0x73, 0xa1, 0xda, 0xf4, 0x60, 0xd0, 0x37, 0xf3, 0xf5, 0xe9,
                    0x0d, 0xa0, 0x42, 0x02, 0x34, 0x43, 0xfc, 0x28, 0xcc, 0xbc,
                    0x06, 0xdb, 0x41, 0x72, 0xce, 0xa4, 0x14, 0x27, 0x3c, 0x46,
                    0x88, 0x57, 0x35, 0x41, 0xf4, 0xc3, 0x1c, 0x18, 0xe8, 0x0d,
                    0x47, 0x44, 0x57, 0x35, 0x67])

# ---------------------------------------------------------------------

def sphincTest():
    public_key, secret_key = pyspx.sha2_128f.generate_keypair(seed)
    print(public_key)
    print(secret_key)

def genRawRequest():
    counter = 0xff
    # this f should correspond to the address of "test" function on Prover
    # But in current Prv, we hard code it to test anyway
    f =  0x0
    inp = 0x11
    req = p32(counter) + p32(f) + p8(inp)
    return req

def genRequest():
    req = genRawRequest()
    h = SHA256.new(req)
    print('Hash:', h.hexdigest())
    if CRYPTO == CRYPTO_HMAC_SHA256:
        token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
        print('HMAC:', token.hexdigest())
        return req, token.digest()
    elif CRYPTO == CRYPTO_ECC:
        # Sign uses SHA256 in the underlying algorithm
        token = ECC_SK.sign_deterministic(req)
        print('ECC Sig:', token.hex(), len(token))
        return req, token
    elif CRYPTO == CRYPTO_ED25519:
        token = ED25519_SK.sign(h.digest())
        token = token[:64]
        print('Sig:', HexEncoder.encode(token))
        return req, token

def verifyRequest(req, token):
    h = SHA256.new(req)
    if CRYPTO == CRYPTO_HMAC_SHA256:
        token2 = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
        return token == token2.digest()
    elif CRYPTO == CRYPTO_ED25519:
        return ED25519_PK.verify(h.digest(),token)
    return 100

if __name__ == '__main__':
    ser = serial.Serial(
        port="COM9", baudrate=921600, bytesize=8, stopbits=serial.STOPBITS_ONE
    )
    if not ser.isOpen():
        ser.open()
    try:
        if ser.isOpen():
            print("Serial com is opened")
            print(ser.name)

            print("recv: ", ser.readline())


            req, token = genRequest()
            reqOut = req + token
            print("Req: ", req, "token: ", token)
            ser.write(reqOut)

            start = time.time()

            while True:
                line = ser.readline()
                print('2', line)
                print('Take: ', time.time()-start, 'sec')
                if b'PoX' in line:
                    start = time.time()
    except KeyboardInterrupt:
        ser.close()
        if not ser.isOpen():
            print("User,Serial comm is closed")
    finally:
        ser.close()
        if not ser.isOpen():
            print("Program,Serial comm is closed")