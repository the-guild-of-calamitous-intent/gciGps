

#  Different commands to set the update rate from once a second (1 Hz) to 10 times
#  a second (10Hz) Note that these only control the rate at which the position is
#  echoed, to actually speed up the position fix you must also send one of the
#  position fix rate commands below too.
# PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ = "$PMTK220,10000*2F" # Once every 10 seconds, 100 millihertz.
# PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ = "$PMTK220,5000*1B" # Once every 5 seconds, 200 millihertz.
PMTK_SET_NMEA_UPDATE_1HZ = "$PMTK220,1000*1F" #  1 Hz
# PMTK_SET_NMEA_UPDATE_2HZ = "$PMTK220,500*2B"  #  2 Hz
PMTK_SET_NMEA_UPDATE_5HZ = "$PMTK220,200*2C"  #  5 Hz
# PMTK_SET_NMEA_UPDATE_10HZ = "$PMTK220,100*2F" # 10 Hz
# Position fix update rate commands.
# PMTK_API_SET_FIX_CTL_100_MILLIHERTZ = "$PMTK300,10000,0,0,0,0*2C" # Once every 10 seconds, 100 millihertz.
# PMTK_API_SET_FIX_CTL_200_MILLIHERTZ = "$PMTK300,5000,0,0,0,0*18" # Once every 5 seconds, 200 millihertz.
PMTK_API_SET_FIX_CTL_1HZ = "$PMTK300,1000,0,0,0,0*1C" # 1 Hz
PMTK_API_SET_FIX_CTL_5HZ = "$PMTK300,200,0,0,0,0*2F"  # 5 Hz
# Can't fix position faster than 5 times a second!

PMTK_SET_BAUD_115200 = "$PMTK251,115200*1F" # 115200 bps
PMTK_SET_BAUD_57600 = "$PMTK251,57600*2C"   #  57600 bps
PMTK_SET_BAUD_9600 = "$PMTK251,9600*17"     #   9600 bps

# Messages
PMTK_SET_NMEA_OUTPUT_GLLONLY = "$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on only the GPGLL sentence
PMTK_SET_NMEA_OUTPUT_RMCONLY = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on only the GPRMC sentence
PMTK_SET_NMEA_OUTPUT_VTGONLY = "$PMTK314,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on only the GPVTG
PMTK_SET_NMEA_OUTPUT_GGAONLY = "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on just the GPGGA
PMTK_SET_NMEA_OUTPUT_GSAONLY = "$PMTK314,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on just the GPGSA
PMTK_SET_NMEA_OUTPUT_GSVONLY = "$PMTK314,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on just the GPGSV
PMTK_SET_NMEA_OUTPUT_RMCGGA = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28" # turn on GPRMC and GPGGA
PMTK_SET_NMEA_OUTPUT_RMCGGAGSA = "$PMTK314,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" # turn on GPRMC, GPGGA and GPGSA
PMTK_SET_NMEA_OUTPUT_ALLDATA = "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28" # turn on ALL THE DATA
PMTK_SET_NMEA_OUTPUT_OFF = "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28" # turn off output

# to generate your own sentences, check out the MTK command datasheet and use a
# checksum calculator such as the awesome
# http://www.hhhh.org/wiml/proj/nmeaxor.html

PMTK_ENABLE_WAAS = "$PMTK301,2*2E" # Use WAAS for DGPS correction data
PMTK_STANDBY = "$PMTK161,0*28" # standby command & boot successful message
PMTK_STANDBY_SUCCESS = "$PMTK001,161,3*36" # Not needed currently
PMTK_AWAKE = "$PMTK010,002*2D"             # Wake up
PMTK_Q_RELEASE = "$PMTK605*31" # ask for the release and version
PGCMD_ANTENNA = "$PGCMD,33,1*6C" # request for updates on antenna status
PGCMD_NOANTENNA = "$PGCMD,33,0*6D" # don't show antenna status messages
