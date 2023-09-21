"""
MIT License

Copyright (c) 2023 Kevin J. Walchko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""
from .messages import *

def parseHex(hi, lo):
    """
    GPS message checksum is a hex number converted to characters and
    this converts them back go a number:
      parseHex('A','4') --> 0xA4
    """
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
    """
    Parser can turn limited NEMA GPS message strings into message
    structures. By default, only GGA and RMC messages are parsed.
    This can be changed by:
      p = Parser()
      p.gsa = True
      msg = p.parse_msg("$GPGSA,...,*A4")
    """

    gga = True
    gsa = False
    gsv = False
    rmc = True
    vtg = False

    def parse_msg(self, msg):
        ok = self.valid_msg(msg)
        if not ok:
            print("*** ERROR: invalide gps message ***")
            print(msg)
            return None

        hdr = msg[3:6]
        gps = None
        if hdr == "GSA":
            if not self.gsa: return None
            gps = self.gsa_parser(msg)
        elif hdr == "RMC":
            if not self.rmc: return None
            gps = self.rmc_parser(msg)
        elif hdr == "GGA":
            if not self.gga: return None
            gps = self.gga_parser(msg)
        elif hdr == "GSV":
            if not self.gsv: return None
            # gps = gsv_parser(msg)
            pass
        elif hdr == "VTG":
            if not self.vtg: return None
            # gps = vtg_parser(msg)
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

        msg = gsa_t(
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

        msg = rmc_t(
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

        msg = gga_t(
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
