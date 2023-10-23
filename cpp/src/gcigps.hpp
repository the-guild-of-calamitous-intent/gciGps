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

// #include <cstdio> // FIXME: remove
#include <vector>
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
  RMC,
  GLL
};

constexpr size_t C_BUFFER_SIZE = 128; // max NEMA length

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

      // buffer.clear(); // reset
      // buffer.push_back(c);
      return false;
    }
    if (state == GpsState::START && (c >= '*' && c <= 'Z')) {
      // buffer.push_back(c);
      cbuff[ptr++] = c;
      return false;
    }
    if (state == GpsState::START && c == CR) {
      state = GpsState::END0;
      // Serial.println("END0");
      return false;
    }
    if (state == GpsState::END0 && c == LF) { // DONE
      state = GpsState::NONE;
      // buffer.push_back('\0');
      cbuff[ptr++] = '\0';
      // Serial.println("END1");
      // cout << "DONE: " << buffer.size() << "  " << get_msg_str() << endl;
      return true;
    }
    // if (state == GpsState::END0 && c != LF) { // error
    //   buffer.clear();
    //   Serial.println("CRAP");
    //   // end = 0;
    //   state = GpsState::NONE;
    //   return false;
    // }
    // cout << "bad: " << c << endl;
    state = GpsState::NONE;
    // buffer.clear();
    return false;
  }

  GpsID get_id() {
    // if (buffer.size() < 6) return GpsID::NONE;

    // memset(cbuff, '\0', C_BUFFER_SIZE);
    // memcpy(cbuff, buffer.data(), buffer.size());

    if (strncmp(cbuff+3, "GGA", 3) == 0) return GpsID::GGA;
    if (strncmp(cbuff+3, "RMC", 3) == 0) return GpsID::RMC;
    // if (strncmp(cbuff+3, "GSA", 3) == 0) return GpsID::GSA;
    // if (strncmp(cbuff+3, "GLL", 3) == 0) return GpsID::GLL;
    return GpsID::NONE;
  }

  bool get_msg(gga_t& msg) {
    // memset(cbuff, '\0', C_BUFFER_SIZE);
    // memcpy(cbuff, buffer.data(), buffer.size());
    return gga_parser(cbuff, msg);
  }

  bool get_msg(rmc_t& msg) {
    // memset(cbuff, '\0', C_BUFFER_SIZE);
    // memcpy(cbuff, buffer.data(), buffer.size());
    return rmc_parser(cbuff, msg);
  }

  // bool get_msg(gsa_t& msg) {
  //   // cstring str = get_msg_str();
  //   memset(cbuff, '\0', C_BUFFER_SIZE);
  //   memcpy(cbuff, buffer.data(), buffer.size());
  //   return gsa_parser(cbuff, msg);
  // }

  // bool get_msg(gll_t& msg) {
  //   // cstring str = get_msg_str();
  //   memset(cbuff, '\0', C_BUFFER_SIZE);
  //   memcpy(cbuff, buffer.data(), buffer.size());
  //   return gll_parser(cbuff, msg);
  // }

protected:
  GpsState state;
  // std::vector<char> buffer;
  char cbuff[C_BUFFER_SIZE];
  uint32_t ptr{0};
  // char end{0};
};


}

// namespace gecko {



// }