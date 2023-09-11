
#pragma once

#if defined(ARDUINO)
typedef String cstring;
#else
#include <string>
typedef std::string cstring;
#endif

#include <string.h>
#include <vector>

std::vector<cstring> split(const cstring& str,   char delimiter) {
    std::vector<cstring> v;
    char *token = strtok(const_cast<char*>(str.c_str()), &delimiter);
    while (token != nullptr) {
        v.push_back(cstring(token));
        token = strtok(nullptr, &delimiter);
    }

  return v;
}

/*
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
*/

struct gsa_t {
  // enum fix_t: uint8_t {UNAVAILABLE=0, FIX_2D=2, FIX_3D=3};
  uint8_t id;
  uint8_t mode;
  uint8_t fix;
  int prns[12];
  float pdop;
  float hdop;
  float vdop;
};

bool gsa_parser(const cstring& s, gsa_t *msg) {
  if (msg == nullptr) return false;
  std::vector<cstring> v = split(s, ',');
  if (v.size() != 18) return false;

  msg->id = std::stoi(v[0]);
  msg->mode = std::stoi(v[1]);
  msg->fix = std::stoi(v[2]);
  for (int i=3; i < 15; ++i) msg->prns[i-3] = std::stoi(v[i]);
  msg->pdop = std::stof(v[15]);
  msg->hdop = std::stof(v[16]);
  msg->vdop = std::stof(v[17]);

  return true;
}

/*
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
*/

struct gga_t {
  uint8_t id;
  time_t utc;
  float lat;
  float lon;
  int qual;
  int num_sats;
  float hdop;
  float msl;
  float geoid;
  float age;  // empty
  int ref_id; // empty
};

bool gga_parser(const cstring& s, gga_t *msg) {
  if (msg == nullptr) return false;
  std::vector<cstring> v = split(s, ',');

  char d[3];
  d[0] = ' ';
  memcpy(&d[1], v[2].data(), 2);
  msg->lat = atof(d);
  v[4][0] = ' ';
  v[4][1] = ' ';
  msg->lat += stof(v[2]) / 60.0f;

  if (v[3] == "S") msg->lat *= -1.0f;

  memcpy(&d[1], v[4].data(), 2);
  msg->lon = atof(d);
  v[4][0] = ' ';
  v[4][1] = ' ';
  msg->lon += stof(v[4]) / 60.0f;

  if (v[5] == "W") msg->lon *= -1.0f;



  return true;
}