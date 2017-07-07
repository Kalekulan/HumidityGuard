#include "LowPower.h"	//Needs to be added separately. See submodule(s)

struct RGB {
	int red;
	int green;
	int blue;
};

boolean const DEBUG = false;
int const NUMBEROFMODULES = 2;	//Change this variable as to how many water sensors and led modules you have (you need one of each)

RGB const ledmodule[NUMBEROFMODULES] = {	//You also needs to add number of rows equal to NUMBEROFMODULES
	{2, 3, 4},	//digital pins - R , G, B
	{5, 6, 7}	//digital pins
};

int const sensorPins[NUMBEROFMODULES] = {A6, A7};  //You also needs to add number of indexes equal to NUMBEROFMODULES

int STATE = 0;

void setup(){
	int r = 0;
	if(DEBUG) Serial.begin(9600);
	//pinMode(ledmodule[0].red, OUTPUT);
	//pinMode(ledmodule[0].green, OUTPUT);
	//pinMode(ledmodule[0].blue, OUTPUT);
	for(r = 0; r < NUMBEROFMODULES; r++) {	//Setting pinmode=OUTPUT for leds
		pinMode(ledmodule[r].red, OUTPUT);
		pinMode(ledmodule[r].green, OUTPUT);
		pinMode(ledmodule[r].blue, OUTPUT);

		TurnOffLeds(&ledmodule[r]);
	}
		//TurnOffLeds(ledmodule[r].red, ledmodule[r].green, ledmodule[r].blue);
	if(DEBUG) Serial.println("Starting up...");


	for(r = 0; r <= 4; r++) {	//Startup blinking led sequence
		for(int i = 0; i < NUMBEROFMODULES; i++) {
			digitalWrite(ledmodule[i].blue, HIGH);
			delay(200);
			digitalWrite(ledmodule[i].blue, LOW);
			digitalWrite(ledmodule[i].red, HIGH);
			delay(200);
			digitalWrite(ledmodule[i].red, LOW);
			digitalWrite(ledmodule[i].green, HIGH);
			delay(200);
			digitalWrite(ledmodule[i].green, LOW);
		}
	}
}

void loop(){
	int const numReads = 5;
	int const numReadSets = 5;
	int waterLevel[NUMBEROFMODULES];
	int const READ = 0;
	int const POWERSAVE = 1;
	int moduleCounter = 0;
	int readCounter = 0;
	int rSetStatus;


	switch (STATE){
		case READ:	//Will stay in this loop for 60sec then go to POWERSAVE
			if(DEBUG) Serial.print("STATE=");
			if(DEBUG) Serial.println(STATE);

			for(readCounter = 0; readCounter < numReadSets; readCounter++){ 	//The number of readings
				for(moduleCounter = 0; moduleCounter < NUMBEROFMODULES; moduleCounter++) {	//Read every module
					waterLevel[moduleCounter] = WaterLevelRead(sensorPins[moduleCounter], numReads);
					if(DEBUG) Serial.print("waterLevel[");
					if(DEBUG) Serial.print(moduleCounter);
					if(DEBUG) Serial.print("]=");
					if(DEBUG) Serial.println(waterLevel[moduleCounter]);
					if(readCounter != 0) rSetStatus = SetStatus(&ledmodule[moduleCounter], waterLevel[moduleCounter]); //Skipping first read since there's some current rush going on...
					if(DEBUG) Serial.print("rSetStatus=");
					if(DEBUG) Serial.println(rSetStatus);
				}
				delay(3500); //3.5 secs
				//if(DEBUG) Serial.print("readCounter=");
				//if(DEBUG) Serial.println(readCounter);
			}


			STATE = POWERSAVE;	//Read is done. Go to sleep!
			break;
		case POWERSAVE:
			if(DEBUG) Serial.print("STATE=");
			if(DEBUG) Serial.println(STATE);
			for(int r = 0; r < NUMBEROFMODULES; r++) TurnOffLeds(&ledmodule[r]);
				//TurnOffLeds(ledmodule[r].red, ledmodule[r].green, ledmodule[r].blue);
			if(DEBUG) Serial.println("Powering down. Forever...");
			delay(1000);
			LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
			break;
		default:
		  // if nothing else matches, do the default
		  // default is optional
		break;
	}

}

int WaterLevelRead(int const sensorPin, int const numReads){

	int sensorvalue[numReads];
	int avgRead = 0;

	for(int i = 0; i < numReads; i++){
		sensorvalue[i] = analogRead(sensorPin);  //Read the analog value
		if(DEBUG) Serial.print("WaterLevelRead::sensorvalue[");
		if(DEBUG) Serial.print(i);
		if(DEBUG) Serial.print("]=");
		if(DEBUG) Serial.println(sensorvalue[i]);  //Print the analog value
		avgRead = avgRead + sensorvalue[i];
		delay(500);
	}

		avgRead = avgRead / numReads;	//Average value of the readings
		if(DEBUG) Serial.print("WaterLevelRead::avgRead=");
		if(DEBUG) Serial.println(avgRead);

	return avgRead;
}

int SetStatus(const struct RGB *leds, int value){

	int const SAFE = 200;		//SAFE is between 0 -> 100
	int const WARNING = 600;	//WARNING is between 101 -> 600
	int const DANGER = 1023;	//DANGER is between 601 -> 1023

	switch(value) {
		case 0 ...SAFE: 	//GREEN LED
			digitalWrite(leds->blue, LOW);
			digitalWrite(leds->red, LOW);
			digitalWrite(leds->green, HIGH);
			if(DEBUG) Serial.print("SetStatus::");
			if(DEBUG) Serial.println("GREEN");
			break;

		case SAFE+1 ...WARNING: 	//BLUE LED
			digitalWrite(leds->green, LOW);
			digitalWrite(leds->red, LOW);
			digitalWrite(leds->blue, HIGH);
			if(DEBUG) Serial.print("SetStatus::");
			if(DEBUG) Serial.println("BLUE");
			break;

		case WARNING+1 ...DANGER: 	//RED LED
			digitalWrite(leds->green, LOW);
			digitalWrite(leds->blue, LOW);
			digitalWrite(leds->red, HIGH);
			if(DEBUG) Serial.print("SetStatus::");
			if(DEBUG) Serial.println("RED");
			break;

		default:	//Value not in range
			digitalWrite(leds->green, LOW);
			digitalWrite(leds->blue, LOW);
			digitalWrite(leds->red, LOW);
			if(DEBUG) Serial.print("SetStatus::");
			if(DEBUG) Serial.println("COLORLESS");
			return false;
	}
	return true;
}

void TurnOffLeds(const struct RGB *leds){
	if(DEBUG) Serial.println("Turning off LED module");

	digitalWrite(leds->blue, LOW);
	digitalWrite(leds->red, LOW);
	digitalWrite(leds->green, LOW);

}
