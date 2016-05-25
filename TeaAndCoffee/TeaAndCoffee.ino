/*
 Program that controls the heating temp of water for different brews
 
 Provides a menu that you scroll up and down to choose from pre-programmed temps
 
 Selecting a temp turns the kettle on till that temperature is reached
 
 Program that maintains that temp while flashing the screen for 5 minutes before
 automatically just turning off
 
 At any time press Reset to go back to the start
 
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

//BREWING TEMPS
//The boiling temperatures used for different brews
//temps are in °F
const int BlackTea = 200;
const int OolangTea = 190;
const int GreenTea = 155;
const int WhiteTea = 180;
const int Rooibos = 200;
const int Herbal = 190;
const int Coffee = 180;

//variable used for keeping track of the menu screens
int count = 1;
//variable for containing the temps
int tempArray[] = {GreenTea,
                    BlackTea,
                    OolangTea,
                    Rooibos,
                    Herbal,
                    WhiteTea,
                    Coffee};

int convert_degrees(int analog_value){
  int temperature = 0;
  
  //NOTE: IF YOU WANT TO FINE TUNE YOUR TEMPERATURE READOUTS
  //THIS IS THE PLACE TO TO DO IT!
  int offset = 23;
  
  //the circuit changes 10mV/degree Celsius (hopefully)
  //Analog inputs measure 0 to 5V
  //0 to 5V is a range of 0 to 500 degrees Celsius
  
  //UNCOMMENT THIS LINE FOR CELSIUS
  //temperature = map(analog_value, 0, 1023, 0, 500) + offset;
  
  //0 to 500 degrees Celsius is 32 to 932 degrees Farenheit
  //UNCOMMENT THIS LINE FOR FARENHEIT 
  //change the scaling appropriately for more accurate readings 
  temperature = map(analog_value, 0, 1023, 32, 900) - offset;

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
 
//routine to maintain the temperature at specified temp and for duration time
void maintain_temp(int target_temp, int time_in_min){
  //only check the temperature once per minute
  int num_loops = time_in_min;
  for(int x = 0; x < num_loops; x++){
    //clear lcd and print current operation
    lcd.clear();
    lcd.home();
    lcd.print("Maintain temp");
    lcd.setCursor(0, 1);
    //display number of minutes
    lcd.print("Num min:  ");
    lcd.print(x);
    lcd.print("   ");
    
    //loop for one minute maintaining the temperature
    //and flashing the light for attention
    //Checks the temperature every 10 seconds
    for(int y = 0; y < 6; y++){
      if (convert_degrees(analogRead(thermo1)) < target_temp){
        digitalWrite(outlet1, HIGH);
      }
      else {
        digitalWrite(outlet1, LOW);
      }
      //flash the light and delay for 10 seconds
      for(int z = 0; z < 10; z++){
        if ( (z % 2) == 0) digitalWrite(backlight, HIGH);
        else digitalWrite(backlight, LOW);
        //delay for one second
        delay(1000);
      }
    }
  }
  //turn outlet off before exiting
  digitalWrite(outlet1, LOW);
  //make sure the backlight is off
  digitalWrite(backlight, LOW);
}

void setscreen(int temp, char brew[]){
  //setup LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(brew);
  lcd.print(temp);
  lcd.print(" F");
  lcd.setCursor(0, 1);
  lcd.print("Select to Start");

}
  

void menuscreen(int num){
  switch (num){
  case 1:
    setscreen(GreenTea, "Green Tea ");
    break;
  case 2:
    setscreen(BlackTea, "Black Tea ");
    break;
  case 3:
    setscreen(OolangTea, "Oolang Tea ");
    break;
  case 4:
    setscreen(Rooibos, "Rooibos ");
    break;
  case 5:
    setscreen(Herbal, "Herbal ");
    break;
  case 6:
    setscreen(WhiteTea, "White Tea ");
    break;
  case 7:
    setscreen(Coffee, "Coffee ");
    break;
  }
}

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
  
  menuscreen(count);
}

void loop() {
 
  //buttonFour moves down the list
  //remember this button is on an analog pin
  if (analogRead(buttonFour) > 600){
    if (count == 7) (count = 1);
    else (count = count + 1);
    menuscreen(count);
    //pause to let them release the button
    delay(500);
  }

  //buttonThree moves up the list
  //remember this button is on an analog pin
  if (analogRead(buttonThree) > 600){
    if (count == 1) (count = 7);
    else (count = count - 1);
    menuscreen(count);
    //pause to let them release the button
    delay(500);
  }

  //button2 selects the current menu item
  if (digitalRead(button2) == HIGH){
    Serial.println(count);
    Serial.print("Heating to: ");
    Serial.println(tempArray[(count-1)]);
    heat_up(tempArray[(count-1)]);
    maintain_temp(tempArray[(count-1)], 5);

    //reset and start over
    count = 1;
    menuscreen(count);
  }

}
