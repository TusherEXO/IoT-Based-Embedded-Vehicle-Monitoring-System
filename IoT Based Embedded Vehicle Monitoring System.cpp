#include "sonar.h"
#include "buzzer.h"
#include "pir.h"
#include "smoke.h"
#include "Display.h"
#include "key.h"
#include "fire.h"
#include "msg.h"
int t1=9,e1=10,t2=4,e2=5,p=6,b=7,g=A1,f=8,pir;
#include <Keypad.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //three columns
char keys[ROW_NUM][COLUMN_NUM] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte pin_rows[ROW_NUM] = {53, 51, 49, 47}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {52, 50, 48, 46}; //connect to the column pinouts of
the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM,
COLUMN_NUM );

const String password = "1234"; // change your password here
String input_password;
void setup() {
Serial.begin(9600);

33

pinMode(t1,OUTPUT);pinMode(e1,INPUT);pinMode(t2,OUTPUT);pinMode(e2,INPUT);
pinMode(p,OUTPUT);pinMode(b,OUTPUT);
input_password.reserve(32); // maximum input characters is 33, change if needed
lcd.begin();
lcd.backlight();
lcd.clear();
}
void loop() {
char key = keypad.getKey();
if (key){
Serial.println(key);
if(key == '*') {
input_password = ""; // clear input password
} else if(key == '#') {
if(password == input_password) {
Serial.println("password is correct");
//--------------------------Stat------------------------
int distance1=getDistance(t1,e1);int distance2=getDistance(t2,e2);
int pir=getPIR(p);int gas=smoke(g);
Serial.print("D1 ");Serial.print(distance1);Serial.print(" D2
");Serial.print(distance2);Serial.print(" ");Serial.print(gas);Serial.print(" ");Serial.print(pir);
digitalWrite(p,HIGH);lcd.clear();lcd.setCursor(0,0);lcd.print("Engine
Started");delay(2000);lcd.clear();
printDis(distance1,distance2);
Serial.println(digitalRead(f));
if(!fire(f)){
lcd.clear();lcd.setCursor(0,0);lcd.print("Warning Flame");beep(7);sendMsg("Flame
Warning");
}
if(getPIR(pir)){

34

lcd.clear();lcd.setCursor(0,0);lcd.print("An unknown person
detected");beep(7);sendMsg("An unknown person detected");
}
if(smoke(g)){
gassWarning();beep(7);sendMsg("Smoke detected");
}
if(distance1<10){
obstacleWarning('F');beep(7);sendMsg("Front obstacle");
}
if(distance2<10){
obstacleWarning('B');beep(7);sendMsg("Back obstacle");
}
} else {
Serial.println("password is incorrect, try again");
}

input_password = ""; // clear input password
} else {
input_password += key; // append new character to input password string
}
}

}