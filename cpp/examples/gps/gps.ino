#include <gcigps.hpp>
// #include <string>

using namespace std;

#define hostser Serial
#define gpsser Serial1

gci::GPS gps;

void setup() {
  hostser.begin(1000000);

  gpsser.begin(9600);
  gpsser.print(GCI_BAUD_115200);

  delay(500);

  gpsser.begin(115200);
  gpsser.print(GCI_RMCGGA);
  gpsser.print(GCI_UPDATE_1HZ);
  gpsser.print(GCI_NOANTENNA);

  delay(500);

  // string s = "START";
  // hostser.println(s.c_str());
}

void loop() {

  gga_t gga;
  rmc_t rmc;
  int avail = gpsser.available();
  bool ok = false;
  while (avail--) {
    int c = gpsser.read();
    ok = gps.read(c);
    if (ok) break;
  }
  if (!ok) return;

  gci::GpsID id = gps.get_id();
  if (id == gci::GpsID::GGA) {
    ok = gps.get_msg(gga);
    if (!ok) return;
    // hostser.write(reinterpret_cast<uint8_t *>(&gga), sizeof(gga_t));
    // hostser.printf("GGA: %f %f\n", gga.lat, gga.lon);
    hostser.print("GGA: ");
    hostser.print(gga.lat);
    hostser.print(" ");
    hostser.println(gga.lon);
  }
  else if (id == gci::GpsID::RMC) {
    ok = gps.get_msg(rmc);
    if (!ok) return;
    // hostser.write(reinterpret_cast<uint8_t *>(&rmc), sizeof(rmc_t));
    // hostser.printf("RMC: %f %f\n", rmc.lat, rmc.lon);
    hostser.print("RMC: ");
    hostser.print(rmc.lat);
    hostser.print(" ");
    hostser.println(rmc.lon);

  }
}
