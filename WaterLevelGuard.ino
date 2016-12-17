int sensorPin=A6;  //Pin for Analog Output
//enum led {
//  BLUE,
//  GREEN,
//  RED
//};
int BLUE = 2;
int GREEN = 3;
int RED = 4;
int sensorvalue=0;
int const sleep = 1000 * 60 * 5; //five minutes


void setup(){
  Serial.begin(9600);
  pinMode(BLUE, OUTPUT);  
  pinMode(GREEN, OUTPUT);  
  pinMode(RED, OUTPUT);
  digitalWrite(BLUE, LOW);  //Turn Led on
  digitalWrite(RED, LOW);  //Turn Led on
  digitalWrite(GREEN, LOW);  //Turn Led on
}

void loop(){
  sensorvalue = analogRead(sensorPin);  //Read the analog value
  Serial.print("Analog: ");  
  Serial.println(sensorvalue);  //Print the analog value

  if(sensorvalue >= 100 && sensorvalue <= 600){  //Compare analog value with threshold 
  	digitalWrite(GREEN, LOW);  //Turn Led on
	digitalWrite(RED, LOW);  //Turn Led on
    digitalWrite(BLUE, HIGH);  //Turn Led on
  }  
  else if(sensorvalue > 600){ 
    digitalWrite(GREEN, LOW);  //Turn Led on
	digitalWrite(BLUE, LOW);  //Turn Led on
	digitalWrite(RED, HIGH);  //Turn Led on
  } 
  else if(sensorvalue < 100){
	digitalWrite(BLUE, LOW);  //Turn Led on
	digitalWrite(RED, LOW);  //Turn Led on
    digitalWrite(GREEN, HIGH);  //Turn Led off
	
  }
  
  delay(sleep);
}
