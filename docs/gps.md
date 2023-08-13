# GPS

- [reference](https://shadyelectronics.com/gps-nmea-sentence-structure/)

## GGA – Global Positioning System Fix Data

Time, position, and fix related data for a GNSS receiver.

Format:

```
$<TalkerID>GGA,<Timestamp>,<Lat>,<N/S>,<Long>,<E/W>,<GPSQual>,<Sats>,<HDOP>,<Alt>,<AltVal>,<GeoSep>,<GeoVal>,<DGPSAge>,<DGPSRef>*<checksum><CR><LF>
```

```
$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55
```

| Field | Char | Example | Description |
|-------|------|---------|-------------|
| `$` | Char | `$` | Each NMEA message starts with “$“
| TalkerID | string[2] | GN | The talker ID.GP: For GPSGL: For GLONASSGA: For GalileoGB: For BeiDouGI: For NAVIC (IRNSS)GQ: For QZSSGN: For multi-constellation mode
| Timestamp | hhmmss.sss | 062735.00 | UTC time of GNSS sample:hh: hours (Fixed two digits)mm: minutes (Fixed two digits)ss: seconds (Fixed two digits).ss: decimal fraction of seconds
| Lat | DDMM.MMMMM | 3150.788156 | Latitude as degrees:DD: Degrees (Fixed two digits)MM: Minutes (Fixed two digits).MMMMMM: Decimal fraction of minutes
| N/S | N or S | N | Latitude direction:N = NorthS = South
| Long | DDDMM.MMMMM | 11711.922383 | Longitude as degrees:DDD: Degree (Fixed three digits)MM: Minutes (Fixed two digits).MMMMMM: Decimal fraction of minutes
| E/W | E or W | E | Longitude direction:E = EastW = West
|GPSQual | int | 1 | 0 = Fix not available or invalid1 = GPS, SPS Mode, fix valid2 = Differential GPS, SPS Mode, fix valid6 = Estimated (dead reckoning) mo
| Sats | int[2] | 12 | Number of satellites in use.
| HDOP | x.x | 2.0 | Horizontal dilution of precision.
| Alt | x.x | 90.0 | Height above mean sea level.
| AltVal | M | M | Reference unit for altitude:M = meter
| GeoSep | x.x | Null | Geoidal separation measures in meters.
| Geoval | M | M | Reference unit for geoidal separation:M = meters
| DGPSAge | x.x	| Null | Age of differential corrections, sec
| DGPSRef | xxxx | Null | Differential reference station ID
| * | * | End | character of data field
| Checksum | 55 | Hexadecimal | checksum

## RMC – Recommended minimum specific GPS/Transit data

Time, date, position, course, and speed data provided by a GNSS receiver.

Format:

```
$<TalkerID>RMC,<Timestamp>,<Status>,<Lat>,<N/S>,<Long>,<E/W>,<SOG>,<COG>,<Date>,<MagVar>,<MagVarDir>,<mode>,<NavStatus>*<checksum><CR><LF>
```

```
$GNRMC,060512.00,A,3150.788156,N,11711.922383,E,0.0,,311019,,,A,V*1B
```

# References

- [shady electronics](https://shadyelectronics.com/gps-nmea-sentence-structure/#:~:text=If%20you%20just%20need%20%E2%80%9CGPS,have%20to%20use%20RMC%20sentences.)