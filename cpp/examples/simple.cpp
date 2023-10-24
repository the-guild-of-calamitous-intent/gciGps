
#include <gcigps.hpp>
#include <iostream>

using namespace std;

// void rmc() {
//   char s[] = "$GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D\0";
//   printf("%s\n", s);

//   rmc_t msg;
//   bool ok = rmc_parser(s, msg);
//   cout << "RMC: " << msg.lat << " " << msg.lon << endl;
// }

void gga() {
  char s[] = "$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\0";
  printf("%s\n", s);
  gga_t msg;
  bool ok = gga_parser(s, msg);
  // cout << "GGA: " << msg.lat << " " << msg.lon << endl;
}

void gsa() {
  char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\0";
  // char s[] = "$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\0";

  printf("%s\n", s);
  gsa_t gsa;
  bool ok = gsa_parser(s, gsa);
  printf(">> gsa_t: %f %f %f\n", gsa.pdop, gsa.hdop, gsa.vdop);
}

int main() {
  // rmc();
  gsa();
  gga();
  return 0;
}