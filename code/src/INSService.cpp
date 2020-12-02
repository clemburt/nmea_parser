// *************************************************************************************
// For parsing IXS (IxBlue) Proprietary NMEA sentences
//
// Based on the following document:
// https://www.igp.de/manuals/7-INS-InterfaceLibrary_MU-INSIII-AN-001-O.pdf
// *************************************************************************************


#include <nmeaparse/INSService.h>
#include <nmeaparse/NumberConversion.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace std::chrono;

using namespace nmea;


// ------------- INSSERVICE CLASS -------------

INSService::INSService(NMEAParser& parser) {
	attachToParser(parser);		// attach to parser in the INS object
}

INSService::~INSService() {
	// TODO Auto-generated destructor stub
}

void INSService::attachToParser(NMEAParser& _parser){

	_parser.setSentenceHandler("AIPOV", [this](const NMEASentence& nmea){
		this->read_AIPOV(nmea);
	});
	_parser.setSentenceHandler("PASHR", [this](const NMEASentence& nmea){
		this->read_TECHSAS(nmea);
	});
	_parser.setSentenceHandler("PHOCT", [this](const NMEASentence& nmea){
		this->read_IXSEA_TAH(nmea);
	});

}


void INSService::read_AIPOV(const NMEASentence& nmea){
	
	/*
	AIPOV Sentence see p.171
    Sentence format is:
    $AIPOV,hhmmss.ssss,h.hhh,r.rrr,p.ppp,x.xxx,y.yyy,z.zzz,e.ee,f.ff,g.gg,LL.LLLLLLLL,ll.llllllll,
	a.aaa,i.iii,j.jjj,k.kkk,m.mmm,n.nnn,o.ooo,c.ccc,hhhhhhhh*hh<CR><LF>

	| Index |   Format    | Parameter name            | Unit            | Range          | More Info                        |
	|-------|-------------|---------------------------|-----------------|----------------|----------------------------------|
	[ 0]      hhmmss.ssss : UTC Time
	[ 1]      h.hhh       : Heading                     deg               0-360            0: North 90: East
	[ 2]      r.rrr       : Roll                        deg               +/-180           >0 when left wing goes up
	[ 3]      p.ppp       : Pitch                       deg               +/-90            >0 when nose up
	[ 4]      x.xxx       : Rotation Rate XV1           deg/sec           +/-750           >0 when left wing goes up
	[ 5]      y.yyy       : Rotation Rate XV2           deg/sec           +/-750           >0 when nose up
	[ 6]      z.zzz       : Rotation Rate XV3           deg/sec           +/-750           >0 clockwise
	[ 7]      e.ee        : Linear Acceleration XV1     m/s2              +/-147.15 (15g)  >0 when going forward
	[ 8]      f.ff        : Linear Acceleration XV2     m/s2              +/-147.15 (15g)  >0 when going right
	[ 9]      g.gg        : Linear Acceleration XV3     m/s2              +/-147.15 (15g)  >0 when going down
	[10]      LL.LLLLLLLL : Latitude                    deg               +/-90
	[11]      ll.llllllll : Longitude                   deg               +/-180
	[12]      a.aaa       : Altitude                    m                 15000
	[13]      i.iii       : North Velocity              m/s               +/-250           >0 when going north
	[14]      j.jjj       : East Velocity               m/s               +/-250           >0 when going east
	[15]      k.kkk       : Vertical Velocity           m/s               +/-250           >0 when going down
	[16]      m.mmm       : Along Velocity XV1          m/s               +/-250           >0 when going forward
	[17]      n.nnn       : Across Velocity XV2         m/s               +/-250           >0 when going right
	[18]      o.ooo       : Down Velocity XV3           m/s               +/-250           >0 when going down
	[19]      c.ccc       : True Course                 deg                                direction of horizontal speed
	[20]      hhhhhhhh    : User Status                 hex
	[20]      hh          : Checksum                    hex
	*/
	
	try
	{
		
		if (!nmea.checksumOK()){
			throw NMEAParseError("Checksum is invalid!");
		}
		
		if (nmea.parameters.size() < 21){
			throw NMEAParseError("INS data is missing parameters.");
		}
		
		this->fix.timestamp.setTime(parseDouble(nmea.parameters[0]));
		
		this->fix.heading = parseDouble(nmea.parameters[1]);
		this->fix.roll = parseDouble(nmea.parameters[2]);
		this->fix.pitch = parseDouble(nmea.parameters[3]);

		this->fix.rotation_rate_xv1 = parseDouble(nmea.parameters[4]);
		this->fix.rotation_rate_xv2 = parseDouble(nmea.parameters[5]);
		this->fix.rotation_rate_xv3 = parseDouble(nmea.parameters[6]);

		this->fix.linear_acceleration_xv1 = parseDouble(nmea.parameters[7]);
		this->fix.linear_acceleration_xv2 = parseDouble(nmea.parameters[8]);
		this->fix.linear_acceleration_xv3 = parseDouble(nmea.parameters[9]);

		this->fix.latitude = parseDouble(nmea.parameters[10]);
		this->fix.longitude = parseDouble(nmea.parameters[11]);
		this->fix.altitude = parseDouble(nmea.parameters[12]);

		this->fix.north_velocity = parseDouble(nmea.parameters[13]);
		this->fix.east_velocity = parseDouble(nmea.parameters[14]);
		this->fix.vertical_velocity = parseDouble(nmea.parameters[15]);

		this->fix.along_velocity_xv1 = parseDouble(nmea.parameters[16]);
		this->fix.across_velocity_xv2 = parseDouble(nmea.parameters[17]);
		this->fix.down_velocity_xv3 = parseDouble(nmea.parameters[18]);

		this->fix.true_course = parseDouble(nmea.parameters[19]);

		this->fix.user_status = nmea.parameters[20];

	}

	catch (NumberConversionError& ex)
	{
		NMEAParseError pe("INS Number Bad Format [$AIPOV] :: " + ex.message, nmea);
		throw pe;
	}

	catch (NMEAParseError& ex)
	{
		NMEAParseError pe("INS Data Bad Format [$AIPOV] :: " + ex.message, nmea);
		throw pe;
	}

}


void INSService::read_TECHSAS(const NMEASentence& nmea){
	
	/*
	TECHSAS Sentence see p.247
    Sentence format is:
    $PASHR,hhmmss.sss,H.HH,T,aR.RR,bP.PP,cD.DD,r.rrr,p.ppp,h.hhh,x,y*hh<CR><LF>

	| Index |   Format    | Parameter name            | Unit            | Range          | More Info                        |
	|-------|-------------|---------------------------|-----------------|----------------|----------------------------------|
	[ 0]      hhmmss.sss  : UTC Time
	[ 1]      H.HH        : Heading                     deg               0-360            0: North 90: East
	[ 2]      T           : Fixed Character
	[ 3]      aR.RR       : Roll                        deg               +/-180           >0 when port up
	[ 4]      bP.PP       : Pitch                       deg               +/-90            >0 when bow up
	[ 5]      cD.DD       : Heave                       m                                  >0 when PHINS goes down
	[ 6]      r.rrr       : Roll Standard Deviation     deg               +/-180
	[ 7]      p.ppp       : Pitch Standard Deviation    deg               +/-90
	[ 8]      h.hhh       : Heading Standard Deviation  deg               0-360
	[ 9]      x           : GPS Aiding Status Flag                        0/1              1 when GPS received and valid, otherwise 0
	[10]      y           : Sensor Error Status Flag                      0/1              1 when ACC or FOG error, otherwise 0
	[10]      hh          : Checksum                    hex
	*/
	
	try
	{
		
		if (!nmea.checksumOK()){
			throw NMEAParseError("Checksum is invalid!");
		}
		
		if (nmea.parameters.size() < 11){
			throw NMEAParseError("INS data is missing parameters.");
		}
		
		this->fix.timestamp.setTime(parseDouble(nmea.parameters[0]));
		
		this->fix.heading = parseDouble(nmea.parameters[1]);

		this->fix.T = nmea.parameters[2];

		this->fix.roll = parseDouble(nmea.parameters[3]);
		this->fix.pitch = parseDouble(nmea.parameters[4]);
		this->fix.heave = parseDouble(nmea.parameters[5]);

		this->fix.roll_standard_deviation = parseDouble(nmea.parameters[6]);
		this->fix.pitch_standard_deviation = parseDouble(nmea.parameters[7]);
		this->fix.heading_standard_deviation = parseDouble(nmea.parameters[8]);

		this->fix.x = parseDouble(nmea.parameters[9]);
		this->fix.y = parseDouble(nmea.parameters[10]);

	}

	catch (NumberConversionError& ex)
	{
		NMEAParseError pe("INS Number Bad Format [$PASHR] :: " + ex.message, nmea);
		throw pe;
	}
	
	catch (NMEAParseError& ex)
	{
		NMEAParseError pe("INS Data Bad Format [$PASHR] :: " + ex.message, nmea);
		throw pe;
	}

}


void INSService::read_IXSEA_TAH(const NMEASentence& nmea){
	
	/*
	IXSEA TAH Sentence see p.201
    Sentence format is:
    $PHOCT,01,hhmmss.sss,G,AA,HHH.HHH,N,eRRR.RRR,L,ePP.PPP,K,eFF.FFF,
	M,eHH.HHH,eSS.SSS,eWW.WWW,eZZ.ZZZ,eYY.YYY,eXX.XXX,eQQQ.QQ*hh

	| Index |   Format    | Parameter name           | Unit            | Range          | More Info                        |
	|-------|-------------|--------------------------|-----------------|----------------|----------------------------------|
	[ 0]      01          : Protocol version id
	[ 1]      hhmmss.sss  : UTC Time
	[ 2]      G           : UTC Time status                                               T=Valid, E=Invalid
	[ 3]      AA          : Latency (for r/p/h)                          03/25            AA=03 (OCTANS IV & PHINSIII), AA= 25 (OCTANSIII)
	[ 4]      HHH.HHH     : True Heading               deg               0-359.999        >0 when nose up
	[ 5]      N           : True Heading Status                                           T=Valid, E=Invalid, I=Initializing
	[ 6]      eRRR.RRR    : Roll                       deg               +/-180           >0 if port side up
	[ 7]      L           : Roll Status                                                   T=Valid, E=Invalid, I=Initializing
	[ 8]      ePP.PPP     : Pitch                      deg               +/-90            >0 if bow down
	[ 9]      K           : Pitch Status                                                  T=Valid, E=Invalid, I=Initializing
	[10]      eFF.FFF     : Heave (No Lever Arms)      m                 +/-99.999
	[11]      M           : Heave Status                                                  T=Valid, E=Invalid, I=Initializing
	[12]      eHH.HHH     : Heave (Lever Arms)         m                 +/-99.999
	[13]      eSS.SSS     : Surge (Lever Arms)         m                 +/-99.999
	[14]      eWW.WWW     : Sway (Lever Arms)          m                 +/-99.999
	[15]      eZZ.ZZZ     : Heave Speed (Lever Arms)   m/s               +/-99.999
	[16]      eYY.YYY     : Surge Speed (Lever Arms)   m/s               +/-99.999
	[17]      eXX.XXX     : Sway Speed (Lever Arms)    m/s               +/-99.999
	[18]      eQQQQ.QQ    : Heading Rate Of Turns      deg/min           +/-9999.99
	[18]      hh          : Checksum                   hex
	*/
	
	try
	{
		
		if (!nmea.checksumOK()){
			throw NMEAParseError("Checksum is invalid!");
		}
		
		if (nmea.parameters.size() < 19){
			throw NMEAParseError("INS data is missing parameters.");
		}
		
		this->fix.protocol_version_id = parseDouble(nmea.parameters[0]);

		this->fix.timestamp.setTime(parseDouble(nmea.parameters[1]));
		
		this->fix.utc_time_status = nmea.parameters[2];

		this->fix.latency = parseInt(nmea.parameters[3]);

		this->fix.true_heading = parseDouble(nmea.parameters[4]);
		this->fix.true_heading_status = nmea.parameters[5];

		this->fix.roll = parseDouble(nmea.parameters[6]);
		this->fix.roll_status = nmea.parameters[7];

		this->fix.pitch = parseDouble(nmea.parameters[8]);
		this->fix.pitch_status = nmea.parameters[9];

		this->fix.heave_no_lever_arms = parseDouble(nmea.parameters[10]);
		this->fix.heave_status = nmea.parameters[11];
		
		this->fix.heave = parseDouble(nmea.parameters[12]);
		this->fix.surge = parseDouble(nmea.parameters[13]);
		this->fix.sway = parseDouble(nmea.parameters[14]);

		this->fix.heave_speed = parseDouble(nmea.parameters[15]);
		this->fix.surge_speed = parseDouble(nmea.parameters[16]);
		this->fix.sway_speed = parseDouble(nmea.parameters[17]);

		this->fix.heading_rate = parseDouble(nmea.parameters[18]);

	}

	catch (NumberConversionError& ex)
	{
		NMEAParseError pe("INS Number Bad Format [$IXSEA_TAH] :: " + ex.message, nmea);
		throw pe;
	}

	catch (NMEAParseError& ex)
	{
		NMEAParseError pe("INS Data Bad Format [$IXSEA_TAH] :: " + ex.message, nmea);
		throw pe;
	}

}





