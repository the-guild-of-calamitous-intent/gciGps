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

GSA = namedtuple("GSA", "id mode fix prns pdop hdop vdop")
RMC = namedtuple("RMC", "id utc status lat lon speed track data mag")
GGA = namedtuple("GGA", "id utc lat lon qual num_sats hdop msl geoid age ref_id")
time_t = namedtuple("time_t", "hr min sec")
date_t = namedtuple("data_t", "day month year")
sat_t = namedtuple("sat_t", "prn el az snr")



def parseHex(c):
  if (c < '0'):
    return 0
  if (c <= '9'):
    return ord(c) - ord('0')
  if (c < 'A'):
    return 0
  if (c <= 'F'):
    return ord(c) - ord('A') + 10
  return 0

def valid_msg(msg):
    cs = 0
    for c in msg[1:-3]:
        # if c == '*':
        #     break
        # print(c,end="")
        cs = cs ^ ord(c)

    cs = cs & 0xff
    cc = parseHex(msg[-2])*16 + parseHex(msg[-1])
    return cc == cs

def gsa_parser(msg):
    """
    $GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
    """
    chunks = msg[1:-3].split(',')
    match int(chunks[2]):
        case 1: fix = "unavailable"
        case 2: fix = "2D"
        case 3: fix = "3D"
        case _: fix = chunks[2]

    msg = GSA(
        chunks[0],
        chunks[1],
        fix,
        chunks[3:15],
        float(chunks[15]),
        float(chunks[16]),
        float(chunks[17])
    )
    return msg

def rmc_parser(msg):
    """
    $GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
    """
    chunks = msg[1:-3].split(',')
    id = chunks[0]
    utc = time_t(
        int(chunks[1][:2]),
        int(chunks[1][2:4]),
        float(chunks[1][4:])
    )
    lat = float(chunks[3][:2]) + float(chunks[3][2:]) / 60.0
    if chunks[4] == 'S':
        lat = -lat
    lon = float(chunks[5][:3]) + float(chunks[5][3:]) / 60.0
    if chunks[6] == 'W':
        lon = -lon

    msg = RMC(
        id,
        utc,
        chunks[2],
        lat,
        lon,
        float(chunks[7]),
        float(chunks[8]),
        date_t(
            int(chunks[9][:2]),
            int(chunks[9][2:4]),
            int(chunks[9][4:])
        ),
        "" if chunks[10] == "" else float(chunks[10])
    )
    return msg

def gga_parser(msg):
    """
    $GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
    GGA = namedtuple("GGA", "id utc lat lon qual num_sats hdop msl geoid age ref_id")
    """
    chunks = msg[1:-3].split(',')
    id = chunks[0]
    utc = time_t(
        int(chunks[1][:2]),
        int(chunks[1][2:4]),
        float(chunks[1][4:])
    )
    lat = float(chunks[2][:2]) + float(chunks[2][2:]) / 60.0
    if chunks[3] == 'S':
        lat = -lat
    lon = float(chunks[4][:3]) + float(chunks[4][3:]) / 60.0
    if chunks[5] == 'W':
        lon = -lon

    msg = GGA(
        id,
        utc,
        lat, lon,
        int(chunks[6]),
        int(chunks[7]),
        float(chunks[8]),
        float(chunks[9]),
        float(chunks[11]),
        "" if chunks[13] == "" else float(chunks[13]),
        "" if chunks[14] == "" else int(chunks[14]),
    )
    return msg


CR = '\r'
LF = '\n'

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
        while True: # $ ... \CR \LF
            buffer = []
            fin = [0,0 ]
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

            ok = valid_msg(msg)
            if not ok:
                print("*** ERROR ***")
                continue

            hdr = msg[3:6]
            gps = None
            if hdr == "GSA":
                gps = gsa_parser(msg)
            elif hdr == "RMC":
                gps = rmc_parser(msg)
            elif hdr == "GGA":
                gps = gga_parser(msg)
            elif hdr == "GSV":
                # gps = gga_parser(msg)
                pass
            elif hdr == "VTG":
                # gps = gga_parser(msg)
                pass
            else:
                print(hdr)

            if gps is not None:
                print(gps)
            #     match int(chunks[2]):
            #         case 1: fix = "unavailable"
            #         case 2: fix = "2D"
            #         case 3: fix = "3D"
            #         case _: fix = chunks[2]

            #     msg = GSA(
            #         chunks[0],
            #         chunks[1],
            #         fix,
            #         chunks[3:15],
            #         float(chunks[15]),
            #         float(chunks[16]),
            #         float(chunks[17].split("*")[0])
            #     )
            #     print(msg)



    except KeyboardInterrupt:
        print("\nctrl-c")
    except Exception as e:
        print(e)
    finally:
        ser.close()

main()