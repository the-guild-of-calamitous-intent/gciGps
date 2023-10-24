#include <gtest/gtest.h>
#include <string>
#include "gcigps.hpp"
#include <iostream>
#include <vector>

using namespace std;

/*
$GPRMC,002441.093,V,,,,,0.00,0.00,231023,,,N*45
$GPGGA,002442.089,,,,,0,00,,,M,,M,,*79
Bad parsing
$GPRMC,002442.089,V,,,,,0.00,0.00,231023,,,N*4D
$GPGGA,002443.089,,,,,0,00,,,M,,M,,*78
Bad parsing
$GPRMC,002443.089,V,,,,,0.00,0.00,231023,,,N*4C
$GPGGA,002444.089,,,,,0,00,,,M,,M,,*7F
Bad parsing
$GPRMC,002444.089,V,,,,,0.00,0.00,231023,,,N*4B
$GPGGA,002445.089,,,,,0,00,,,M,,M,,*7E
Bad parsing
$GPRMC,002445.089,V,,,,,0.00,0.00,231023,,,N*4A
$GPGGA,002446.089,,,,,0,00,,,M,,M,,*7D
Bad parsing
$GPRMC,002446.089,V,,,,,0.00,0.00,231023,,,N*49
*/


TEST(gps, GGA1) {
  gps_time_t utc{23,48,5.000};
  char s[] = "$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\0";

  bool ans = gci::valid_gps_msg(s);
  EXPECT_TRUE(ans);

  gga_t gga;
  bool ok = gga_parser(s, gga);
  // cout << gga.lat << " " << gga.lon << endl;
  EXPECT_TRUE(ok);
  EXPECT_EQ(utc, gga.utc);
  EXPECT_FLOAT_EQ(gga.lat, atof("39") + atof("06.7106")/60.0f);
  EXPECT_FLOAT_EQ(gga.lon, -1. * (atof("121") + atof("20.3144")/60.0f));
}

TEST(gps, GGA2) {
  gps_time_t utc{23,48,5.000};
  char s[] = "$GPGGA,234805.000,,,,,,,,,,,,,*50\0";

  bool ans = gci::valid_gps_msg(s);
  EXPECT_FALSE(ans);

  gga_t gga;
  bool ok = gga_parser(s, gga);
  EXPECT_FALSE(ok);
}

TEST(gps, GSA1) {
  char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\0";

  gsa_t gsa;
  bool ok = gsa_parser(s, gsa);
  EXPECT_TRUE(ok);
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

// TEST(gps, GSA2) {
//   char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,,,*08\0";

//   gsa_t gsa;
//   bool ok = gsa_parser(s, gsa);
//   EXPECT_FALSE(ok);
// }

TEST(gps, GPS) {
  gps_time_t utc{23,48,5.678};
  vector<string> v{
    "3.14frg$GPRMC,234805.678,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D\r\na\t\r",
    "\n\n$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\r\na\t\r",
    "49fmdd$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\r\n",
    "49,f,m$GPGSA,,,,,,,,,,,,,,,2.01,1.77,0.95*08\r\n\t",
    "234$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A\r\n"
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

      ok = gps.get_msg(gga);
      // cout << gga.lat << " " << gga.lon << endl;

      EXPECT_TRUE(ok);
      EXPECT_EQ(utc, gga.utc);
      EXPECT_FLOAT_EQ(gga.lat, 39. + 6.7106/60.);
      EXPECT_FLOAT_EQ(gga.lon, -121. - 20.3144/60.);
    }
    else if (id == gci::GpsID::GSA) {
      gsa_t gsa;
      // cout << "GSA" << endl;

      ok = gps.get_msg(gsa);

      EXPECT_TRUE(ok);
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
    else EXPECT_TRUE(false);
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
