
#pragma once

#define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F" ///<  1 Hz
#define PMTK_SET_NMEA_UPDATE_2HZ "$PMTK220,500*2B"  ///<  2 Hz
#define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"  ///<  5 Hz
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F" ///< 10 Hz
// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ                                    \
  "$PMTK300,10000,0,0,0,0*2C" ///< Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ                                    \
  "$PMTK300,5000,0,0,0,0*18" ///< Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ "$PMTK300,1000,0,0,0,0*1C" ///< 1 Hz
#define PMTK_API_SET_FIX_CTL_5HZ "$PMTK300,200,0,0,0,0*2F"  ///< 5 Hz
// Can't fix position faster than 5 times a second!

#define PMTK_SET_BAUD_115200 "$PMTK251,115200*1F" ///< 115200 bps
#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"   ///<  57600 bps
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"     ///<   9600 bps

#define PMTK_ENABLE_SBAS                                                       \
  "$PMTK313,1*2E" ///< Enable search for SBAS satellite (only works with 1Hz
                  ///< output rate)
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E" ///< Use WAAS for DGPS correction data

#define PMTK_SET_NMEA_OUTPUT_GLLONLY                                           \
  "$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on only the
                                                      ///< GPGLL sentence
#define PMTK_SET_NMEA_OUTPUT_RMCONLY                                           \
  "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on only the
                                                      ///< GPRMC sentence
#define PMTK_SET_NMEA_OUTPUT_VTGONLY                                           \
  "$PMTK314,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on only the
                                                      ///< GPVTG
#define PMTK_SET_NMEA_OUTPUT_GGAONLY                                           \
  "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on just the
                                                      ///< GPGGA
#define PMTK_SET_NMEA_OUTPUT_GSAONLY                                           \
  "$PMTK314,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on just the
                                                      ///< GPGSA
#define PMTK_SET_NMEA_OUTPUT_GSVONLY                                           \
  "$PMTK314,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on just the
                                                      ///< GPGSV
#define PMTK_SET_NMEA_OUTPUT_RMCGGA                                            \
  "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on GPRMC and
                                                      ///< GPGGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGAGSA                                         \
  "$PMTK314,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on GPRMC, GPGGA
                                                      ///< and GPGSA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA                                           \
  "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on ALL THE DATA