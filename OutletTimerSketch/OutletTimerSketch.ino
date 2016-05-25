/*
 Brief Outlet Shutoff Sketch for the Portlet
 
 Demonstrates the use a Portlet as a safety shutoff device

 */

// include the library code for the LCD:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins for LCD control
LiquidCrystal lcd(11, 12, 7, 8, 9, 10);

// define the switches and outputs
const int button1 = 2;
const int button2 = 3;

//buttonThree and buttonFour are hooked to Analog
const int buttonThree = A2;
const int buttonFour = A3;

const int thermo1 = A0;
const int thermo2 = A1;

const int outlet1 = 6;
const int outlet2 = 5;

//the LCD backlight is used as feedback to the user
const int backlight = 13;


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  //define buttons as inputs and outlets as outputs
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  
  pinMode(outlet1, OUTPUT);
  pinMode(outlet2, OUTPUT);
  pinMode(backlight, OUTPUT);
  
  //enable serial for troubleshooting
  Serial.begin(9600);
  
}

void loop() {
  // prepare LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 1  ");
  lcd.setCursor(0, 1);
  lcd.print("to Start  ");
  
  //make sure both outlets are off
  digitalWrite(outlet1, LOW);
  digitalWrite(outlet2, LOW);
  
  //turn the backlight on for ease of reading
  digitalWrite(backlight,HIGH);
  
  //do nothing loop until a button is pressed
  while(digitalRead(button1) == LOW);
  
  //turn outlet 1 on
  digitalWrite(outlet1, HIGH);
  //start the timer
  int time_in_minutes = 6;
  for(int x = 0; x < time_in_minutes; x++){
    //print number of minutes elapsed
    lcd.clear();
    lcd.home();
    lcd.print("Num min:   ");
    lcd.print(x);
    lcd.print("    ");
    //pause for 1 minute
    delay(6000);
    //for fun, send the number of minutes elapsed over the serial
    Serial.println(x);
  }
 
}

