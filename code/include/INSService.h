#ifndef INSSERVICE_H_
#define INSSERVICE_H_

#include <string>
#include <chrono>
#include <functional>
#include <nmeaparse/INSFix.h>
#include <nmeaparse/NMEAParser.h>
#include <nmeaparse/Event.h>

namespace nmea {

class INSService {

private:

	void read_AIPOV(const NMEASentence& nmea); // $AIPOV
	void read_TECHSAS(const NMEASentence& nmea); // $PASHR
	void read_IXSEA_TAH(const NMEASentence& nmea); // $PHOCT

public:

	INSFix fix;

	INSService(NMEAParser& parser);
	virtual ~INSService();

	void attachToParser(NMEAParser& parser);			// will attach to this parser's nmea sentence events

};


}

#endif /* INSSERVICE_H_ */
