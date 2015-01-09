// Do not remove the include below
#include "Coop.h"


Timer timer;
Device mdevices[DEVICE_COUNT];

//The setup function is called once at startup of the sketch
void setup() {
	// Init Web
	setupWeb();

	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	mdevices[ARDUINO_IDX].setPin(LED_PIN);
	mdevices[ARDUINO_IDX].setType(TYPE_ARDUINO);
	mdevices[ARDUINO_IDX].begin( "Arduino"      ,  98, ARDUINO_IDX , (long)60*60*1000   , arduinoCallbackT, deviceCommandHandler  );

	mdevices[THERMO_IDX].setPin(RELAY_HEAT_PIN);
	mdevices[THERMO_IDX].setInput(NTC_0_IDX);
	mdevices[THERMO_IDX].setType(TYPE_THERMO_HEAT);
	mdevices[THERMO_IDX].begin(  "Heat A0-A3"   , 202, THERMO_IDX  , (long)0            , NULL            , deviceCommandHandler   );

	mdevices[DOOR_IDX].setType(TYPE_AUTO_DOOR);
	mdevices[DOOR_IDX].begin(    "Door 2345"    , 203, DOOR_IDX    , (long)300          , doorCallbackT   , doorHandler            );

	mdevices[DHT_IDX].setPin(DHT_PIN);
	mdevices[DHT_IDX].setType(TYPE_DHT22);
	mdevices[DHT_IDX].begin(     "DHT22 6"      , 201, DHT_IDX     , (long)15*60*1000   , dhtCallbackT    , deviceCommandHandler   );

	mdevices[DARK_IDX].setPin(PHOTO_RESISTOR_PIN);
	mdevices[DARK_IDX].setType(TYPE_ANALOG_IN);
	mdevices[DARK_IDX].begin(     "Dark A1"     , 209, DARK_IDX    , (long)5*60*1000    , darkCallbackT   , deviceCommandHandler   );

	mdevices[RELAY_1_IDX].setPin(RELAY_1_PIN);
	mdevices[RELAY_1_IDX].setType(TYPE_DIGITAL_IO);
	mdevices[RELAY_1_IDX].begin( "Light1 A4"    , 205, RELAY_1_IDX , (long)0            , NULL            , deviceCommandHandler   );
	mdevices[RELAY_2_IDX].setPin(RELAY_2_PIN);
	mdevices[RELAY_2_IDX].setType(TYPE_DIGITAL_IO);
	mdevices[RELAY_2_IDX].begin( "Light2 A5"    , 206, RELAY_2_IDX , (long)12*60*60*1000, relayCallbackT  , deviceCommandHandler   );

	mdevices[AUTO_FAN_IDX].setPin(RELAY_0_PIN);
	mdevices[AUTO_FAN_IDX].setInput(DHT_IDX);
	mdevices[AUTO_FAN_IDX].setType(TYPE_THERMO_COOL);
	mdevices[AUTO_FAN_IDX].begin( "Cool 6-A2"   , 208, AUTO_FAN_IDX, (long)1*5*1000     , thermoCallbackT , deviceCommandHandler   );

	mdevices[NTC_0_IDX].setPin(NTC_0_PIN);
	mdevices[NTC_0_IDX].setType(TYPE_ANALOG_IN);
	mdevices[NTC_0_IDX].begin(    "NTC A0"      , 204, NTC_0_IDX   , (long)0            , NULL            , deviceCommandHandler   );
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

