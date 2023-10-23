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

#if defined(ARDUINO)
  typedef String cstring;
#elif defined(PI_PICO) || defined(PI_PICO_W)
  #warning "Pi pico is using std::string in gcigps ... FIXME"
  #include <string>
  typedef std::string cstring;
#else
  #warning "Using std::string in gcigps ... FIXME"
  #include <string>
  typedef std::string cstring;
#endif

#include <stdio.h> // FIXME: remove

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

class GPS {
  public:
  GPS(): state(GpsState::NONE) {}
  ~GPS() {}

  bool valid_msg(const cstring& msg) {
    return true;
  }

  bool read(const char c) {
    constexpr char CR = '\r';
    constexpr char LF = '\n';

    if (state == GpsState::NONE && c == '$') { // START
      state = GpsState::START;
      buffer.clear(); // reset
      buffer.push_back(c);
      // cout << "START: " << buffer.size() << endl;
      // Serial.println("START");
      return false;
    }
    if (state == GpsState::START && (c >= '*' && c <= 'Z')) {
      buffer.push_back(c);
      return false;
    }
    if (state == GpsState::START && c == CR) {
      state = GpsState::END0;
      // Serial.println("END0");
      return false;
    }
    if (state == GpsState::END0 && c == LF) { // DONE
      state = GpsState::NONE;
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
    buffer.clear();
    return false;
  }

  // char cbuf[128];

  cstring get_msg_str() {
    buffer.push_back('\0');
    cstring ret(buffer.data());
    // cstring ret(buffer.data(), buffer.size());

    // memset(cbuf,0,128);
    // memcpy(cbuf, buffer.data(), buffer.size());
    // cbuf[buffer.size()] = '\0';

    // cstring ret(cbuf);
    return ret;
  }

  GpsID get_id() {
    if (buffer.size() < 6) return GpsID::NONE;
    // char hdr[4] = {buffer[3],buffer[4],buffer[5],'\0'};
    // cstring msg(&buffer.data()[3], 3);
    // cstring msg(hdr);
    cstring msg;
    msg += buffer[3];
    msg += buffer[4];
    msg += buffer[5];

    // cout << "id: " << msg << endl;
    if (msg == "GGA") return GpsID::GGA;
    if (msg == "RMC") return GpsID::RMC;
    if (msg == "GSA") return GpsID::GSA;
    if (msg == "GLL") return GpsID::GLL;
    return GpsID::NONE;
  }

  bool get_msg(gga_t& msg) {
    cstring str = get_msg_str();
    return gga_parser(str, msg);
  }

  bool get_msg(rmc_t& msg) {
    cstring str = get_msg_str();
    return rmc_parser(str, msg);
  }

  bool get_msg(gsa_t& msg) {
    cstring str = get_msg_str();
    return gsa_parser(str, msg);
  }

  bool get_msg(gll_t& msg) {
    cstring str = get_msg_str();
    return gll_parser(str, msg);
  }

protected:
  GpsState state;
  std::vector<char> buffer;
  // char end{0};
};


}

// namespace gecko {



// }