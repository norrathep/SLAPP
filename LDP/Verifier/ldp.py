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

ATTACK_A4 = False

# --------------------------- KEYS ------------------------------------
HMAC_KEY = b'\x00'*16

# ---------------------------------------------------------------------

counter = 0xff

def genInitRequest():
    global counter
    func = 0x80402f1
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

def genLDPRequest():
    global counter
    func = 0x80405b5
    f = 0.23456
    p = 0.1256
    q = 0.8121
    input = floatToBytes(f) + floatToBytes(p) + floatToBytes(q)
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
    #print("Waiting...")
    #out = ser.readline()
    #print('Recv', out)
    #if b'[' not in out and b']' not in out:
    #    return
    #sleep(.1)
    start = time.time()
    if command == 'i':
        req, token, reqWithCommand = genInitRequest()
    else:
        req, token, reqWithCommand = genLDPRequest()
    payload = req + token
    print_bytes(payload)
    print("")
    print("Payload: ", payload, len(payload))
    print('Phase 1 takes:', time.time()-start)
    ser.write(payload)

    prev = b""

    rec = False

    while True:
        out = ser.readline()
        print("Receive: ", out, time.time()-start)
        if command == 'c' and b'[Test]' in out:
            print("Plug in Arduino to proceed")
        if b'[Over]' in out:
            # do something with prev
            output = prev[4:4+32]
            token = prev[32+4+1: 32+5+32]
            print("Output: ", output, type(output))
            #print("Token: ", token)
            #print_bytes(token)
            #print("Done")
            #print_bytes(token)
            #print()

            # Simulate A4 attack which modifies the output
            if command == 'c' and ATTACK_A4:
                output = bytearray([0x11] * len(output))

            rToken = recomputeToken(reqWithCommand, output)
            if rToken == token:
                print("Token verification succeeds")
            else:
                #print("Token verification fails")
                raise Exception("Token verification fails. Aborting")

            print("Command:", command, "takes", time.time()-start)
            return
        elif b'[OS]' in out:
            rec = True

        if rec:
            prev += out


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

    filename = "TRACES_NonSecure.elf"

    text_section_data = read_text_section(filename)
    code_size = 0x1000

    text_section_data = text_section_data[:code_size]
    #print_bytes(text_section_data)
    #print()
    #print(len(text_section_data))

    h.update(text_section_data)

    h.update(output)
    print('Recomputed Hash:', h.hexdigest())

    token = HMAC.new(HMAC_KEY, h.digest(), digestmod=SHA256)
    print('Recomputed Token:', token.hexdigest())
    return token.digest()


if __name__ == '__main__':
    ser = serial.Serial(
        port="COM9", baudrate=921600, bytesize=8, stopbits=serial.STOPBITS_ONE
    )
    if not ser.isOpen():
        ser.open()
    while True:

        if ser.isOpen():
            user_input = input("Type 'i' for Setup and 'c' for Collect: ")
            if user_input.lower() != 'c' and user_input.lower() != 'i':
                continue
            try:
                new_single(user_input.lower())

            except KeyboardInterrupt:
                ser.close()