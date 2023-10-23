/////////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2023 Kevin J. Walchko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <cstdlib>
#include <cstring>


struct cstr {
  cstr(char* s, size_t sz) {
    memcpy(data, s, sz);
    data[sz] = '\0';
    size = sz + 1;
  }

  char data[16]{'\0'};
  size_t size=0;
};


// Helper functions ///////////////////////////////////////////////////////////

static
uint8_t char2hex(const char c) {
  if (c < '0') return 0;
  if (c <= '9') return uint8_t(c - '0');
  if (c < 'A') return 0;
  if (c <= 'F') return uint8_t(c - 'A') + 10;
  return 0;
}

inline
uint8_t checksum(const char hi, const char lo) {
  return char2hex(hi)*16 + char2hex(lo);
}

inline
int to_int(const cstr& s) {
  return atoi(s.data);
}

static
float to_dd(const char* a, char dir, int off=2) {
  float val;
  char d[3];
  d[0] = ' ';

  memcpy(&d[3-off], a, off);

  val = atof(d);
  // printf("val: %f  ", val);
  const char* p = a;
  val += atof(&p[off]) / 60.0f;
  // printf("val: %f\n", val);

  if ((dir == 'S') || (dir == 'W')) val *= -1.0f;
  return val;
}

inline
uint8_t to_num(const char* s) {
  return uint8_t(s[0]-'0')*10 + uint8_t(s[1]-'0');
}

static
gps_time_t to_time(const cstr& s) {
  gps_time_t t;
  const char *p = s.data;
  t.hour = to_num(p);
  t.minute = to_num(&p[2]);
  // char* str(&p[4]);
  t.seconds = atof(&p[4]);
  return t;
}

static
gps_date_t to_date(const cstr& s) {
  gps_date_t d;
  const char *p = s.data;
  d.day = to_num(p);
  d.month = to_num(&p[2]);
  d.year = to_num(&p[4]);
  return d;
}

static
bool split(const char* str, char delimiter, std::vector<cstr>& v) {
  char * pch;
  pch = strtok ((char*)str,",*");
  while (pch != NULL) {
    // printf ("%s  %zu\n",pch,strlen(pch));
    cstr tok(pch, strlen(pch));
    v.push_back(tok);
    pch = strtok (NULL, ",*");
  }

  return true;
}


//////////////////////////////////////////////////////////////////////////////

// $GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
static
bool gga_parser(const char* s, gga_t& msg) {
  std::vector<cstr> v;
  try {
    bool ok = split(s, ',', v);
    if (!ok) return false;
    // printf(">> good split\n");
    // int i=1;
    // for (const cstr& c: v) printf("%d: %s\n", i++, c.data);

    if (v.size() < 10) return false;
    // printf(">> good size\n");

    // msg.id = to_id(v[0]);
    msg.utc = to_time(v[1]);
    msg.lat = to_dd(v[2].data, v[3].data[0]);
    msg.lon = to_dd(v[4].data, v[5].data[0],3);
    msg.qual = to_int(v[6]);
    msg.num_sats = to_int(v[7]);
    msg.hdop = atof(v[8].data);
    msg.msl = atof(v[9].data);
    // msg.geoid = atof(v[11].data);
  } catch (...) { return false; }
  return true;
}

// $GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
static
bool rmc_parser(const char* s, rmc_t& msg) {
  std::vector<cstr> v;
  try {
    bool ok = split(s, ',', v);
    if (!ok) return false;

    if (v.size() < 10) return false;
    // msg.id = to_id(v[0]);
    msg.utc = to_time(v[1]);
    // msg.status
    msg.lat = to_dd(v[3].data,v[4].data[0]);
    msg.lon = to_dd(v[5].data,v[6].data[0],3);
    msg.date = to_date(v[9]);
  } catch (...) { return false; }

  return true;
}

// $GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A
// static
// bool gll_parser(const char* s, gll_t& msg) {
//   std::vector<char*> v;
//   bool ok = split(s, ',', v);
//   if (!ok) return false;

//   msg.lat = to_dd(v[1], v[2][0]);
//   msg.lon = to_dd(v[3], v[4][0],3);
//   msg.utc = to_time(v[5]);

//   return true;
// }

// $GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
// static
// bool gsa_parser(const char* s, gsa_t& msg) {
//   std::vector<char*> v;
//   bool ok = split(s, ',',v);
//   if (v.size() != 18) return false;


//   // cout << v.length() << endl;
//   // int i = 0;
//   // for (const std::string& s: v) cout << i++ << ": " << s << endl;

//   // msg.id = to_id(v[0]);
//   // msg.mode = std::stoi(v[1]);

//   msg.fix = to_int(v[2]);

//   uint8_t count = 0;
//   // for (int i=3; i < 15; ++i) {
//   //   // if (v[i].length() > 0) msg.prns[count++] = std::stoi(v[i]);
//   //   if (v[i].length() > 0) msg.prns[count++] = to_int(v[i]);
//   //   else msg.prns[i-3] = 0;
//   // }
//   msg.num_prn = count;
//   // msg.pdop = (v[15].length() > 0) ? to_float(v[15]) : 0.0f;
//   // msg.hdop = (v[16].length() > 0) ? to_float(v[16]) : 0.0f;
//   // msg.vdop = (v[17].length() > 0) ? to_float(v[17]) : 0.0f;

//   // cout << v[17] << " " << msg.vdop << endl;

//   return true;
// }