#include "LowPower.h"

int sensorPin=A6;  //Pin for Analog Output
//enum led {
//  ledmodule0.blue,
//  ledmodule0.green,
//  ledmodule0.red
//};

struct RGB {
	int red;
	int green;
	int blue;
};


RGB const ledmodule0 = {2, 3, 4};

//int const ledmodule0.blue = 2;
//int const ledmodule0.green = 3;
//int const ledmodule0.red = 4;
int sensorvalue = 0;
int const sleep = 1000 * 60 * 5; //five minutes
int i = 0;
int STATE = 0;
int counter = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledmodule0.blue, OUTPUT);  
  pinMode(ledmodule0.green, OUTPUT);  
  pinMode(ledmodule0.red, OUTPUT);
  digitalWrite(ledmodule0.blue, LOW);  //Turn Led on
  digitalWrite(ledmodule0.red, LOW);  //Turn Led on
  digitalWrite(ledmodule0.green, LOW);  //Turn Led on
  Serial.println("Starting up...");
  for(i = 0; i<= 4; i++) { //Startup led sequence
	  digitalWrite(ledmodule0.blue, HIGH);
	  delay(200);
	  digitalWrite(ledmodule0.blue, LOW);
	  digitalWrite(ledmodule0.red, HIGH);
	  delay(200);
	  digitalWrite(ledmodule0.red, LOW);
	  digitalWrite(ledmodule0.green, HIGH);
	  delay(200);
	  digitalWrite(ledmodule0.green, LOW);	  
  }
}

void SetStatus(int sensorvalue) {
	
	int const SAFE = 100;
	int const WARNING = 600;
	int const DANGER = 600;
	
	if(sensorvalue >= SAFE && sensorvalue <= WARNING){  //Compare analog value with threshold 
		digitalWrite(ledmodule0.green, LOW);  //Turn Led on
		digitalWrite(ledmodule0.red, LOW);  //Turn Led on
		digitalWrite(ledmodule0.blue, HIGH);  //Turn Led on
	}  
	else if(sensorvalue > DANGER){ 
		digitalWrite(ledmodule0.green, LOW);  //Turn Led on
		digitalWrite(ledmodule0.blue, LOW);  //Turn Led on
		digitalWrite(ledmodule0.red, HIGH);  //Turn Led on
	} 
	else if(sensorvalue < SAFE){
		digitalWrite(ledmodule0.blue, LOW);  //Turn Led on
		digitalWrite(ledmodule0.red, LOW);  //Turn Led on
		digitalWrite(ledmodule0.green, HIGH);  //Turn Led off
	
	}
}

void TurnOffLeds() {
	Serial.println("Turning off LED module");
	digitalWrite(ledmodule0.blue, LOW);  //Turn Led on
	digitalWrite(ledmodule0.red, LOW);  //Turn Led on
	digitalWrite(ledmodule0.green, LOW);  //Turn Led off	
	
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
