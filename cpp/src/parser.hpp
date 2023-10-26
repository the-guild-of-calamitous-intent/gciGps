#pragma once

#include <cstdio>
#include <cstdint> // int types
#include <cstdlib> // atoi, atof
#include <cstring> // memcpy, memset, strtok

#define CSTR_LEN 16
#define CVEC_LEN 25

// Fixed string array
struct cstr_t {
  cstr_t() {}
  ~cstr_t() {}

  bool set(const char* s, size_t sz) {
    if (sz >= CSTR_LEN) return false;
    memcpy(data, s, sz);
    data[sz] = '\0';
    // size = sz + 1;
    return true;
  }

  void clear() {
    memset(data,'\0',CSTR_LEN);
    // size = 0;
  }

  char data[CSTR_LEN]{'\0'};
  // size_t size=0;
};

// Fixed size vector of fixed string arrays
struct cvec_t {
  cvec_t () {}
  ~cvec_t () {}

  char* get_next() {
    if (index >= CVEC_LEN) return nullptr;
    return array[index++].data;
  }

  inline
  const size_t size() const { return index; }

  inline
  const char* operator[](const size_t i) const {
    if (i >= CVEC_LEN) return nullptr;
    return array[i].data;
  }

  void clear() {
    int i = CVEC_LEN;
    while (i >= 0) array[i--].clear();
    index = 0;
  }

  cstr_t array[CVEC_LEN];
  size_t index{0};
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

static
float to_dd(const char* p, char dir, int off) {
  char d[4];
  memcpy(d, p, off);
  d[off] = '\0';

  float val = atof(d) + atof(&p[off]) / 60.0f;

  if ((dir == 'S') || (dir == 'W')) val = -val;
  return val;
}

inline
uint8_t to_num(const char* s) {
  return uint8_t(s[0]-'0')*10 + uint8_t(s[1]-'0');
}

static
gps_time_t to_time(const char* s) {
  gps_time_t t;
  const char *p = s;
  t.hour = to_num(p);
  t.minute = to_num(&p[2]);
  t.seconds = atof(&p[4]);
  return t;
}

static
gps_date_t to_date(const char* s) {
  gps_date_t d;
  const char *p = s;
  d.day = to_num(p);
  d.month = to_num(&p[2]);
  d.year = to_num(&p[4]);
  return d;
}

cvec_t v;

bool split(char const* input) {
  char const* token_start = input;
  char const* token_end = input;
  size_t str_size = 0;

  while (*token_end != '\0') {
    if (*token_end == ',' || *token_end == '*') { // found deliminator
      if (token_start == token_end) { // empty token
        char* p = v.get_next();
        if (p == nullptr) return false;
        p[0] = '#';
        p[1] = '\0';
      }
      else { // token has len > 0
        char* p = v.get_next();
        if (p == nullptr) return false;
        strncpy(p, token_start, str_size-1);
      }
      // reset for next token
      token_start = token_end + 1;
      str_size = 0;
    }

    token_end++;
    str_size++;
  }

  // get final token, which is checksum
  // if (token_start != token_end) {
  //   char* p = v.get_next();
  //   if (p == nullptr) return false;
  //   strncpy(p, token_start, str_size-1);
  // }

  // printf(">> Msg size: %zu ", v.size());
  // for (int i=0; i<v.size(); ++i) printf("%i: %s ", i, v[i]);
  // printf("\n");

  return true;
}

//////////////////////////////////////////////////////////////////////////////

// $GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
// enum GGA {NO_ERROR, }
static
uint16_t gga_parser(const char* s, gga_t& msg) {
  // if (valid_gps_msg(s) == false) return false; // move earlier?
  // cvec_t v;
  v.clear();
  try { // FIXME: can I get rid of try/catch?
    if (!split(s)) return 1;
    // for (const cstr_t& c: v) printf("%s\n", c.data);

    if (v.size() != 15) return 2;

    if (v[1][0] != '#') msg.utc = to_time(v[1]);
    else return 3;
    if (v[2][0] != '#') msg.lat = to_dd(v[2], v[3][0],2);
    else return 4;
    if (v[4][0] != '#') msg.lon = to_dd(v[4], v[5][0],3);
    else return 5;

    if (v[6][0] != '#') msg.fix_qual = static_cast<uint8_t>(atoi(v[6]));
    else return 6;
    if (v[7][0] != '#') msg.num_sats = static_cast<uint8_t>(atoi(v[7]));
    else return 7;
    if (v[8][0] != '#') msg.hdop = atof(v[8]);
    else return 8;
    if (v[9][0] != '#') msg.msl = atof(v[9]); // altitude
    else return 9;
  } catch (...) { return 10; }
  return 0;
}

// ONLY care about the LAST 3 numbers: PDOP, HDOP, VDOP
// https://www.mathworks.com/help/nav/ref/marggpsfuser.fusegps.html
// GPS covariance matrix: [hdop^2 hdop^2 vdop^2]
// $GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
static
uint16_t gsa_parser(const char* s, gsa_t& msg) {
  // cvec_t v;
  v.clear();
  try {
    if (!split(s)) return 1;
    if (v.size() != 18) return 2;
    // for (int i=0; i<v.size(); ++i) printf("%i: %s ", i, v[i]);
    // printf("\n");

    // size_t end = v.index;
    // for (int i=0; i<v.index; ++i) printf("%i: %s\n", i, v[i]);
    // printf(">> %s %s %s\n", v[v.index-1], v[v.index-2], v[v.index-3]);

    // msg.vdop = atof(v[end-1]); // vertical dop
    // msg.hdop = atof(v[end-2]); // horizontal dop
    // msg.pdop = atof(v[end-3]); // spread of satellites

    if (v[15][0] != '#') msg.pdop = atof(v[15]); // vertical dop
    else return 3;
    if (v[16][0] != '#') msg.hdop = atof(v[16]); // horizontal dop
    else return 4;
    if (v[17][0] != '#') msg.vdop = atof(v[17]); // spread of satellites
    else return 5;
  } catch (...) { return 6; }

  return 0;
}

// // $GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
// static
// bool rmc_parser(const char* s, rmc_t& msg) {
//   if (valid_gps_msg(s) == false) return false; // move earlier?
//   // std::vector<cstr_t> v;
//   cvec_t<10> v;
//   try {
//     if (!split(s, ',', v)) return false;
//     if (v.size() < 10) return false;
//     // msg.id = to_id(v[0]);
//     msg.utc = to_time(v[1]);
//     // msg.status
//     msg.lat = to_dd(v[3].data,v[4].data[0]);
//     msg.lon = to_dd(v[5].data,v[6].data[0],3);
//     msg.date = to_date(v[9]);
//   } catch (...) { return false; }

//   return true;
// }

// value?
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
