#include "LowPower.h"

int sensorPin=A6;  //Pin for Analog Output
//enum led {
//  BLUE,
//  GREEN,
//  RED
//};
int const BLUE = 2;
int const GREEN = 3;
int const RED = 4;
int sensorvalue=0;
int const sleep = 1000 * 60 * 5; //five minutes
int i = 0;
int STATE = 0;
int counter = 0;

void setup(){
  Serial.begin(9600);
  pinMode(BLUE, OUTPUT);  
  pinMode(GREEN, OUTPUT);  
  pinMode(RED, OUTPUT);
  digitalWrite(BLUE, LOW);  //Turn Led on
  digitalWrite(RED, LOW);  //Turn Led on
  digitalWrite(GREEN, LOW);  //Turn Led on
  Serial.println("Starting up...");
  for(i = 0; i<= 4; i++) { //Startup led sequence
	  digitalWrite(BLUE, HIGH);
	  delay(200);
	  digitalWrite(BLUE, LOW);
	  digitalWrite(RED, HIGH);
	  delay(200);
	  digitalWrite(RED, LOW);
	  digitalWrite(GREEN, HIGH);
	  delay(200);
	  digitalWrite(GREEN, LOW);	  
  }
}

void SetStatus(int sensorvalue) {
	
	int const SAFE = 100;
	int const WARNING = 600;
	int const DANGER = 600;
	
	if(sensorvalue >= SAFE && sensorvalue <= WARNING){  //Compare analog value with threshold 
		digitalWrite(GREEN, LOW);  //Turn Led on
		digitalWrite(RED, LOW);  //Turn Led on
		digitalWrite(BLUE, HIGH);  //Turn Led on
	}  
	else if(sensorvalue > DANGER){ 
		digitalWrite(GREEN, LOW);  //Turn Led on
		digitalWrite(BLUE, LOW);  //Turn Led on
		digitalWrite(RED, HIGH);  //Turn Led on
	} 
	else if(sensorvalue < SAFE){
		digitalWrite(BLUE, LOW);  //Turn Led on
		digitalWrite(RED, LOW);  //Turn Led on
		digitalWrite(GREEN, HIGH);  //Turn Led off
	
	}
}

void TurnOffLeds() {
	Serial.println("Turning off LED module");
	digitalWrite(BLUE, LOW);  //Turn Led on
	digitalWrite(RED, LOW);  //Turn Led on
	digitalWrite(GREEN, LOW);  //Turn Led off	
	
}


void loop(){

int avgRead = 0;
int numReads = 5;


	switch (STATE) {
		case 0:
			Serial.print("STATE=");
			Serial.println(STATE);
			for(int i=1; i<=numReads; i++){
				sensorvalue = analogRead(sensorPin);  //Read the analog value
				Serial.print("Analog: ");  
				Serial.println(sensorvalue);  //Print the analog value
				avgRead = avgRead + sensorvalue;
				delay(500);
			}
			if (counter != 0) {
				avgRead = avgRead / numReads;
				Serial.print("avgRead=");
				Serial.println(avgRead);
				SetStatus(avgRead);
			}
			delay(3000); //3 sec
			counter++;
			if(counter >= 3) STATE=1;
			break;
		case 1:
			Serial.print("STATE=");
			Serial.println(STATE);
			TurnOffLeds();
			Serial.println("Powering down. Forever...");
			delay(1000);
			LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
			break;
		default:
		  // if nothing else matches, do the default
		  // default is optional
		break;
	}
  
  
  
	//delay(sleep);
}
