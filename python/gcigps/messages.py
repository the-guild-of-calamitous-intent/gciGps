from collections import namedtuple

"""
$GPGGA,234804.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*51
$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
$GPRMC,234804.000,A,3906.7106,N,12120.3144,W,0.10,107.17,120823,,,A*70
$GPVTG,107.17,T,,M,0.10,N,0.19,K,A*34
$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50
$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08
$GPRMC,234805.000,A,3906.7106,N,12120.3144,W,0.17,95.16,120823,,,A*4D
$GPVTG,95.16,T,,M,0.17,N,0.31,K,A*02
"""

GSA = namedtuple("GSA", "id mode fix prns pdop hdop vdop")
RMC = namedtuple("RMC", "id utc status lat lon speed track data mag")
GGA = namedtuple("GGA", "id utc lat lon qual num_sats hdop msl geoid age ref_id")
time_t = namedtuple("time_t", "hr min sec")
date_t = namedtuple("data_t", "day month year")
sat_t = namedtuple("sat_t", "prn el az snr")