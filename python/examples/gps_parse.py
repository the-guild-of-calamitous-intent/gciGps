#!/usr/bin/env python3
from serial import Serial
from serial.tools.list_ports import comports, grep
from collections import namedtuple

"""
$GPGGA,234804.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*51
$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
$GPRMC,234804.000,A,3906.7106,N,12120.3144,W,0.10,107.17,120823,,,A*70
$GPVTG,107.17,T,,M,0.10,N,0.19,K,A*34
$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
$GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
$GPVTG,95.16,T,,M,0.17,N,0.31,K,A*02
"""

def grab_msg(ser):
    CR = '\r'
    LF = '\n'

    buffer = []
    fin = [0,0]
    c = 0
    while (c != '$'):
        c = ser.read(1).decode("utf8")
    fin[0] = c
    buffer.append(c)
    while (fin[1] != CR and fin[0] != LF):
        c = ser.read(1).decode("utf8")
        buffer.append(c)
        fin[1] = fin[0]
        fin[0] = c
    msg = "".join(buffer[:-2]) # remove \CR \LF

    return msg

def main():
    ports = comports()
    port = None
    for p in ports:
        if "usbmodem" in p.device:
            port = p.device
            break
    print(port)
    ser = Serial(port, 1000000)

    try:
        while True:
            msg = grab_msg(ser)
            print(msg)

    except KeyboardInterrupt:
        print("\nctrl-c")
    except Exception as e:
        print(e)
    finally:
        ser.close()

main()