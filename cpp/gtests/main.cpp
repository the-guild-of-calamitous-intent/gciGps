#include <gtest/gtest.h>
#include <string>
#include "gcigps.hpp"
#include <iostream>
#include <vector>

using namespace std;

TEST(gps, GGA1) {
  gps_time_t utc{23,48,5.000};
  char s[] = "$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\0";

  bool ans = gci::valid_gps_msg(s);
  EXPECT_TRUE(ans);

  gga_t gga;
  uint16_t err = gga_parser(s, gga);
  EXPECT_TRUE(err == 0) << err;
  EXPECT_EQ(utc, gga.utc);
  EXPECT_FLOAT_EQ(gga.lat, atof("39") + atof("06.7106")/60.0f);
  EXPECT_FLOAT_EQ(gga.lon, -1. * (atof("121") + atof("20.3144")/60.0f));
}

TEST(gps, GGA2) {
  char s[] = "$GPGGA,234805.000,,,,,,,,,,,,,*50\0";

  bool ans = gci::valid_gps_msg(s);
  EXPECT_FALSE(ans);

  gga_t gga;
  uint16_t err = gga_parser(s, gga);
  EXPECT_TRUE(err > 0) << err;
}

TEST(gps, GSA1) {
  char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\0";

  gsa_t gsa;
  uint16_t err = gsa_parser(s, gsa);
  EXPECT_TRUE(err == 0) << err;
  // EXPECT_EQ(gsa.fix, 3);
  // EXPECT_EQ(gsa.prns[0], 3);
  // EXPECT_EQ(gsa.prns[1], 32);
  // EXPECT_EQ(gsa.prns[2], 2);
  // EXPECT_EQ(gsa.prns[3], 4);
  // EXPECT_EQ(gsa.num_prn, 4);
  EXPECT_FLOAT_EQ(gsa.pdop, 2.01);
  EXPECT_FLOAT_EQ(gsa.hdop, 1.77);
  EXPECT_FLOAT_EQ(gsa.vdop, 0.95);
}

TEST(gps, GSA2) {
  char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,,,*08\0";

  gsa_t gsa;
  uint16_t err = gsa_parser(s, gsa);
  EXPECT_TRUE(err > 0) << err;
}

TEST(gps, GPS1) {
  gps_time_t utc{23,48,5.678};
  vector<string> v{
    "\n\n$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\r\na\t\r",
    "49fmdd$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\r\n",
    // "$GPGGA,151924,3723.454444,N,12202.269777,W,2,09,1.9,–17.49,M,–25.67,M,1,0000*57\r\n",
    // "$GPGSA,A,3,19,28,14,18,27,22,31,29,,,,,1.7,1.0,1.3*35\r\n"
  };

  for (const string& s: v) {
    gci::GPS gps;
    bool ok = false;
    for (const char& c: s) {
      // don't want to read junk at end of string
      if (!ok) ok = gps.read(c);
    }

    EXPECT_TRUE(ok); // good message

    gci::GpsID id = gps.get_id();
    if (id == gci::GpsID::GGA) {
      gps_time_t utc{23,48,5.000};
      gga_t gga;
      // cout << "GGA" << endl;

      uint16_t err = gps.get_msg(gga);
      // cout << gga.lat << " " << gga.lon << endl;

      EXPECT_TRUE(err == 0) << err;
      EXPECT_EQ(utc, gga.utc);
      EXPECT_FLOAT_EQ(gga.lat, 39. + 6.7106/60.);
      EXPECT_FLOAT_EQ(gga.lon, -121. - 20.3144/60.);
    }
    else if (id == gci::GpsID::GSA) {
      gsa_t gsa;
      // cout << "GSA" << endl;

      uint16_t err = gps.get_msg(gsa);

      EXPECT_TRUE(err == 0) << err;
      // EXPECT_EQ(gsa.fix, 3);
      // EXPECT_EQ(gsa.prns[0], 3);
      // EXPECT_EQ(gsa.prns[1], 32);
      // EXPECT_EQ(gsa.prns[2], 2);
      // EXPECT_EQ(gsa.prns[3], 4);
      // EXPECT_EQ(gsa.num_prn, 4);
      EXPECT_FLOAT_EQ(gsa.pdop, 2.01);
      EXPECT_FLOAT_EQ(gsa.hdop, 1.77);
      EXPECT_FLOAT_EQ(gsa.vdop, 0.95);
    }
    else if (id == gci::GpsID::NONE) {}
    else EXPECT_TRUE(false);
  }

}

TEST(gps, GPS2) {
  gps_time_t utc{23,48,5.678};
  vector<string> v{
    "\n\n$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\r\na\t\r",
    "49fmdd$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\r\n45d7ftugy",
    "$GPGSA,A,3,19,28,14,18,27,22,31,29,,,,,1.7,1.0,1.3*35\r\nhugy",
    "hyui$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F\r\n",
    "$GNGSA,A,3,21,5,29,25,12,10,26,2,,,,,1.2,0.7,1.0*27\r\n",
    "$GNGSA,A,3,65,67,80,81,82,88,66,,,,,,1.2,0.7,1.0*20\r\n"
  };

  for (const string& s: v) {
    gci::GPS gps;
    bool ok = false;
    for (const char& c: s) {
      ok = gps.read(c);
      if (ok) break;
    }
    EXPECT_TRUE(ok) << s;
  }
}

// TEST(gps, GLL) {
//   gps_time_t utc{3,41,38.00};
//   string s = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A";

//   gll_t gll;
//   bool ok = gll_parser(s, gll);
//   // cout << gga.lat << " " << gga.lon << endl;
//   EXPECT_TRUE(ok);
//   EXPECT_EQ(utc, gll.utc);
//   EXPECT_FLOAT_EQ(gll.lat, 39. + 53.88008971/60.);
//   EXPECT_FLOAT_EQ(gll.lon, -105. - 6.75318910/60.);
// }

    // else if (id == gci::GpsID::RMC) {
    //   gps_time_t utc{23,48,5.678};
    //   rmc_t rmc;
    //   ok = gps.get_msg(rmc);

    //   EXPECT_TRUE(ok);
    //   EXPECT_EQ(utc, rmc.utc);
    //   EXPECT_FLOAT_EQ(rmc.lat, 39. + 6.7106/60.);
    //   EXPECT_FLOAT_EQ(rmc.lon, -121. - 20.3144/60.);
    // }
    // else if (id == gci::GpsID::GLL) {
    //   gps_time_t utc{3,41,38.00};
    //   gll_t gll;
    //   bool ok = gps.get_msg(gll);
    //   EXPECT_TRUE(ok);
    //   EXPECT_EQ(utc, gll.utc);
    //   EXPECT_FLOAT_EQ(gll.lat, 39. + 53.88008971/60.);
    //   EXPECT_FLOAT_EQ(gll.lon, -105. - 6.75318910/60.);
    // }

// TEST(gps, RMC1) {
//   gps_time_t utc{23,48,5.678};
//   gps_date_t date{23,8,12};
//   char s[] = "$GPRMC,234805.678,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D\0";

//   rmc_t rmc;
//   bool ok = rmc_parser(s, rmc);
//   // cout << rmc.lat << " " << rmc.lon << endl;
//   EXPECT_TRUE(ok);
//   EXPECT_EQ(utc, rmc.utc);
//   EXPECT_FLOAT_EQ(rmc.lat, 39. + 6.7106/60.);
//   EXPECT_FLOAT_EQ(rmc.lon, -121. - 20.3144/60.);
//   EXPECT_EQ(rmc.date, date);
// }

// TEST(gps, RMC2) {
//   gps_time_t utc{23,48,5.678};
//   gps_date_t date{23,8,12};
//   char s[] = "$GPRMC,234805.678,A,3906.7106,S,12120.3144,E,0.17,95.16,120823,,,A*4D\0";

//   rmc_t rmc;
//   bool ok = rmc_parser(s, rmc);
//   // cout << rmc.lat << " " << rmc.lon << endl;
//   EXPECT_TRUE(ok);
//   EXPECT_EQ(utc, rmc.utc);
//   EXPECT_FLOAT_EQ(rmc.lat, -39. - 6.7106/60.);
//   EXPECT_FLOAT_EQ(rmc.lon, 121. + 20.3144/60.);
//   EXPECT_EQ(rmc.date, date);
// }

// TEST(gps, GPS1) {
//   gps_time_t utc{23,48,5.678};
//   string s = "3.14frg$GPRMC,234805.678,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D\r\na\t\r";

//   gci::GPS gps;
//   bool ok = false;
//   for (const char& c: s) {
//     // don't want to read junk at end of string
//     if (!ok) ok = gps.read(c);
//   }

//   EXPECT_TRUE(ok); // good message

//   gci::GpsID id = gps.get_id();
//   EXPECT_EQ(id, gci::GpsID::RMC);

//   rmc_t rmc;
//   ok = gps.get_msg(rmc);

//   EXPECT_TRUE(ok);
//   EXPECT_EQ(utc, rmc.utc);
//   EXPECT_FLOAT_EQ(rmc.lat, 39. + 6.7106/60.);
//   EXPECT_FLOAT_EQ(rmc.lon, -121. - 20.3144/60.);
// }
