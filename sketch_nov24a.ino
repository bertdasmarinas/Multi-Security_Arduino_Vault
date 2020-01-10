#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Keypad.h>


char* password ="1234"; 
int count = 0; 
int relay_pin = 13; //pin 13 for relay + solenoid
const int buzzer = 12;
const byte rows = 4; 
const byte cols = 4;
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
char keyMap [rows] [cols] = { 

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins [rows] = {11,10,9,8}; 
byte colPins [cols] = {7,6,5,4};
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);
LiquidCrystal_I2C lcd (0x27, 16, 2); 

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  finger.begin(57600);
  pinMode(relay_pin , OUTPUT);
  if(finger.verifyPassword()){
    lcd.setCursor(0,0);
    lcd.print("Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("Sensor connected");
    delay(3000);
    lcd.clear();
  }
  else{
     lcd.setCursor(0,0);
     lcd.print("No sensor");
     lcd.setCursor(0,1);
     lcd.print("found");
     delay(3000);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Check pins");

     while(1){
      delay(1);
     }
  } 
}
void loop(){
  int i;
  char keypressed = myKeypad.getKey(); 
  if (int(keypressed) != 0) {
    lcd.setCursor(0,0);
    lcd.print("Enter password: ");
    lcd.setCursor(0,1);    
    for (i=0; i<=count; ++i){ 
      lcd.print("*");
    }    
    if (keypressed == password[count]){ //iteration of continuous typing
        ++count;
        if (count == 4)
        {
          unlockDoor();                                 
          count = 0;
        }       
      } else {
       invalidCode();   
        count = 0;
      }
   }  
}

  void invalidCode()  //wrong input of code
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ACCESS DENIED!");
  lcd.setCursor(0,1);
  lcd.print("INVALID CODE");
  tone(buzzer, 1000); 
  delay(1000);        
  noTone(buzzer);
  delay(2000);
  displayCodeEntryScreen();
   delay(5000);
  lcd.clear();
}
 void unlockDoor()  //unlocks the door
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ACCESS GRANTED");
  lcd.setCursor(0,1);
  lcd.print("WELCOME!");    
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Place finger");
  getFingerprintIDez();
}
 void displayCodeEntryScreen()  //display the code on the screen
{
  clearScreen();
  lcd.setCursor(0,0);
  lcd.print("Enter the");
  lcd.setCursor(0,1);
  lcd.print("correct password");
}
 void clearScreen()       //clears the screen
{
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
} 

// returns -1 if failed, otherwise returns ID #
  int getFingerprintIDez() {  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK){
    lcd.setCursor(0,0);
    lcd.print("Waiting for");
    lcd.setCursor(0,1);
    lcd.print("valid finger");
    delay(3000);
    lcd.clear();
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    lcd.clear();
    tone(buzzer, 1000); 
    delay(1000);        
    noTone(buzzer);
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("Messy image");
    lcd.setCursor(0,1);
    lcd.print("try again");
    delay(3000);
    lcd.clear(); 
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    lcd.clear();
    tone(buzzer, 1000); 
    delay(1000);        
    noTone(buzzer);
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("Invalid finger");
    delay(3000);
    lcd.clear();   
    return -1;
  }
 // found a match!
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Found ID#"); 
  lcd.print(finger.fingerID);
  delay(5000);
  lcd.clear(); 
  digitalWrite(relay_pin, HIGH);
  delay(10000);
  digitalWrite(relay_pin , LOW);
  delay(5000);
  return finger.fingerID; 
  delay(5000);
     
}

  
