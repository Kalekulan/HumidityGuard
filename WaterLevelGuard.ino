#include "LowPower.h"	//Needs to be added separately. See submodule(s)



struct RGB {
	int red;
	int green;
	int blue;
};

int const NUMBEROFMODULES = 2;	//Change this variable as to how many water sensors and led modules you have (you need one of each)

RGB const ledmodule[NUMBEROFMODULES] = {	//You also needs to add number of rows equal to NUMBEROFMODULES
	{2, 3, 4},	//digital pins
	{5, 6, 7}	//digital pins
};

int const sensorPin=A6;  //Pin for Analog Output

void setup(){
	
	Serial.begin(9600);
	pinMode(ledmodule[0].red, OUTPUT);
	pinMode(ledmodule[0].green, OUTPUT); 
	pinMode(ledmodule[0].blue, OUTPUT);  
	for(int r=0; r < NUMBEROFMODULES; r++) TurnOffLeds(&ledmodule[r]);
		//TurnOffLeds(ledmodule[r].red, ledmodule[r].green, ledmodule[r].blue);
	Serial.println("Starting up...");

	for(int i = 0; i<= 4; i++) {	//Startup led sequence
		digitalWrite(ledmodule[0].blue, HIGH);
		delay(200);
		digitalWrite(ledmodule[0].blue, LOW);
		digitalWrite(ledmodule[0].red, HIGH);
		delay(200);
		digitalWrite(ledmodule[0].red, LOW);
		digitalWrite(ledmodule[0].green, HIGH);
		delay(200);
		digitalWrite(ledmodule[0].green, LOW);	  
	}
}

void loop(){

	int const numReads = 5;
	int const numReadSets = 10;
	int waterLevel[NUMBEROFMODULES];
	int const READ = 0;
	int const POWERSAVE = 1;
	int STATE = 0;
	int moduleCounter = 0;
	int readCounter = 0;
	boolean rSetStatus;
	
	bajs(&ledmodule[0]);
	//TurnOffLeds(ledmodule[0]);
	
	switch (STATE){
		case READ:	//Will stay in this loop for 60sec then go to POWERSAVE
			Serial.print("STATE=");
			Serial.println(STATE);
			
			for(readCounter = 0; readCounter < numReadSets; readCounter++){
				for(moduleCounter = 0; moduleCounter < NUMBEROFMODULES; moduleCounter++) {
					waterLevel[moduleCounter] = WaterLevelRead(numReads);
					Serial.print("waterLevel[");
					Serial.print(moduleCounter);
					Serial.print("]=");
					Serial.println(waterLevel[moduleCounter]);
					if(readCounter != 0) rSetStatus = SetStatus(&ledmodule[moduleCounter], waterLevel[moduleCounter]); //Skipping first read since there's some current rush going on...
					Serial.print("rSetStatus=");
					Serial.println(rSetStatus);
				}
				delay(3500); //3.5 secs
				//Serial.print("readCounter=");
				//Serial.println(readCounter);
			}
			

			STATE=POWERSAVE;	//Read is done. Go to sleep!
			break;
		case POWERSAVE:
			Serial.print("STATE=");
			Serial.println(STATE);
			for(int r=0; r < NUMBEROFMODULES; r++) TurnOffLeds(&ledmodule[r]);
				//TurnOffLeds(ledmodule[r].red, ledmodule[r].green, ledmodule[r].blue);
			Serial.println("Powering down. Forever...");
			delay(1000);
			LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
			break;
		default:
		  // if nothing else matches, do the default
		  // default is optional
		break;
	}

}

int WaterLevelRead(int const numReads){
	
	int sensorvalue[numReads];
	int avgRead = 0;
	
	for(int i = 0; i < numReads; i++){
		sensorvalue[i] = analogRead(sensorPin);  //Read the analog value
		Serial.print("WaterLevelRead::sensorvalue[");
		Serial.print(i);
		Serial.print("]=");
		Serial.println(sensorvalue[i]);  //Print the analog value
		avgRead = avgRead + sensorvalue[i]; 
		delay(500); 
	}

		avgRead = avgRead / numReads;	//Average value of the readings
		Serial.print("WaterLevelRead::avgRead=");
		Serial.println(avgRead);

	return avgRead;
}

void bajs(const struct RGB *x) {}


int SetStatus(const struct RGB *leds, int value){
	
	int const SAFE = 100;		//SAFE is between 0 -> 100
	int const WARNING = 600;	//WARNING is between 101 -> 600
	int const DANGER = 1023;	//DANGER is between 601 -> 1023
	
	switch(value) {	
		case 0 ...SAFE: 	//GREEN LED
			digitalWrite(leds->blue, LOW);  
			digitalWrite(leds->red, LOW);  
			digitalWrite(leds->green, HIGH);  
			Serial.print("SetStatus::");
			Serial.println("GREEN");
			break;
		
		case SAFE+1 ...WARNING: 	//BLUE LED
			digitalWrite(leds->green, LOW);  
			digitalWrite(leds->red, LOW);  
			digitalWrite(leds->blue, HIGH);  
			Serial.print("SetStatus::");
			Serial.println("BLUE");
			break;
			
		case WARNING+1 ...DANGER: 	//RED LED
			digitalWrite(leds->green, LOW);  
			digitalWrite(leds->blue, LOW);  
			digitalWrite(leds->red, HIGH);  		
			Serial.print("SetStatus::");
			Serial.println("RED");
			break;
			
		default:	//Value not in range
			digitalWrite(leds->green, LOW);  
			digitalWrite(leds->blue, LOW); 
			digitalWrite(leds->red, LOW); 	
			Serial.print("SetStatus::");
			Serial.println("COLORLESS");			
			return false;
	}
/* 	if(value >= SAFE && value <= WARNING){  //Compare analog value with threshold 
		//ledmodule[0] = (RGB){LOW, LOW, HIGH};
		digitalWrite(ledmodule[0].green, LOW);  
		digitalWrite(ledmodule[0].red, LOW);  
		digitalWrite(ledmodule[0].blue, HIGH);  

		Serial.print("SetStatus::");
	}  
	else if(value > DANGER){ 
		//ledmodule[0] = (RGB){HIGH, LOW, LOW};
		digitalWrite(ledmodule[0].green, LOW);  
		digitalWrite(ledmodule[0].blue, LOW);  
		digitalWrite(ledmodule[0].red, HIGH);  
	} 
	else if(value < SAFE){
		//ledmodule[0] = (RGB){LOW, HIGH, LOW};
		digitalWrite(ledmodule[0].blue, LOW);  
		digitalWrite(ledmodule[0].red, LOW);  
		digitalWrite(ledmodule[0].green, HIGH);  
	
	}
	else{
		Serial.print("SetStatus::value=");
		Serial.println(value);
		return false;
	} */
	return true;
}

void TurnOffLeds(const struct RGB *leds){
	Serial.println("Turning off LED module");

	digitalWrite(leds->blue, LOW);  
	digitalWrite(leds->red, LOW);  
	digitalWrite(leds->green, LOW);   

}

/*
void TurnOffLeds(int red, int green, int blue){
	Serial.println("Turning off LED module");

/* 	digitalWrite(ledmodule[0].blue, LOW);  
	digitalWrite(ledmodule[0].red, LOW);  
	digitalWrite(ledmodule[0].green, LOW);   
	digitalWrite(red, LOW); 
	digitalWrite(green, LOW); 
	digitalWrite(blue, LOW);  
}
 */