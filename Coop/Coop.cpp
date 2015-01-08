// Do not remove the include below
#include "Coop.h"


Timer timer;
Device mdevices[DEVICE_COUNT];


//The setup function is called once at startup of the sketch
void setup() {
	// Init Web
	setupWeb();

	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	mdevices[ARDUINO_IDX].begin( "Arduino"      ,  98, ARDUINO_IDX , (long)60*60*1000   , arduinoInit, arduinoCallbackT, arduinoHandler  );
	mdevices[DOOR_IDX].begin(    "Door 2345"    , 203, DOOR_IDX    , (long)300    , doorInit   , doorCallbackT   , doorHandler     );
	mdevices[DHT_IDX].begin(     "DHT22 6"      , 201, DHT_IDX     , (long)15*60*1000   , dhtInit    , dhtCallbackT    , dhtHandler      );
	mdevices[RELAY_0_IDX].setPin(RELAY_0_PIN);
	mdevices[RELAY_0_IDX].begin( "Fan A2"       , 204, RELAY_0_IDX , (long)0            , relayInit  , NULL            , relayHandler    );// Special Timer taking care off all simple devices
	mdevices[RELAY_1_IDX].setPin(RELAY_1_PIN);
	mdevices[RELAY_1_IDX].begin( "Light1 A4"    , 205, RELAY_1_IDX , (long)0            , relayInit  , NULL            , relayHandler    );
	mdevices[RELAY_2_IDX].setPin(RELAY_2_PIN);
	mdevices[RELAY_2_IDX].begin( "Light2 A5"    , 206, RELAY_2_IDX , (long)12*60*60*1000, relayInit  , relayCallbackT  , relayHandler    );
//	mdevices[NTC_0_IDX].setPin(NTC_0_PIN);
//	mdevices[NTC_0_IDX].begin(   "NTC1 A0"      , 208, NTC_0_IDX   , (long)15*60*1000   , ntcInit    , ntcCallbackT    , ntcHandler      );
	mdevices[DARK_IDX].setPin(PHOTO_RESISTOR_PIN);
	mdevices[DARK_IDX].begin(    "Dark A1"      , 209, DARK_IDX    , (long)5*60*1000    , darkInit   , darkCallbackT   , darkHandler     );
	mdevices[THERMO_IDX].begin(  "Heater A3"    , 202, THERMO_IDX  , (long)30*1000      , thermoInit , thermoCallbackT , thermoHandler   );
	relayCallbackT();

	showStatus(INFO_NORMAL, 0);

	// Use analog ref
	//analogReference(EXTERNAL);

}

// The loop function is called in an endless loop
void loop() {

	//if (DEBUG_MEMORY) printMem(" Main ");
	updateWeb();
	timer.update();

}

