/*
Quick sketch for testing the analog output of the Portlet

It receives the thermocouple analog signals, performs a quick
conversion to degrees Farenheit and dumps it to the serial port
 
 by Mech Tech Labs
 
www.mechtechlab.com

*/

// These constants won't change.  They're used to give names
// to the pins used:
const int Thermo1 = A0;  // Analog input pin that the potentiometer is attached to
const int Thermo2 = A1; 

//Buttons
const int S1 = 2;
const int S2 = 3;
const int S3 = A2;
const int S4 = A3;

int sensor1Value = 0;        
int sensor2Value = 0;      
int converted1Value = 0;
int converted2Value = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
  sensor1Value = analogRead(Thermo1);
  sensor2Value = analogRead(Thermo2);  
  
  converted1Value = map(sensor1Value, 0, 1023, 32, 932);
  converted2Value = map(sensor2Value, 0, 1023, 32, 932);
         

  // print the analog results to the serial monitor:
  Serial.print("sensor 1 =  " );                       
  Serial.print(converted1Value);      
  Serial.print("\t sensor 2 = ");      
  Serial.println(converted2Value);   
  
  // print the button results to the serial monitor:
  //Serial.print("b1= ");
  //Serial.print(digitalRead(S1));
  //Serial.print("\t b2= ");
  //Serial.print(digitalRead(S2));
  //Serial.print("\t b3= ");
  //Serial.print(analogRead(S3));
  //Serial.print("\t b4= ");
  //Serial.println(analogRead(S4));
  
  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(100);                     
}
