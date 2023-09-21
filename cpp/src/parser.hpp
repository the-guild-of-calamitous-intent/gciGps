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

#include <string.h>
#include <vector>
#include <stdlib.h>


// #include <iostream>
// using std::cout;
// using std::endl;


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
float to_float(const cstring& s) {
#if defined(ARDUINO)
  return s.toFloat();
#else
  return std::stof(s);
#endif
}

inline
int to_int(const cstring& s) {
#if defined(ARDUINO)
  return s.toInt();
#else
  return std::stoi(s);
#endif
}


static
bool split(const cstring& str, char delimiter, std::vector<cstring>& v) {
  size_t i = 0;
  cstring ss;
  size_t size = str.length();
  while (i < size) {
    char c = str[i++];
    if (c == ',') {
      v.push_back(ss);
      ss = ""; // ss.clear();
    }
    else ss += c;
  }
  // remove last 3 chars, checksum
#if defined(ARDUINO)
  cstring fin = ss.substring(0,ss.length()-3);
#else
  // ss.pop_back(); // b
  // ss.pop_back(); // b
  // ss.pop_back(); // *
  cstring fin = ss.substr(0,ss.length()-3);
#endif
  v.push_back(ss);

  // cout << v.length() << endl;
  // for (const std::string& s: v) cout << s << endl;

  return true;
}

static
float to_dd(const cstring& a, char dir, int off=2) {
  float val;
  char d[3];
  d[0] = ' ';

  memcpy(&d[3-off], a.c_str(), off);

  val = atof(d);
  const char* p = a.c_str(); //a.data();
  val += atof(&p[off]) / 60.0f;

  if ((dir == 'S') || (dir == 'W')) val *= -1.0f;
  return val;
}

inline
uint8_t to_num(const char* s) {
  return uint8_t(s[0]-'0')*10 + uint8_t(s[1]-'0');
}

static
gps_time_t to_time(const cstring& s) {
  gps_time_t t;
  const char *p = s.c_str(); //s.data();
  t.hour = to_num(p);
  t.minute = to_num(&p[2]);
  cstring str(&p[4]);
  t.seconds = to_float(str);
  return t;
}

static
gps_date_t to_date(const cstring& s) {
  gps_date_t d;
  const char *p = s.c_str(); //s.data();
  d.day = to_num(p);
  d.month = to_num(&p[2]);
  d.year = to_num(&p[4]);
  return d;
}

//////////////////////////////////////////////////////////////////////////////

// $GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
static
bool gsa_parser(const cstring& s, gsa_t& msg) {
  std::vector<cstring> v;
  bool ok = split(s, ',',v);
  if (v.size() != 18) return false;


  // cout << v.length() << endl;
  // int i = 0;
  // for (const std::string& s: v) cout << i++ << ": " << s << endl;

  // msg.id = to_id(v[0]);
  // msg.mode = std::stoi(v[1]);

  msg.fix = to_int(v[2]);

  uint8_t count = 0;
  for (int i=3; i < 15; ++i) {
    // if (v[i].length() > 0) msg.prns[count++] = std::stoi(v[i]);
    if (v[i].length() > 0) msg.prns[count++] = to_int(v[i]);
    else msg.prns[i-3] = 0;
  }
  msg.num_prn = count;
  msg.pdop = (v[15].length() > 0) ? to_float(v[15]) : 0.0f;
  msg.hdop = (v[16].length() > 0) ? to_float(v[16]) : 0.0f;
  msg.vdop = (v[17].length() > 0) ? to_float(v[17]) : 0.0f;

  // cout << v[17] << " " << msg.vdop << endl;

  return true;
}

// $GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
static
bool gga_parser(const cstring& s, gga_t& msg) {
  std::vector<cstring> v;
  bool ok = split(s, ',', v);
  if (!ok) return false;
  if (v.size() != 15) return false;
  // msg.id = to_id(v[0]);
  msg.utc = to_time(v[1]);
  msg.lat = to_dd(v[2], v[3][0]);
  msg.lon = to_dd(v[4], v[5][0],3);
  msg.qual = to_int(v[6]);
  msg.num_sats = to_int(v[7]);
  msg.hdop = to_float(v[8]);
  msg.msl = to_float(v[9]);
  msg.geoid = to_float(v[11]);
  return true;
}

// $GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
static
bool rmc_parser(const cstring& s, rmc_t& msg) {
  std::vector<cstring> v;
  bool ok = split(s, ',', v);
  if (!ok) return false;

  // cout << v.size() << endl;
  // for (const std::string& s: v) cout << s << endl;

  if (v.size() < 13) return false;
  // msg.id = to_id(v[0]);
  msg.utc = to_time(v[1]);
  // msg.status
  msg.lat = to_dd(v[3],v[4][0]);
  msg.lon = to_dd(v[5],v[6][0],3);
  msg.date = to_date(v[9]);

  return true;
}

// $GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A
static
bool gll_parser(const cstring& s, gll_t& msg) {
  std::vector<cstring> v;
  bool ok = split(s, ',', v);
  if (!ok) return false;

  msg.lat = to_dd(v[1], v[2][0]);
  msg.lon = to_dd(v[3], v[4][0],3);
  msg.utc = to_time(v[5]);

  return true;
}