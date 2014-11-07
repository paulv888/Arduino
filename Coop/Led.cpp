/*
 * Led.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: pvloon
 */
#include <Led.h>

void showStatus(byte Error) {
	static byte eSensorSend = 0;
	switch (Error) {
	case INFO_NORMAL:
		timer.oscillate(LED_PIN, HIGH, 500, 2, 3);
		break;
	case INFO_CALIBRATING:
		timer.oscillate(LED_PIN, HIGH, 500, 50, 3);
		break;
	case ERROR_READ_SENSOR: {
		timer.oscillate(LED_PIN, HIGH, 100, 50, 10);
		//Serial.println("SENSOR ERROR!");
		if (eSensorSend++ % 400 == 0) {														// do not flood (about every hour)
/*			payLoad_Simple scommand;
			scommand.command = COMMAND_RESULT_ERROR;
			scommand.data = ERROR_READ_SENSOR;*/
//			sendMessageRF(RF_BASE_STATION, scommand);
			eSensorSend=true;
		}
	}
		break;
	case ERROR_CALIBRATE: {
		timer.oscillate(LED_PIN, HIGH, 500, 50, 10);
		//Serial.println("CALIBRATE ERROR!");
/*		payLoad_Simple scommand;
		scommand.command = COMMAND_RESULT_ERROR;
		scommand.data = ERROR_CALIBRATE;*/
//		sendMessageRF(RF_BASE_STATION, scommand);
	}
		break;
	}
}

