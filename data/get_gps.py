#!/usr/bin/env python3
from serial import Serial
from serial.tools.list_ports import comports, grep
from the_collector import Collector
import sys

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

    # if msg[0] == '$' and msg[-3] == '*':
    #     msgs.append(msg)

    return msg


def grab(num):
    CR = '\r'
    LF = '\n'

    try:
        ports = comports()
        port = None
        for p in ports:
            if "usbmodem" in p.device:
                port = p.device
                break
        print(port)
        ser = Serial(port, 1000000)

        msgs = []

        for i in range(num):
            msg = grab_msg(ser)

            if msg[0] == '$' and msg[-3] == '*':
                msgs.append(msg)
    except Exception as e:
        print(e)
    finally:
        ser.close()
    return msgs


def main():
    num = 10
    argc = ''
    if len(sys.argv) > 1:
        argc = sys.argv[1]

    if 'h' in argc:
        print("get_gps.py [options]")
        print("  h      help")
        print("  n num  number of messages to get, default 10")
        print("  s      save data to file")
        return 0
    elif 'n' in argc:
        if len(sys.argv) >= 3:
            num = int(sys.argv[2])
        else:
            print("*** Need to specify number of points to collect ***")
            return 1

    msgs = grab(num)
    for m in msgs:
        print(m)

    if 's' in argc:
        info = {
            "gps": "Adafruit Ultimage GPS v3"
        }
        c = Collector()
        c.timestamp = False
        c.write("raw/data.gzip", msgs, info)

    return 0

ret = main()
exit(ret)