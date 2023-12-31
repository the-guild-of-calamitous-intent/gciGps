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

#include <cstdint>

struct __attribute__((packed)) gps_date_t {
  uint8_t year, month, day;
  bool operator==(const gps_date_t& c) const {
    return (year == c.year) && (month == c.month) && (day == c.day);
  }
}; // 3*1 = 3

struct __attribute__((packed)) gps_time_t {
  uint8_t hour, minute;
  float seconds;
  bool operator==(const gps_time_t& c) const {
    return (hour == c.hour) && (minute == c.minute) && (int(seconds) == int(c.seconds));
  }
}; // 1+1+4 = 6

// almost everyting I want ... except date, but really don't need it
struct __attribute__((packed)) gga_t {
  enum FixQuality: uint8_t {INVALID,GNSS,DGPS,PPS,RTKINEMATIC,ESTIMATED,MANUAL,SIMULATION};
  // uint8_t id;
  gps_time_t utc;   // XX:XX:XX.XXXX
  float lat;        // XXX.XXX
  float lon;        //  XX.XXX
  uint8_t fix_qual; // 0-8
  uint8_t num_sats; // 0-32
  float hdop;       // horizontal (altitude) dilution of percision
  float msl;        // mean sealevel or altitude
  // float geoid;
  // float age;  // empty
  // int ref_id; // empty
}; // 6+4+4+1+1+4+4 = 24

// ONLY care about the LAST 3 numbers
struct __attribute__((packed)) gsa_t {
  // enum fix_t: uint8_t {UNAVAILABLE=0, FIX_2D=2, FIX_3D=3};
  // uint8_t id;
  // uint8_t mode;
  // uint8_t fix;
  // uint8_t prns[12]; // make vector?
  // uint8_t num_prn;
  float pdop;
  float hdop;
  float vdop;
};

// // value over GGA: gps_date_t field
// // $GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
// struct __attribute__((packed)) rmc_t {
//   // uint8_t id;
//   gps_time_t utc;
//   // status;
//   float lat;
//   float lon;
//   // speed;
//   // track;
//   gps_date_t date;
//   // float mag;
// }; // 6+4+4+3 = 17

// value?
// // $GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A
// struct __attribute__((packed)) gll_t {
//   float lat;
//   float lon;
//   gps_time_t utc;
// };