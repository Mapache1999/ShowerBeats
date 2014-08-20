
#include <SoftwareSerial.h>

extern volatile unsigned long timer0_millis;
extern volatile unsigned long timer0_overflow_count;

SoftwareSerial BLESerial(11, 12); // RX, TX

long time=1;
String Command="";
String content ="";
char ch;
unsigned long timer1;


const int VALVE_PIN_BENBL = 2;  // (pwm) pin 2 connected to pin B ENBL 
const int VALVE_PIN_BPHASE= 3;  // (GPIO) pin 3 connected to pin B Phase 
const int BLUE_PIN_AENBL= 4;  // (pwm) pin 4 connected to pin A ENBL  
const int BLUE_PIN_APHASE= 5;   // (GPIO) pin 5 connected to pin A Phase
const int MODE_PIN= 6; //(GPIO) Mode pin must be set high for phase GPIO pin 6

const int LED_PIN =13; //LED SYSTEM ON PIN

//byte speed = 255; //chnage Speed if motor

int UnusedPINS []= {7, 8, 9, 10,14,15,16}; 

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Serial Listo");
  delay(500);
  
  // set the data rate for the SoftwareSerial port
  BLESerial.begin(9600);
  BLESerial.println("BLESerial on: PIN2 RX; PIN3 TX");
  delay(500);

  //Set usefull pins
  pinMode(VALVE_PIN_BENBL, OUTPUT);
  pinMode(VALVE_PIN_BPHASE, OUTPUT);
  pinMode(BLUE_PIN_AENBL, OUTPUT);
  pinMode(BLUE_PIN_APHASE, OUTPUT);
  pinMode(MODE_PIN, OUTPUT); 
  //Set initial PIN State
  digitalWrite(VALVE_PIN_BENBL,LOW);
  digitalWrite(VALVE_PIN_BPHASE,LOW);
  digitalWrite(BLUE_PIN_AENBL,LOW);
  digitalWrite(BLUE_PIN_APHASE,LOW);
  digitalWrite(MODE_PIN,HIGH);
  
  //disabling unused pins
  for (int i =0; i <= 6; i++){
    Serial.print("disable PIN: ");
    Serial.println(UnusedPINS[i]);
    pinMode(UnusedPINS[i], OUTPUT);
    digitalWrite(UnusedPINS[i], LOW);
  }
}

void loop() {
  
   Serial.println("RDY");
   delay(500);
   
  // put your main code here, to run repeatedly:
  String content ="";
  String contentINT ="";

  Command = "DEF";
  
  while (BLESerial.available()){
    if(BLESerial.available() > 0)
    {
      //read whats on serial
      ch = BLESerial.read();
      Serial.println(ch);
      delay (500);
      if (isDigit(ch)){
      contentINT.concat(ch);
      delay(10);
      Serial.print("concatenated String is:");
      Serial.println(contentINT);
      time = contentINT.toInt();
      }
      else {
      content.concat(ch);
      delay (10);
      Command = content;
      }
      
      Serial.print("Coomand is: ");
      Serial.println(Command);
      delay(500);
      Serial.print("time is in int: ");
      Serial.println(time);
      delay (500);
    }
  }
  Action ();
  
   if(millis() - timer1 > time * 60UL * 1000UL){
    OFF_WATER();
    delay(100);
    OFF_MUSIC();
    delay(100);
    ResetMillis();
    Serial.print("millis reset to:");
    Serial.println(millis());
    delay(500);
    timer1=0;
    }
    
   if (Serial.available()) {
    BLESerial.write(Serial.read());
  }
}

void ON_WATER (){
  analogWrite(VALVE_PIN_BENBL,  HIGH);
  analogWrite(VALVE_PIN_BPHASE,  LOW);
  Serial.println("Valve ON");
  delay(500);
  analogWrite(VALVE_PIN_BENBL,  LOW);
  analogWrite(VALVE_PIN_BPHASE,  LOW);
}

void OFF_WATER (){
 analogWrite(VALVE_PIN_BENBL,  HIGH);
 analogWrite(VALVE_PIN_BPHASE,  HIGH);
 Serial.println("Valve OFF");
 delay(500);
 analogWrite(VALVE_PIN_BENBL,  LOW);
 analogWrite(VALVE_PIN_BPHASE,  LOW);
}

void ON_MUSIC(){
  digitalWrite(BLUE_PIN_AENBL, HIGH);
  digitalWrite(BLUE_PIN_APHASE, LOW);
  Serial.println("Music is ON");
  delay(100);

}

void OFF_MUSIC(){
  digitalWrite(BLUE_PIN_AENBL, LOW);
  digitalWrite(BLUE_PIN_APHASE, LOW);
  Serial.println("Music is off");
  delay(100);
}

void Action(){
 if (Command == "ON"){
    timer1 = millis();
    ON_WATER();
    ON_MUSIC();
    Serial.print("turning Shower on for m: ");
    Serial.println(time);
    delay(500);
    return;
 }
 if (Command == "OFF") {
    OFF_WATER ();
    OFF_MUSIC();
    Serial.println("Command Received, Turning OFF shower");
    delay(500);
    return;
 }
 else {
     Serial.println("no command");
     delay(500);
     return;
  }
 }
  
 void ResetMillis()
{
    unsigned char oldSREG = SREG;  // almacena el registro de estado
    cli();  // para las interrupciones
    timer0_millis = 0;
    timer0_overflow_count = 0;
    SREG = oldSREG;
} 
  
