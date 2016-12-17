#include "LowPower.h"

struct RGB {
	int red;
	int green;
	int blue;
};

RGB const ledmodule[2] = {
	{2, 3, 4},	//digital pins
	{5, 6, 7}	//digital pins
};

int const sensorPin=A6;  //Pin for Analog Output

void setup(){
	int i = 0;
	Serial.begin(9600);
	pinMode(ledmodule[0].blue, OUTPUT);  
	pinMode(ledmodule[0].green, OUTPUT);  
	pinMode(ledmodule[0].red, OUTPUT);
	digitalWrite(ledmodule[0].blue, LOW);		//Turn Led LOW
	digitalWrite(ledmodule[0].red, LOW);		//Turn Led LOW
	digitalWrite(ledmodule[0].green, LOW);	//Turn Led LOW
	Serial.println("Starting up...");

	for(i = 0; i<= 4; i++) {	//Startup led sequence
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

void SetStatus(int sensorvalue){
	
	int const SAFE = 100;
	int const WARNING = 600;
	int const DANGER = 600;
	
	if(sensorvalue >= SAFE && sensorvalue <= WARNING){  //Compare analog value with threshold 
		//ledmodule[0] = (RGB){LOW, LOW, HIGH};
		digitalWrite(ledmodule[0].green, LOW);  
		digitalWrite(ledmodule[0].red, LOW);  
		digitalWrite(ledmodule[0].blue, HIGH);  
	}  
	else if(sensorvalue > DANGER){ 
		//ledmodule[0] = (RGB){HIGH, LOW, LOW};
		digitalWrite(ledmodule[0].green, LOW);  
		digitalWrite(ledmodule[0].blue, LOW);  
		digitalWrite(ledmodule[0].red, HIGH);  
	} 
	else if(sensorvalue < SAFE){
		//ledmodule[0] = (RGB){LOW, HIGH, LOW};
		digitalWrite(ledmodule[0].blue, LOW);  
		digitalWrite(ledmodule[0].red, LOW);  
		digitalWrite(ledmodule[0].green, HIGH);  
	
	}
}

void TurnOffLeds(){
	Serial.println("Turning off LED module");
	//ledmodule[0] = (RGB){LOW, LOW, LOW};
	digitalWrite(ledmodule[0].blue, LOW);  
	digitalWrite(ledmodule[0].red, LOW);  
	digitalWrite(ledmodule[0].green, LOW);  
}


void loop(){
	int STATE = 0;
	int counter = 0;
	int avgRead = 0;
	int numReads = 5;
	int sensorvalue = 0;
	int const READ = 0;
	int const POWERSAVE = 1;

	switch (STATE){
		case READ:
			Serial.print("STATE=");
			Serial.println(STATE);
			for(int i = 1; i <= numReads; i++){
				sensorvalue = analogRead(sensorPin);  //Read the analog value
				Serial.print("Analog: ");  
				Serial.println(sensorvalue);  //Print the analog value
				avgRead = avgRead + sensorvalue; 
				delay(500); 
			}
			if (counter != 0) { //Don't read first avgRead since there's some current rush...
				avgRead = avgRead / numReads;	//Average value of the readings
				Serial.print("avgRead=");
				Serial.println(avgRead);
				SetStatus(avgRead);
			}
			delay(3000); //3 sec
			counter++;
			if(counter >= 3) STATE=POWERSAVE;	//Read is done. Go to sleep!
			break;
		case POWERSAVE:
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

}
