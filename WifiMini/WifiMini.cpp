// Do not remove the include below
#include "WifiMini.h"

Timer timer;
Device mdevices[DEVICE_COUNT];
char temp[MAX_STRING_LEN];
SoftwareSerial dbg(2,3); // use pins 2, 3 for software Serial

///The setup function is called once at startup of the sketch
void setup() {
	wdt_reset(); 
	wdt_enable(WDTO_2S);
	
	if (DEBUG_MAIN || DEBUG_DEVICE_HAND || DEBUG_WEB || DEBUG_DEVICE || DEBUG_MEMORY) dbg.begin(DBG_BAUD_RATE);
	if (DEBUG_MEMORY) printMem("S ");

	// Init Web
	wdt_reset(); 
	setupWeb();

	wdt_reset(); 
	mdevices[ARDUINO_IDX].setPin(LED_PIN);
	mdevices[ARDUINO_IDX].setType(TYPE_ARDUINO);
	mdevices[ARDUINO_IDX].begin(       98, ARDUINO_IDX      );
	
	if (DEBUG_MEMORY) printMem("M0 ");

	wdt_reset(); 

	mdevices[TEMP_1_IDX].setPin(ONE_WIRE_BUS);
	mdevices[TEMP_1_IDX].setType(TYPE_TEMP_18B20);
	mdevices[TEMP_1_IDX].begin(       220,  TEMP_1_IDX );
	
	wdt_reset(); 
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
//		dbg.print("ETMR");
//		dbg.println(ARDUINO_IDX);
	}
	// TimerCount = 6
	if (timer.every((long)1*60*1000, checkTimer, CHECK_1_MIN) < 0) {		// 1 Min checks
		showStatus(TIMER_ERROR, ARDUINO_IDX);
//		dbg.print("ETMR");
//		dbg.println(ARDUINO_IDX);
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


