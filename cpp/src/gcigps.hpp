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

#include <cstring>
#include "mtk.hpp"
#include "gps_msgs.hpp"
#include "parser.hpp"

namespace gci {

enum class GpsState: uint8_t {
  NONE,
  START,
  END0,
  END1
};

// enum class GpsID: uint8_t {
//   NONE,
//   GGA,
//   GSA,
//   // RMC,
//   // GLL
// };

constexpr uint8_t GPS_NONE = 0;
constexpr uint8_t GPS_GGA = 1;
constexpr uint8_t GPS_GSA = 2;
constexpr uint8_t GPS_RMC = 4;
constexpr uint8_t GPS_GLL = 8;

// change to MAX_NEMA_LENGTH
constexpr size_t C_BUFFER_SIZE = 128; // max NEMA length

static
bool valid_gps_msg(const char* msg) {
  if (msg == nullptr) return false;
  if (msg[0] != '$') return false;
  size_t sz = strlen(msg);
  uint8_t chksum = 0;
  size_t i = 1;
  while (i < sz) {
    if (msg[i] == '*') break;
    chksum ^= msg[i++];
  }
  uint8_t msg_cs = checksum(msg[sz-2], msg[sz-1]);
  return chksum == msg_cs;
}

class NEMA {
  public:
  NEMA() {}
  ~NEMA() {}

  bool valid_msg(const char* msg, size_t len) {
    return true;
  }

  uint8_t parse(const char c) {
    constexpr char CR = '\r';
    constexpr char LF = '\n';

    if (state == GpsState::NONE && c == '$') { // START
      state = GpsState::START;

      memset(cbuff, '\0', C_BUFFER_SIZE);
      ptr = 0;
      cbuff[ptr++] = c;
      return GPS_NONE;
    }
    if (state == GpsState::START && (c >= '*' && c <= 'Z')) {
      cbuff[ptr++] = c;
      return GPS_NONE;
    }
    if (state == GpsState::START && c == CR) {
      state = GpsState::END0;
      return GPS_NONE;
    }
    if (state == GpsState::END0 && c == LF) { // DONE
      state = GpsState::NONE;
      cbuff[ptr++] = '\0';
      if (valid_gps_msg(cbuff) == false) {
        memset(cbuff, '\0', C_BUFFER_SIZE);
        ptr = 0;
        return GPS_NONE;
      }
      return get_id();
    }
    state = GpsState::NONE;
    return GPS_NONE;
  }

  inline
  bool get_msg(gga_t& msg) {
    return gga_parser(cbuff, msg);
  }

  inline
  bool get_msg(gsa_t& msg) {
    return gsa_parser(cbuff, msg);
  }

  // bool get_msg(rmc_t& msg) {
  //   // memset(cbuff, '\0', C_BUFFER_SIZE);
  //   // memcpy(cbuff, buffer.data(), buffer.size());
  //   return rmc_parser(cbuff, msg);
  // }

  // bool get_msg(gll_t& msg) {
  //   // cstring str = get_msg_str();
  //   // memset(cbuff, '\0', C_BUFFER_SIZE);
  //   // memcpy(cbuff, buffer.data(), buffer.size());
  //   return gll_parser(cbuff, msg);
  // }

protected:
  GpsState state{GpsState::NONE};
  char cbuff[C_BUFFER_SIZE]{0};
  uint32_t ptr{0};

  uint8_t get_id() {
    // if (valid_gps_msg(cbuff) == false) return GpsID::NONE;
    if (strncmp(cbuff+3, "GGA", 3) == 0) return GPS_GGA;
    if (strncmp(cbuff+3, "GSA", 3) == 0) return GPS_GSA;
    // if (strncmp(cbuff+3, "RMC", 3) == 0) return GPS_RMC;
    // if (strncmp(cbuff+3, "GLL", 3) == 0) return GPS_GLL;
    return GPS_NONE;
  }
};

} // end namespace gci