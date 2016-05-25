/*
Quick sketch for the Portlet with a MaxBotics Sonar sensor
to become a motion sensing nightlight

For this sketch I hooked the AN of the Maxbotic to A3 of the Adruino
(which is normally a switch).  Then 5V -> Vcc and GND -> GND

Sorry for the hack.  Didn't want to figure out the TX/RX of the Maxbotics
*/

// include the library code for testing the LCD:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 12, 7, 8, 9, 10);

// These constants won't change.  They're used to give names
// to the pins used:

//Buttons
const int S1 = 2;
const int S2 = 3;
const int S3 = A2;

//Analog input from the Maxbotics
const int anPin = A3;

//backlight
const int LED = 13;

//outlets
const int outlet_1 = 6;
const int outlet_2 = 5;

//input number
long input,disp;
int sum = 0;
int avgrange=60;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  //get the LED ready
  pinMode(13, OUTPUT);
  //intialize the outlets
  pinMode(outlet_1, OUTPUT);
  pinMode(outlet_2, OUTPUT);
  //print starting message to LCD
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Number is:  ");
}

void loop() {
  lcd.clear();

  input = analogRead(anPin);
  if (input < 20){
    lcd.setCursor(0,0);
    lcd.print("I see you!");
    lcd.setCursor(0,1);
    lcd.print("Lights on!");
    digitalWrite(LED, HIGH);
    digitalWrite(outlet_1, HIGH);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Searching...");
    lcd.setCursor(0,1);
    lcd.print("...lights off...");
    digitalWrite(LED, LOW);
    digitalWrite(outlet_1, LOW);
  }
  
  delay(500);                     
}
