#include <iostream>
#include <fstream>
#include <iomanip>
#include <nmeaparse/nmea.h>

using namespace std;
using namespace nmea;


int main(int argc, char** argv){

    /*
     * To compute checksums of your own sentences:
     * https://nmeachecksum.eqth.net/
     */

    /*
     * 3 examples of iXblue sentences: AIPOV ($AIPOV), TECHSAS ($PASHR) and IXSEA_TAH ($PHOCT)
     * Uncomment the one you want to test
     * (and uncomment )
     * https://nmeachecksum.eqth.net/
     */

    string sentence = "$AIPOV,163405.123456,180.000,+120.000,+90.200,+400.000,+300.000,+200.000,+110.00,+100.00,+90.00,+45.00000000,-45.00000000,200.000,+200.000,+150.000,+100.000,+75.000,+50.000,+25.000,180.000,1234ABCD*69";
    //string sentence = "$PASHR,163405.123,180.00,T,+120.00,+60.200,500.00,+100.00,+50.00,240,0,0*32";
    //string sentence = "$PHOCT,01,163405.123,T,03,180.000,T,+120.000,T,-45.000,T,+90.000,T,+85.000,+75.000,+65.000,-85.000,-75.000,-65.000,+1250.00*0B";

    // Create a INS service that will keep track of the fix data
    NMEAParser parser;
    INSService ins(parser);
    parser.log = false;

    try {
        parser.readLine(sentence);
    }
    catch (NMEAParseError& e){
        cout << e.message << endl;
        // You can keep feeding data to the INS service...
        // The previous data is ignored and the parser is reset
    }


    // Show the final fix information
    cout << ins.fix.toString_AIPOV() << endl;
    //cout << ins.fix.toString_TECHSAS() << endl;
    //cout << ins.fix.toString_IXSEA_TAH() << endl;

    return 0;
    
}