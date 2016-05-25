/*
Quick sketch for testing the features of the Portlet
 
 by Mech Tech Labs
 
www.mechtechlab.com

*/

// include the library code for testing the LCD:
#include <LiquidCrystal.h>

// handy globals for the various pins
//Buttons
const int S1 = 2;
const int S2 = 3;
const int S3 = A2;
const int S4 = A3;

//ints to be a digital threshold for using analogs for buttons
const int S3_limit = 500;
const int S4_limit = 500;

// outlet controlling pins
const int outlet_1 = 6;
const int outlet_2 = 5;

// LED backlight
const int LED = 13;

// thermocouple inputs
const int A = A0;
const int B = A1;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 12, 7, 8, 9, 10);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // intialize the serial port for extra communication
  Serial.begin(9600);
  // Print a message to the LCD.
  lcd.print("Begin Testing!");
  Serial.println("Begin Testing!");
  
  //set up the switches
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  // remember S3 and S4 are put on analog inputs
  
  //outlet control
  pinMode(outlet_1, OUTPUT);
  pinMode(outlet_2, OUTPUT);
  
}

void test_buttons(){
    // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);

  // print the first step
  lcd.print("Press S1");
  Serial.println("Press S1");
  // wait for S1 to be pressed
  while(digitalRead(S1) == LOW);
  // display success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1 works!");
  Serial.println("S1 works!");
  
  //prepare for the next step
  lcd.setCursor(0, 1);
  lcd.print("Press S2");
  Serial.println("Press S2");
  // wait for S2 to be pressed
  while(digitalRead(S2) == LOW);
  // display success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S2 works!");
  Serial.println("S2 works!");
  
  //prepare for the next step
  lcd.setCursor(0, 1);
  lcd.print("Press S3");
  Serial.println("Press S3");
  //wait for S3 to be pressed
  while(analogRead(S3) < S3_limit);
  //display success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S3 works!");
  Serial.println("S3 works!");

  //prepare for the next step
  lcd.setCursor(0, 1);
  lcd.print("Press S4");
  Serial.println("Press S4");
  // wait for S2 to be pressed
  while(analogRead(S4) < S4_limit);
  // display success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S4 works!");
  Serial.println("S4 works!");
  
  // and advance to the next step
  lcd.setCursor(0, 1);
  lcd.print("S1 to move on...");
  Serial.println("S1 to move on...");
  while(digitalRead(S1) == LOW);
}

void test_outlets(){
  // prepare display
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // start by toggling outlet_1
  lcd.print("To test outlet 1");
  Serial.println("To test outlet 1");
  lcd.setCursor(0, 1);
  lcd.print("press S4");
  Serial.println("press S4");
  //wait for S4 to be pressed
  while(analogRead(S4) < S4_limit);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1 to hear click");
  Serial.println("S1 to hear click");
  lcd.setCursor(0, 1);
  lcd.print("S2 to go on...");
  Serial.println("S2 to go on...");
  // the relay will make an audible clicking sound everytime you hit S1
  // if you hear the clicking, press S2 to continue
  while(digitalRead(S2) == LOW){
    if (digitalRead(S1) == HIGH){
      digitalWrite(outlet_1, HIGH);
      delay(500);
      digitalWrite(outlet_1, LOW);
    }
  }
  
  // Then toggle outlet_2
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("To test outlet 2");
  Serial.println("To test outlet 2");
  lcd.setCursor(0, 1);
  lcd.print("press S4");
  Serial.println("press S4");
  //wait for S1 to be pressed
  while(analogRead(S4) < S4_limit);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1 to hear click");
  Serial.println("S1 to hear click");
  lcd.setCursor(0, 1);
  lcd.print("S2 to go on...");
  Serial.println("S2 to go on...");
  // the relay will make an audible clicking sound everytime you hit S1
  // if you hear the clicking, press S2 to continue
  while(digitalRead(S2) == LOW){
    if (digitalRead(S1) == HIGH){
      digitalWrite(outlet_2, HIGH);
      delay(500);
      digitalWrite(outlet_2, LOW);
    }
  }
}

void test_backlight(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test backlight");
  Serial.println("Test backlight");
  lcd.setCursor(0, 1);
  lcd.print("S4 to go on...");
  Serial.println("S4 to go on...");
  // wait for S4
  while(analogRead(S4) < S4_limit);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1 to flicker");
  Serial.println("S1 to flicker");
  lcd.setCursor(0, 1);
  lcd.print("S2 to go on...");
  Serial.println("S2 to go on...");
  
  //pressing S1 turns the backlight on
  //pressing S2 goes to the next step
  while(digitalRead(S2) == LOW){
    if(digitalRead(S1) == HIGH){
      digitalWrite(LED, HIGH);
    }
    else{
      digitalWrite(LED, LOW);
    }
  }
}
  
int convert_degrees(int channel){
  int sensorvalue = 0;
  int temperature = 0;
  
  //NOTE: IF YOU WANT TO FINE TUNE YOUR TEMPERATURE READOUTS
  //THIS IS THE PLACE TO TO DO IT!
  
  int offset = 0;
  
  //read the appropriate sensor
  sensorvalue = analogRead(channel);
  
  //the circuit changes 10mV/degree Celsius (hopefully)
  //Analog inputs measure 0 to 5V
  //0 to 5V is a range of 0 to 500 degrees Celsius
  
  //UNCOMMENT THIS LINE FOR CELSIUS
  //temperature = map(analog_value, 0, 1023, 0, 500) + offset;
  
  //0 to 500 degrees Celsius is 32 to 932 degrees Farenheit
  //UNCOMMENT THIS LINE FOR FARENHEIT
  temperature = map(sensorvalue, 0, 1023, 32, 932) + offset;
  
  return temperature;
}

void test_thermo(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing Thermo!");
  Serial.println("Testing Thermo!");
  lcd.setCursor(0, 1);
  lcd.print("S4 to go on...");
  Serial.println("S4 to go on...");
  //wait for S4
  while(analogRead(S4) < S4_limit);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thermo 1:  ");
  Serial.println("Thermo 1:");
  lcd.setCursor(0, 1);
  lcd.print("S1 to go on...");
  Serial.println("S1 to go on...");
  //display Thermo 1 values
  //S1 to continue
  while(digitalRead(S1) == LOW){
    //erasing the old number ...
    lcd.setCursor(11, 0);
    lcd.print("    ");
    //print the new number
    lcd.setCursor(11, 0);
    lcd.print(convert_degrees(A));
    Serial.println(convert_degrees(A));
    //this delay cuts down on flickering
    delay(100);
  }
  //make sure you've released S1 before continuing
  while(digitalRead(S1) == HIGH);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thermo 2:  ");
  Serial.println("Thermo 2:");
  lcd.setCursor(0, 1);
  lcd.print("S1 to go on...");
  Serial.println("S1 to go on...");
  //display Thermo 1 values
  //S1 to continue
  while(digitalRead(S1) == LOW){
    //erasing the old number ...
    lcd.setCursor(11, 0);
    lcd.print("    ");
    //print the new number
    lcd.setCursor(11, 0);
    lcd.print(convert_degrees(B));
    Serial.println(convert_degrees(B));
    //this delay cuts down on flickering
    delay(100);
  }
}

void test_reset(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press Reset to");
  Serial.println("Press Reset to");
  lcd.setCursor(0, 1);
  lcd.print("do it again!");
  Serial.println("do it again!");
  // infinite while loop 
  while(1);
}

void loop() {
    test_buttons();
    test_outlets();
    test_backlight();
    test_thermo();
    test_reset();
}
