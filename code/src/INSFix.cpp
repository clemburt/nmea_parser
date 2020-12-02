#include <nmeaparse/INSFix.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

using namespace nmea;


// ===========================================================
// ======================== INS TIMESTAMP ====================
// ===========================================================


INSTimestamp::INSTimestamp(){
	hour = 0;
	min = 0;
	sec = 0;
	microsec = 0;

	month = 1;
	day = 1;
	year = 1970;

	rawTime = 0;
	rawDate = 0;
};

// indexed from 1!
std::string INSTimestamp::monthName(uint32_t index){
	if (index < 1 || index > 12){
		std::stringstream ss;
		ss << "[month:" << index << "]";
		return ss.str();
	}

	std::string names[] = {
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};
	return names[index - 1];
};

// Returns seconds since Jan 1, 1970. Classic Epoch time.
time_t INSTimestamp::getTime() {
	struct tm t = { 0 };
	t.tm_year = year - 1900;	// This is year-1900, so 112 = 2012
	t.tm_mon = month;			// month from 0:Jan
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = (int)sec;
	return mktime(&t);
}

void INSTimestamp::setTime(double raw_ts){
	rawTime = trunc(raw_ts);

	hour = (int32_t)trunc(trunc(raw_ts) / 10000.0);
	min = (int32_t)trunc((trunc(raw_ts) - hour * 10000) / 100.0);
	sec = (int32_t)trunc(trunc(raw_ts) - min * 100 - hour * 10000);
	microsec = (int32_t)round((raw_ts - trunc(raw_ts)) * 1000000);
}

//ddmmyy
void INSTimestamp::setDate(int32_t raw_date){
	rawDate = raw_date;
	// If uninitialized, use posix time.
	if(rawDate == 0) {
		month = 1;
		day = 1;
		year = 1970;
	}
	else {
		day = (int32_t)trunc(raw_date / 10000.0);
		month = (int32_t)trunc((raw_date - 10000 * day) / 100.0);
		year = raw_date - 10000 * day - 100 * month + 2000;
	}
}

std::string INSTimestamp::toString(){
	std::stringstream ss;
	ss << hour << "h " << min << "m " << sec << "s " << microsec << "*10e-6s UTC " << "(" << monthName(month) << " " << day << " " << year << ")";
	return ss.str();
};



// =====================================================
// ======================== INS FIX ====================
// =====================================================

INSFix::INSFix() {

	// =========================== AIPOV =====================================

	heading = 0;
	roll = 0;
	pitch = 0;

	rotation_rate_xv1 = 0;
	rotation_rate_xv2 = 0;
	rotation_rate_xv3 = 0;

	linear_acceleration_xv1 = 0;
	linear_acceleration_xv2 = 0;
	linear_acceleration_xv3 = 0;

	latitude = 0;
	longitude = 0;
	altitude = 0;

	north_velocity = 0;
	east_velocity = 0;
	vertical_velocity = 0;

	along_velocity_xv1 = 0;
	across_velocity_xv2 = 0;
	down_velocity_xv3 = 0;

	true_course = 0;

	user_status = "";

	// =========================== TECHSAS =====================================

	T = "T";

	heave = 0;

	roll_standard_deviation = 0;
	pitch_standard_deviation = 0;
	heading_standard_deviation = 0;

	x = 0;
	y = 0;

	// =========================== IXSEA_TAH =====================================

	protocol_version_id = 0;

	utc_time_status = "T";

	latency = 0;

	true_heading = 0;
	
	true_heading_status = "T";
	roll_status = "T";
	pitch_status = "T";

	heave_no_lever_arms = 0;
	std::string heave_status = "T";
	
	//heave = 0;
	surge = 0;
	sway = 0;

	heave_speed = 0;
	surge_speed = 0;
	sway_speed = 0;
	
	heading_rate = 0;

}


INSFix::~INSFix() {
	// TODO Auto-generated destructor stub
}


std::string INSFix::toString_AIPOV(){
	
	stringstream ss;

	ss << "================================= AIPOV =================================" << endl
		<< "   [ 0]  Timestamp                  : " << timestamp.toString() << endl
		<< "   [ 1]  Heading                    : " << heading << " deg" << endl
		<< "   [ 2]  Roll                       : " << roll << " deg" << endl
		<< "   [ 3]  Pitch                      : " << pitch << " deg" << endl
		<< "   [ 4]  Rotation Rate XV1          : " << rotation_rate_xv1 << " deg/sec" <<  endl
		<< "   [ 5]  Rotation Rate XV2          : " << rotation_rate_xv2 << " deg/sec" <<  endl
		<< "   [ 6]  Rotation Rate XV3          : " << rotation_rate_xv3 << " deg/sec" <<  endl
		<< "   [ 7]  Linear Acceleration XV1    : " << linear_acceleration_xv1 << " m/s2" << endl
		<< "   [ 8]  Linear Acceleration XV2    : " << linear_acceleration_xv2 << " m/s2" << endl
		<< "   [ 9]  Linear Acceleration XV3    : " << linear_acceleration_xv3 << " m/s2" << endl
		<< "   [10]  Latitude                   : " << latitude << " deg" << endl
		<< "   [11]  Longitude                  : " << longitude << " deg" << endl
		<< "   [12]  Altitude                   : " << altitude << " m" << endl
		<< "   [13]  North Velocity             : " << north_velocity << " m/s" << endl
		<< "   [14]  East Velocity              : " << east_velocity << " m/s" << endl
		<< "   [15]  Vertical Velocity          : " << vertical_velocity << " m/s" << endl
		<< "   [16]  Along Velocity XV1         : " << along_velocity_xv1 << " m/s" << endl
		<< "   [17]  Across Velocity XV2        : " << across_velocity_xv2 << " m/s" << endl
		<< "   [18]  Down Velocity XV3          : " << down_velocity_xv3 << " m/s" << endl
		<< "   [19]  True Course                : " << true_course << " deg" << endl
		<< "   [20]  User Status                : " << user_status << endl;

	return ss.str();
}


std::string INSFix::toString_TECHSAS(){
	
	stringstream ss;
	
	ss << "================================= TECHSAS =================================" << endl
		<< "   [ 0]  Timestamp                  : " << timestamp.toString() << endl
		<< "   [ 1]  Heading                    : " << heading << " deg" << endl
		<< "   [ 2]  Fixed Character            : " << T << endl
		<< "   [ 3]  Roll                       : " << roll << " deg" << endl
		<< "   [ 4]  Pitch                      : " << pitch << " deg" <<  endl
		<< "   [ 5]  Heave                      : " << heave << " m" <<  endl
		<< "   [ 6]  Roll Standard Deviation    : " << roll_standard_deviation << " deg" <<  endl
		<< "   [ 7]  Pitch Standard Deviation   : " << pitch_standard_deviation << " deg" << endl
		<< "   [ 8]  Heading Standard Deviation : " << heading_standard_deviation << " deg" << endl
		<< "   [ 9]  GPS Aiding Status Flag     : " << x << endl
		<< "   [10]  Sensor Error Status Flag   : " << y << endl;

	return ss.str();
}


std::string INSFix::toString_IXSEA_TAH(){
	
	stringstream ss;

	ss << "================================= AIPOV =================================" << endl
		<< "   [ 0]  Protocol version id        : " << protocol_version_id << endl
		<< "   [ 1]  UTC Time                   : " << timestamp.toString() << endl
		<< "   [ 2]  UTC Time Status            : " << utc_time_status << endl
		<< "   [ 3]  Latency (for r/p/h)        : " << latency << endl
		<< "   [ 4]  True Heading               : " << true_heading << " deg" <<  endl
		<< "   [ 5]  True Heading Status        : " << true_heading_status << endl
		<< "   [ 6]  Roll                       : " << roll << " deg" <<  endl
		<< "   [ 7]  Roll Status                : " << roll_status << endl
		<< "   [ 8]  Pitch                      : " << pitch << " deg" << endl
		<< "   [ 9]  Pitch Status               : " << pitch_status << " m/s2" << endl
		<< "   [10]  Heave (No Lever Arms)      : " << heave_no_lever_arms << " m" << endl
		<< "   [11]  Heave Status               : " << heave_status << endl
		<< "   [12]  Heave (Lever Arms)         : " << altitude << " m" << endl
		<< "   [13]  Surge (Lever Arms)         : " << surge << " m" << endl
		<< "   [14]  Sway (Lever Arms)          : " << sway << " m" << endl
		<< "   [15]  Heave Speed (Lever Arms)   : " << heave_speed << " m/s" << endl
		<< "   [16]  Surge Speed (Lever Arms)   : " << surge_speed << " m/s" << endl
		<< "   [17]  Sway Speed (Lever Arms)    : " << sway_speed << " m/s" << endl
		<< "   [18]  Heading Rate Of Turns      : " << heading_rate << " deg/min" << endl;

	return ss.str();
}