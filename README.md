# nmea_parser
C++ library for parsing iXblue's NMEA sentences, adapted from open source [NemaTode library](https://github.com/ckgt/NemaTode/)

- **Parsing of the following iXblue NMEA sentences:** AIPOV ($AIPOV), TECHSAS ($PASHR) and IXSEA_TAH ($PHOCT)

Add toString functions for visualization

Modify timestamp extraction function for adding microseconds retrieving

Test file: *ixblue_sentences_demo.cpp*

- **iXblue's Phins simulator (INS):** *phins_simulator.py* 

Send AIPOV-type NMEA sentences by UDP at frequency of 25Hz
