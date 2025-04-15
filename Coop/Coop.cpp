// Do not remove the include below
#include "Coop.h"

Timer timer;
Device mdevices[DEVICE_COUNT];
char temp[MAX_STRING_LEN];

///The setup function is called once at startup of the sketch
void setup() {
	wdt_reset(); 
	wdt_enable(WDTO_2S);
	
	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) Serial.begin(57600);
	if (DEBUG_MEMORY) printMem("S ");

	// Init Web
//	wdt_reset(); 
	setupWeb();

	wdt_reset(); 
	mdevices[ARDUINO_IDX].setPin(LED_PIN);
	mdevices[ARDUINO_IDX].setType(TYPE_ARDUINO);
	mdevices[ARDUINO_IDX].begin(       98, ARDUINO_IDX      );
	
	if (DEBUG_MEMORY) printMem("M0 ");

	wdt_reset(); 
	mdevices[RELAY_1_IDX].setPin(RELAY_1_PIN);
	mdevices[RELAY_1_IDX].setType(TYPE_DIGITAL_IO_NEG);
	mdevices[RELAY_1_IDX].begin(       205, RELAY_1_IDX );
	
	wdt_reset(); 
	mdevices[RELAY_2_IDX].setPin(RELAY_2_PIN);
	mdevices[RELAY_2_IDX].setType(TYPE_DIGITAL_IO);
	mdevices[RELAY_2_IDX].begin(       206, RELAY_2_IDX  );
	if (DEBUG_MEMORY) printMem("M1 ");

	wdt_reset(); 
	mdevices[DOOR_IDX].setType(TYPE_AUTO_DOOR);
	mdevices[DOOR_IDX].begin(         203, DOOR_IDX         );
	mdevices[DOOR_IDX].setCheckTimer((long)10         ,  doorTimer );
	if (DEBUG_MEMORY) printMem("M2 ");
	// TimerCount = 1

	// wdt_reset(); 
	// mdevices[NTC_0_IDX].setPin(NTC_0_PIN);
	// mdevices[NTC_0_IDX].setType(TYPE_NTC);
	// mdevices[NTC_0_IDX].begin(         204, NTC_0_IDX    );
	// if (DEBUG_MEMORY) printMem("M3 ");

	// wdt_reset(); 
	// mdevices[THERMO_IDX].setPin(RELAY_HEAT_PIN);
	// mdevices[THERMO_IDX].setInput(NTC_0_IDX);
	// mdevices[THERMO_IDX].setType(TYPE_THERMO_HEAT);
	// mdevices[THERMO_IDX].begin(        202, THERMO_IDX       );
	// if (DEBUG_MEMORY) printMem("M4 ");
	// TimerCount = 2

	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) {
		mdevices[AUTO_FAN_IDX].setPin(RELAY_1_PIN);
	} else {
		mdevices[AUTO_FAN_IDX].setPin(RELAY_FAN_PIN);
	}
	wdt_reset(); 
	mdevices[DHT_IDX].setPin(DHT_PIN);
	mdevices[DHT_IDX].setType(TYPE_DHT22);
	mdevices[DHT_IDX].begin(           201, DHT_IDX           );
	if (DEBUG_MEMORY) printMem("M5 ");

	wdt_reset(); 
	mdevices[AUTO_FAN_IDX].setInput(DHT_IDX);
	mdevices[AUTO_FAN_IDX].setType(TYPE_THERMO_COOL);
	mdevices[AUTO_FAN_IDX].begin(      208, AUTO_FAN_IDX       );
	if (DEBUG_MEMORY) printMem("M6 ");
	// TimerCount = 3 (could combine)

	// wdt_reset(); 
	// mdevices[WATER_LEVEL_IDX].setPin(PRESSURE_MPX4250GP);
	// mdevices[WATER_LEVEL_IDX].setType(TYPE_ANALOG_IN);
	// mdevices[WATER_LEVEL_IDX].begin(   210, WATER_LEVEL_IDX   );
	// if (DEBUG_MEMORY) printMem("M7 ");

	wdt_reset(); 
	mdevices[DARK_IDX].setPin(PHOTO_RESISTOR_PIN);
	mdevices[DARK_IDX].setType(TYPE_ANALOG_IN);
	mdevices[DARK_IDX].begin(          209, DARK_IDX    );
	if (DEBUG_MEMORY) printMem("M8 ");

	wdt_reset(); 
	mdevices[RELAY_3_IDX].setPin(RELAY_FAN_PIN);
	mdevices[RELAY_3_IDX].setType(TYPE_DIGITAL_IO);
	mdevices[RELAY_3_IDX].begin(       386, RELAY_3_IDX  );

	wdt_reset(); 
	if (timer.every((long)10*60*1000, reportTimer, REPORT_10MIN) < 0) {	// 10MIN reports
		showStatus(TIMER_ERROR, ARDUINO_IDX);
	}

	if (timer.every((long)60*60*1000 + (long)55*1000, reportTimer, REPORT_HOURLY) < 0) {	// Hourly reports
		showStatus(TIMER_ERROR, ARDUINO_IDX);
	}
	// TimerCount = 4

	if (timer.every((long)24*60*60*1000, reportTimer, REPORT_DAILY) < 0) {	// Daily reports
		showStatus(TIMER_ERROR, ARDUINO_IDX);
	}
	// TimerCount = 5
	if (timer.every((long)1*15*1000, checkTimer, CHECK_15_SEC) < 0) {		// 15 Sec checks
		showStatus(TIMER_ERROR, ARDUINO_IDX);
//		Serial.print("ETMR");
//		Serial.println(ARDUINO_IDX);
	}
	// TimerCount = 6
	if (timer.every((long)1*60*1000, checkTimer, CHECK_1_MIN) < 0) {		// 1 Min checks
		showStatus(TIMER_ERROR, ARDUINO_IDX);
//		Serial.print("ETMR");
//		Serial.println(ARDUINO_IDX);
	}
	// TimerCount = 7
	showStatus(INFO_NORMAL, 0);
}

/// The loop function is called in an endless loop
void loop() {
//	if (DEBUG_MEMORY) printMem(" Main ");
	wdt_reset(); 
	updateWeb();
	timer.update();
}


