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
import struct
from elftools.elf.elffile import ELFFile


from pwn import *

ATTACK_AM = True

# --------------------------- KEYS ------------------------------------
HMAC_KEY = b'\x00'*16

# ---------------------------------------------------------------------

counter = 0x88

def genInitRequest():
    global counter
    func = 0x8550
    input =  b'\x00'*12
    reqWithoutCommand = p32(counter) + p32(func) + input
    req = b'[stp]' + reqWithoutCommand
    counter += 1

    h = SHA256.new(req)
    print('Hash:', h.hexdigest())
    token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    print('HMAC:', token.hexdigest())
    return req, token.digest(), reqWithoutCommand

def floatToBytes(f):
    return bytes(struct.pack("<f", f))

def genTrainRequest():
    global counter
    func = 0x89b4
    lr = 1.0
    numEpoch = 100.0
    initWB = 0.2
    input = floatToBytes(lr) + floatToBytes(numEpoch) + floatToBytes(initWB)
    reqWithoutCommand = p32(counter) + p32(func) + input
    req = b'[trn]' + reqWithoutCommand
    counter += 1

    h = SHA256.new(req)
    print('Hash:', h.hexdigest())
    token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    print('HMAC:', token.hexdigest())
    return req, token.digest(), reqWithoutCommand


def genCollectRequest():
    global counter
    func = 0x8650
    input = b'\x00'*12
    reqWithoutCommand = p32(counter) + p32(func) + input
    req = b'[clt]' + reqWithoutCommand
    counter += 1

    h = SHA256.new(req)
    print('Hash:', h.hexdigest())
    token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    print('HMAC:', token.hexdigest())
    return req, token.digest(), reqWithoutCommand

def verifyRequest(req, token):
    h = SHA256.new(req)
    token2 = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    return token == token2.digest()

def print_bytes(bytes_data):
    for byte in bytes_data:
        print("\\x{:02X}".format(byte), end='')

def new_single(command):
    print("Waiting...")
    out = ser.readline()
    print('Recv', out)
    if b'[' not in out and b']' not in out:
        return
    sleep(.1)
    start = time.time()
    if command == 'i':
        req, token, reqWithCommand = genInitRequest()
    elif command == 'c':
        req, token, reqWithCommand = genCollectRequest()
    elif command == 't':
        req, token, reqWithCommand = genTrainRequest()
    else:
        raise Exception("Wrong command: ",command)
    payload = req + token
    print_bytes(payload)
    print("")
    print("Payload: ", payload, len(payload))
    ser.write(payload)

    prev = ""

    while True:
        out = ser.readline()
        print("Receive: ", out)
        if b'[S]' in out or b'[Test]' in out:
            print("Cur time taken", time.time()-start)

        if b'[OS]' in out and b'[OE]' not in out:
            print("HMMM")
            # keep reading until it hits [OE]
            while b'[OE]' not in out:
                out = out + ser.readline()
                print("Receive: ", out)

        if command == 'c' and b'[Test]' in out:
            print("Plug in Arduino to proceed")
        if b'[Over]' in out:
            # do something with prev
            output = prev[4:4+32]
            token = prev[32+4+1: 32+5+32]
            if command == 't':
                weightSize = 800
                output = prev[4:4+weightSize]
                token = prev[weightSize+4+1: weightSize+5+32]

            print("Output: ", output)
            print("Token: ", token)
            #print_bytes(token)
            #print()

            # Simulate A4 attack which modifies the output
            if command == 't' and ATTACK_AM:
                output = b'\x11' * len(output)

            rToken = recomputeToken(reqWithCommand, output)
            if rToken == token:
                print("Token verification succeeds")
            else:
                #print("Token verification fails")
                raise Exception("Token verification fails. Aborting")

            print("Command:", command, "takes", time.time()-start)
            if command == 'c':
                print("You have 10 secs to unplug Arduino.")
            time.sleep(10)
            ser.write(b'OAK')
            return
        prev = out


def read_text_section(filename):
    with open(filename, 'rb') as f:
        elffile = ELFFile(f)

        # Find the .text section
        for section in elffile.iter_sections():
            if section.name == '.text':
                # Read and return the contents of the .text section
                return section.data()

def recomputeToken(reqWithoutCommand, output):

    h = SHA256.new(reqWithoutCommand)

    filename = "NonSecureApp.elf"

    text_section_data = read_text_section(filename)
    #text_section_data += b'\x00'*(0x8000-len(text_section_data))
    #print_bytes(text_section_data)
    #print()
    print(len(text_section_data))

    h.update(text_section_data)
    h.update(output)
    print('Recomputed Hash:', h.hexdigest())


    token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    print('Recomputed Token:', token.hexdigest())
    return token.digest()


if __name__ == '__main__':
    ser = serial.Serial(
        port="COM6", baudrate=115200, bytesize=8, stopbits=serial.STOPBITS_ONE
    )
    if not ser.isOpen():
        ser.open()
    while True:

        if ser.isOpen():
            user_input = input("Type 'i' for Setup, 'c' for Collect and 't' for Train: ")
            if user_input.lower() != 'c' and user_input.lower() != 'i' and user_input.lower() != 't':
                continue
            try:
                new_single(user_input.lower())

            except KeyboardInterrupt:
                ser.close()
        #finally:
            #print('done single')
            #ser.close()
            #if not ser.isOpen():
            #    print("Program,Serial comm is closed")