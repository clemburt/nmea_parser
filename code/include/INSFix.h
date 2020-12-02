#ifndef INSFIX_H_
#define INSFIX_H_

#include <cstdint>
#include <ctime>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <sstream>

#include <iostream> // std::hex

namespace nmea {

	class INSTimestamp;
	class INSFix;
	class INSService;


// =========================== INS TIMESTAMP =====================================

	// UTC time
	class INSTimestamp {
	private:
		std::string monthName(uint32_t index);
	public:
		INSTimestamp();

		int32_t hour;
		int32_t min;
		int32_t sec;
		int32_t microsec;

		int32_t month;
		int32_t day;
		int32_t year;

		// Values collected directly from the INS
		double rawTime;
		int32_t rawDate;

		time_t getTime();

		// Set directly from the NMEA time stamp
		// hhmmss.ssssss
		void setTime(double raw_ts);

		// Set directly from the NMEA date stamp
		// ddmmyy
		void setDate(int32_t raw_date);

		std::string toString();

	};


// =========================== INS FIX =====================================

	class INSFix {
		friend INSService;

	private:


	public:

		INSFix();
		virtual ~INSFix();

		INSTimestamp timestamp;	// UTC time

	// =========================== AIPOV =====================================

		double heading;
		double roll;
		double pitch;

		double rotation_rate_xv1;
		double rotation_rate_xv2;
		double rotation_rate_xv3;

		double linear_acceleration_xv1;
		double linear_acceleration_xv2;
		double linear_acceleration_xv3;

		double latitude;
		double longitude;
		double altitude;

		double north_velocity;
		double east_velocity;
		double vertical_velocity;

		double along_velocity_xv1;
		double across_velocity_xv2;
		double down_velocity_xv3;

		double true_course;

		std::string user_status;

		std::string toString_AIPOV();

	// =========================== TECHSAS =====================================

		std::string T;

		double heave;

		double roll_standard_deviation;
		double pitch_standard_deviation;
		double heading_standard_deviation;

		bool x;
		bool y;

		std::string toString_TECHSAS();

	// =========================== IXSEA_TAH =====================================

		int protocol_version_id;

		std::string utc_time_status;

		int latency;

		double true_heading;
		
		std::string true_heading_status;
		std::string roll_status;
		std::string pitch_status;

		double heave_no_lever_arms;
		std::string heave_status;
		
		//double heave;
		double surge;
		double sway;

		double heave_speed;
		double surge_speed;
		double sway_speed;

		double heading_rate;

		std::string toString_IXSEA_TAH();

	};

}

#endif /* INSFIX_H_ */
