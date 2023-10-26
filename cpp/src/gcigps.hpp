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

enum class GpsID: uint8_t {
  NONE,
  GGA,
  GSA,
  // RMC,
  // GLL
};

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

class GPS {
  public:
  GPS(): state(GpsState::NONE) {}
  ~GPS() {}

  bool valid_msg(const char* msg, size_t len) {
    return true;
  }

  bool read(const char c) {
    constexpr char CR = '\r';
    constexpr char LF = '\n';

    if (state == GpsState::NONE && c == '$') { // START
      state = GpsState::START;

      memset(cbuff, '\0', C_BUFFER_SIZE);
      ptr = 0;
      cbuff[ptr++] = c;
      return false;
    }
    if (state == GpsState::START && (c >= '*' && c <= 'Z')) {
      cbuff[ptr++] = c;
      return false;
    }
    if (state == GpsState::START && c == CR) {
      state = GpsState::END0;
      return false;
    }
    if (state == GpsState::END0 && c == LF) { // DONE
      state = GpsState::NONE;
      cbuff[ptr++] = '\0';
      if (valid_gps_msg(cbuff) == false) {
        memset(cbuff, '\0', C_BUFFER_SIZE);
        ptr = 0;
        return false;
      }
      return true;
    }
    state = GpsState::NONE;
    return false;
  }

  GpsID get_id() {
    // if (valid_gps_msg(cbuff) == false) return GpsID::NONE;
    if (strncmp(cbuff+3, "GGA", 3) == 0) return GpsID::GGA;
    if (strncmp(cbuff+3, "GSA", 3) == 0) return GpsID::GSA;
    // if (strncmp(cbuff+3, "RMC", 3) == 0) return GpsID::RMC;
    // if (strncmp(cbuff+3, "GLL", 3) == 0) return GpsID::GLL;
    return GpsID::NONE;
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
  GpsState state;
  char cbuff[C_BUFFER_SIZE];
  uint32_t ptr{0};
};

} // end namespace gci