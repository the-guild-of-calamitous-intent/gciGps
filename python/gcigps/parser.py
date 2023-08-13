
from .messages import *

def parseHex(hi, low):
    def char2hex(c):
        if (c < '0'):
            return 0
        if (c <= '9'):
            return ord(c) - ord('0')
        if (c < 'A'):
            return 0
        if (c <= 'F'):
            return ord(c) - ord('A') + 10
        return 0
    ans = char2hex(hi)*16 + char2hex(lo)
    return ans

class Parser:
    def parse_msg(self, msg):
        ok = valid_msg(msg)
        if not ok:
            print("*** ERROR: invalide gps message ***")
            return None

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
            print("Unknown message:", msg)

        return gps

    def valid_msg(self, msg):
        if msg[0] != '$' or msg[-3] != '*':
            return False

        cs = 0
        for c in msg[1:-3]:
            cs = cs ^ ord(c)

        cs = cs & 0xff
        # cc = parseHex(msg[-2])*16 + parseHex(msg[-1])
        cc = parseHex(msg[-2], msg[-1])
        return cc == cs

    def gsa_parser(self, msg):
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

    def rmc_parser(self, msg):
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

    def gga_parser(self, msg):
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
