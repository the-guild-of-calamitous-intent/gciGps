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

// CR \r
// LF \n
// <CR><LF> \r\n
// $[....]<CR><LF>

/////////////////////////////////////////////////////////////////////////////
// PMTK001 ACK
// PMTK01,CMD,FLAG*CHECKSUM
// FLAG: 0-invalid, 1-unsupported, 2-valid,failed, 3-valid,success
// $PMTK001,604,3*32<CR><LF>
//
// $PMTK001,314,3*36 -> (314) set nema output, GCI_RMCGGA
// $PMTK001,220,3*30 -> (220) set pos fix, GCI_UPDATE_1HZ
/////////////////////////////////////////////////////////////////////////////

// // PMTK commands
// // https://www.sparkfun.com/datasheets/GPS/Modules/PMTK_Protocol.pdf
// constexpr uint8_t GCI_UPDATE_1HZ[] = "$PMTK220,1000*1F\r\n"; //  1 Hz
// constexpr uint8_t GCI_UPDATE_2HZ[] = "$PMTK220,500*2B\r\n";  //  2 Hz
// // constexpr uint8_t GCI_UPDATE_5HZ[] = "$PMTK220,200*2C\r\n";  //  5 Hz ... invalid?
// // constexpr uint8_t GCI_UPDATE_10HZ[] = "$PMTK220,100*2F\r\n"; // 10 Hz ... invalid? must > 200, only 100

// // Position fix update rates
// // Can't fix position faster than 5 times a second!
// constexpr uint8_t GCI_FIX_CTL_1HZ[] = "$PMTK300,1000,0,0,0,0*1C\r\n"; // 1 Hz output
// constexpr uint8_t GCI_FIX_CTL_5HZ[] = "$PMTK300,200,0,0,0,0*2F\r\n";  // 5 Hz output

// constexpr uint8_t GCI_BAUD_115200[] = "$PMTK251,115200*1F\r\n"; // 115200 bps
// constexpr uint8_t GCI_BAUD_57600[] = "$PMTK251,57600*2C\r\n";   //  57600 bps
// constexpr uint8_t GCI_BAUD_9600[] = "$PMTK251,9600*17\r\n";     //   9600 bps

// constexpr uint8_t GCI_ANTENNA[] = "$PGCMD,33,1*6C\r\n"; // request for updates on antenna status
// constexpr uint8_t GCI_NOANTENNA[] = "$PGCMD,33,0*6D\r\n"; // don't show antenna status messages

// constexpr uint8_t GCI_ENABLE_SBAS[] = "$PMTK313,1*2E\r\n"; // Enable search for SBAS satellite (only works with 1Hz < output rate)
// constexpr uint8_t  GCI_ENABLE_WAAS[] = "$PMTK301,2*2E\r\n"; // WAAS for DGPS correction data

// constexpr uint8_t GCI_GLL[] = "$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; // only the GLL sentence
// constexpr uint8_t GCI_RMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; // only the RMC sentence
// // constexpr uint8_t GCI_VTG[] = "$PMTK314,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" // only the VTG
// constexpr uint8_t GCI_GGA[] = "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; // just the GGA
// constexpr uint8_t GCI_GSA[] = "$PMTK314,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; // just the GSA
// // constexpr uint8_t GCI_GSV[] = "$PMTK314,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0*29" // just the GSV
// constexpr uint8_t GCI_RMCGGA[] = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"; // RMC and GGA
// constexpr uint8_t GCI_RMCGGAGSA[] = "$PMTK314,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; // RMC, GGA and GSA
