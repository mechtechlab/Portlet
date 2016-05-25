/*
 Very basic reflow oven sketch for the Portlet
 
 Jon Thorn 1-17-2015
 
 It attempts to follow a basic leaded solder temperature profile
from the spec sheet for ChipQuick SMD291AX:

http://media.digikey.com/pdf/Data%20Sheets/Chip%20Quik%20PDFs/SMD291AX.pdf
 
 The total process is divided into 4 basic steps for programming:
 
 1.  Heat to 100°C as quickly as possible
 2.  Go from 100°C to 170°C over a time of 90 seconds
 (we're going to cheat here by just holding at 140° for 30 seconds)
 3.  Get the temperature above 190 °C for 30 seconds (but below 215°C)
 4.  Start flashing to indicate the door should be opened for cooling

When the board has cooled down it is hopefully done
 
 
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
  temperature = map(analog_value, 0, 1023, 0, 500) + offset;
  
  //0 to 500 degrees Celsius is 32 to 932 degrees Farenheit
  //UNCOMMENT THIS LINE FOR FARENHEIT  
  //temperature = map(analog_value, 0, 1023, 32, 932) + offset;

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

//signalling the user to open the door for cooling
void open_door(){
  //clear lcd and print current operation
  lcd.clear();
  lcd.home();
  lcd.print("Open door for cool");
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
void maintain_temp(int target_temp, int time_in_sec){
  //only check the temperature once per second
  int num_loops = time_in_sec;
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
    delay(1000);
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
  
  //heat up to 100°C
  heat_up(100);
  //heat up to 140°C
  heat_up(140);
  //hold at 140 for about 30 seconds...
  maintain_temp(140, 30);
  //continue up to 170°C
  heat_up(170);
  //take the final jump to 190
  heat_up(190);
  //signal to open the door to begin cool down
  open_door();
  
  //Signal success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALL DONE!");
  lcd.setCursor(0, 1);
  lcd.print("I hope it works!");
  
  //either press button1 or reset to do it again
  while(digitalRead(button1) == LOW);  
}

