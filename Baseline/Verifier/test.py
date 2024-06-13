import serial

if __name__ == '__main__':
    ser = serial.Serial(
        port="COM9", baudrate=115200, bytesize=8, stopbits=serial.STOPBITS_ONE
    )
    if not ser.isOpen():
        ser.open()
    try:
        if ser.isOpen():
            print("Serial com is opened")
            print(ser.name)

            while True:
                r = ser.readline()
                print(r)
                if b'REC' in r:
                    ser.write(b'OAK1\n')
    except KeyboardInterrupt:
        ser.close()
        if not ser.isOpen():
            print("User,Serial comm is closed")
    finally:
        ser.close()
        if not ser.isOpen():
            print("Program,Serial comm is closed")