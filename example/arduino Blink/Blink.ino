#include "EventDriven.h"
#include "TimerEv.h"

// define your events
#define ADC_START      1
#define ADC_END        2
#define LED            3
#define DEBUG_PRINT    4


EventDriven eventloop;
TimerEv timerev(eventloop);

boolean led = 0;
int data = -1;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);

	// initialize digital pin 13 as an output.
	pinMode(13, OUTPUT);

	// sensor trigger pin
	pinMode(10, OUTPUT);

	timerev.add(LED, 62.5*1000, 1, 1);  // add repeat LED timer (62.5 ms)
	timerev.add(ADC_START, 50000, 1, 1);  // add repeat ADC timer (50 ms)
	timerev.add(DEBUG_PRINT, 500000, 1, 1);  // add repeat DEBUG_PRINT timer (500 ms)
}

void loop() {
	timerev.tick();
	switch(eventloop.get()){

		case ADC_START:
			digitalWrite(10, HIGH);   // trigger the sensor
			digitalWrite(10, LOW);
			timerev.add(ADC_END, 10*1000, 0, 1); // emit ADC_END 10 ms later (no need for waitting)
		break;

		case ADC_END:
			// get data & do somthing
			data = analogRead(A0);
			// ...
			delay(10);  // blocking code
			// ...
		break;

		// print every 500 ms
		case DEBUG_PRINT:
			Serial.println(data);
		break;

		//LED do every 62.5 ms
		case LED:
			digitalWrite(13, led);
			led = !led;
		break;
	}
}

uint32_t us_ticker_read(){
	return micros();
}

