/*
 Brief Yoghurt Program for the Portlet
 
 Demonstrates the use a Portlet for making Yoghurt.  
 
 The total process is divided into 5 basic steps for programming:
 
 1.  Heat the milk to 185 degrees F
 2.  Pastuerize the milk (185 degrees F for 30 minutes)
 3.  Cool the milk down to 110 degrees F
 4.  Pitch the yoghurt culture
 5.  Hold the yoghurt culture at 100 degrees F for 7 hours
 
 After this the yoghurt is ready to put into containers and refrigerate.
 
 Reference and yoghurt making instructions at:
 
 www.makeyourownyogurt.com
 
 It is expected that Outlet 1 and Thermo 1 are being used
 
  The LCD circuit:
 * LCD RS pin to digital pin 11
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 7
 * LCD D5 pin to digital pin 8
 * LCD D6 pin to digital pin 9
 * LCD D7 pin to digital pin 10
 * LCD R/W pin to ground

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
  
  //enable serial for troubleshooting
  Serial.begin(9600);
  
}


int convert_degrees(int analog_value){
  int temperature = 0;
  
  //NOTE: IF YOU WANT TO FINE TUNE YOUR TEMPERATURE READOUTS
  //THIS IS THE PLACE TO TO DO IT!
  int offset = 0;
  
  //the circuit changes 10mV/degree Celsius (hopefully)
  //Analog inputs measure 0 to 5V
  //0 to 5V is a range of 0 to 500 degrees Celsius
  
  //UNCOMMENT THIS LINE FOR CELSIUS
  //temperature = map(analog_value, 0, 1023, 0, 500) + offset;
  
  //0 to 500 degrees Celsius is 32 to 932 degrees Farenheit
  //UNCOMMENT THIS LINE FOR FARENHEIT  
  temperature = map(analog_value, 0, 1023, 32, 932) + offset;

  return temperature;
}

//turn the outlet on until target temperature is reached
void heat_up(int target_temp){
  //while temp is less than target, keep outlet on
  while(target_temp > convert_degrees(analogRead(thermo1))){
    digitalWrite(outlet1, HIGH);
      //reset LCD and print current operation
    lcd.clear();
    lcd.home();
    lcd.print("Heating to ");
    lcd.print(target_temp);
    //set cursor for printing temp
    lcd.setCursor(0,1);
    //print current temperature
    lcd.print("Temp:");
    //clear the old temp from the screen
    lcd.setCursor(8, 1);
    lcd.print("     ");
    lcd.setCursor(8, 1);
    //print current temp
    lcd.print(convert_degrees(analogRead(thermo1)));
    //pause for a bit so the screen doesn't flicker
    delay(200);
  }
  //turn outlet off before exiting
  digitalWrite(outlet1, LOW);
}

//turn the outlet off until it cools to target temperature
void cool_down(int target_temp){
  //make sure outlet is off
  digitalWrite(outlet1, LOW);
  //wait for temperature to drop below target
  while(target_temp < convert_degrees(analogRead(thermo1))){
      //clear lcd and print current operation
    lcd.clear();
    lcd.home();
    lcd.print("Cooling to: ");
    lcd.print(target_temp);
    lcd.setCursor(0, 1);
    //print current temperature
    lcd.print("Temp:");
    //clear the old temp from the screen
    lcd.setCursor(8, 1);
    lcd.print("     ");
    lcd.setCursor(8, 1);
    //print current temp
    lcd.print(convert_degrees(analogRead(thermo1)));
    //pause for a bit so the screen doesn't flicker
    delay(200);
  }
}

//signalling the user to pitch the yoghurt bacteria
void pitch_yoghurt(){
  //clear lcd and print current operation
  lcd.clear();
  lcd.home();
  lcd.print("Pitch Yoghurt");
  lcd.setCursor(0, 1);
  lcd.print("1 to continue");
  //flash the backlight to get attention
  int time_elapsed = millis();
  while(digitalRead(button1) == LOW){
    if ((millis()-time_elapsed) > 1000){
      digitalWrite(backlight, HIGH);
    }
    if ((millis()-time_elapsed) > 2000){
      digitalWrite(backlight, LOW);
      time_elapsed = millis();
    }
  }
}
 
//routine to maintain the temperature at specified temp and for duration time
void maintain_temp(int target_temp, int time_in_min){
  //only check the temperature once per minute
  int num_loops = time_in_min;
  for(int x = 0; x < num_loops; x++){
    if (convert_degrees(analogRead(thermo1)) < target_temp){
      digitalWrite(outlet1, HIGH);
    }
    else {
      digitalWrite(outlet1, LOW);
    }
    //clear lcd and print current operation
    lcd.clear();
    lcd.home();
    lcd.print("Maintain temp");
    lcd.setCursor(0, 1);
    //display number of minutes
    lcd.print("Num min:  ");
    lcd.print(x);
    lcd.print("   ");
    //pause 1 minute each cycle
    delay(60000);
  }
  //turn outlet off before exiting
  digitalWrite(outlet1, LOW);
}



void loop() {
  // prepare LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 1  ");
  lcd.setCursor(0, 1);
  lcd.print("to Start  ");
  
  //do nothing loop until a button is pressed
  while(digitalRead(button1) == LOW);
  
  //heat milk to 185 degrees
  heat_up(185);
  //pastuerize milk for 30 minutes
  maintain_temp(185, 30);
  //cool the milk down to 100 degrees
  cool_down(100);
  //request that the user pitch the culture
  pitch_yoghurt();
  //culture the bacteria at 100 degrees for 7 hours
  maintain_temp(100, 420);
  
  //Signal success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALL DONE!");
  lcd.setCursor(0, 1);
  lcd.print("Refrigerate");
  
  //either press button1 or reset to do it again
  while(digitalRead(button1) == LOW);  
}

