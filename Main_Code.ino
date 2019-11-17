#include <Keypad.h>
#include <Password.h>
#include "SimpleTimer.h"
#include "DHT.h"


#define COMMON_ANODE

int DHTPIN = 32; //dht

DHT
dht(DHTPIN, DHTTYPE);

SimpleTimer timer1;
SimpleTimer timer2;
SimpleTimer timer3;
int timer1Id;
int timer2Id;
#define dpg Serial
#define esp Serial1//0000000000000000000000000000000000000000000000
#define uno Serial2


String Gaz;//gaz value from gaz sensor
int door = 0; //variable for selecting close open door

float i = 0; //door delay

int maxGazValue = 400; //***

float celsius = 0; // LM35

int trigPin = 22; //ultrasonic
int echoPin = 24; //ultrasonic
long duration, distance;//ultrasonic



//auoto light control
const int sensorPinLdr0 = A1;
const int sensorPinLdr1 = A3;

int sensorValueLdr0;
int sensorValueLdr1;

int door1pin1 = 5;
int door1pin2 = 6;

int alarmRedLed = 1;
int Buzzer = 9;
int ledPinLdr0 = 14;
int ledPinLdr1 = 12;

//for light control
int cool = 0; //select auto control lights

int Thief = 0; //ultrasonic is off

String color = "GET";
String incomingByte ;
String LightOnMsg1 = "TLOR1";//Turn the light on, in room one
String LightOnMsg2 = "TLOR2";//Turn the light on, in room two
String LightOnMsg3 = "TLOR3";//Turn the light on, in room three
String LightOnMsg4 = "TLOR4";//Turn the light on, in room four
String LightOffMsg1 = "TLFR1";//Turn the light off, in room one
String LightOffMsg2 = "TLFR2";//Turn the light off, in room two
String LightOffMsg3 = "TLFR3";//Turn the light off, in room three
String LightOffMsg4 = "TLFR4";//Turn the light off, in room four
String RedLightOnMsg = "RLO ";//turn the red light on
String RedLightOffMsg = "RLF "; //turn the red light off
String BlueLightOnMsg = "BLO";//turn the blue light on
String BlueLightOffMsg = "BLF "; //turn the blue light off
String PurpleLightOnMsg = "PLO"; //turn the purpl light on
String PurpleLightOffMsg = "PLF"; //turn the purpl light off
String OpenDoorMsg = "OTD";//Open the door
String ClosDoorMsg = "CTD";//Close the door
String OpenDoorMsgCA = "OTCA";//Open the door
String ClosDoorMsgCA = "CTCA";//Close the door

String BuzzerOffMsg = "BF";//Turn the buzzer off

String UltrasonicOn = "UO";//start security code
String UltrasonicOff = "UF"; //stop security code

String controlLight = "CFU"; //controling from user
String NcontrolLight = "NCFU"; //notControlLight from user

int Light4KTR = 14; //led1 pin in kitchen room
int Light2BDR = 15; //led2 pin in bed room
int Light3PTR = 28; //led3 pin Bath room
int Light1LVR = 26; //led4 pin Living room
int redPin = 10;
int greenPin = 11;
int bluePin = 12;
int Capin1 = 2; // to arduino Uno
int Capin2 = 3; // to arduino Uno



//for open the door
Password password = Password( "1992" );
int DoreDelay = 1000; // time to close the main dore or open
const int ROWS = 4; //for keybad
const int COLS = 3; //for keybad

const int pin11 = 16; //control the main dore
const int pin22 = 17 ; //control the main dore
//keybad array
char keys [ROWS][COLS] =
{

  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {40, 42, 44, 46}; //for keybad
byte colPins[COLS] = {48, 50, 52}; //for keybad
Keypad keypad = Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//
String openTheDoor = "OTD"; //openTheDoor
String closeTheDoor = "CTD"; //closeTheDoor
String controlLight = "CFU"; //controling from user
String NcontrolLight = "NCFU"; //notControlLight from user

//.................................................................................................................................


void setup() {



  dpg.begin(115200);
  esp.begin(115200);
  uno.begin(115200);

  dht.begin();

  dpg.println("set up ");

  timer1Id = timer1.setInterval(2000, takeReadingFromSensor1);
  timer2Id = timer2.setInterval(3000, takeReadingFromSensor2);
  timer3.setInterval(4000, takeReadingFromSensor3);//****

  //auto ligth control
  pinMode(ledPinLdr0, OUTPUT);
  pinMode(ledPinLdr1, OUTPUT);


  pinMode(alarmRedLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  //for light control
  pinMode(Light1LVR , OUTPUT);
  pinMode(Light2BDR , OUTPUT);
  pinMode(Light3PTR , OUTPUT);
  pinMode(Light4KTR , OUTPUT);
  pinMode(door1pin1 , OUTPUT);
  pinMode(door1pin2 , OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(Capin1, OUTPUT);
  pinMode(Capin2, OUTPUT);
  digitalWrite(Capin1, HIGH);
  digitalWrite(Capin2, HIGH);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  pinMode(pin11, OUTPUT);
  pinMode(pin22, OUTPUT);

  for (int rov = 0; rov < ROWS ; rov++)
  {
    pinMode(rowPins[rov], INPUT);
    digitalWrite(rowPins[rov], HIGH);
  }

  for (int column = 0; column < COLS ; column++)
  {
    pinMode(colPins[column], OUTPUT);
    digitalWrite(colPins[column], HIGH);
  }

}
//...............................................................................................................

void loop() {

  timer1.run();
  timer2.run();
  timer3.run();
  if (cool == 0) {
    AutomaticLightControl();
  }
  if (Thief == 1) {
    ultrasonic ();
  }


}
void serialEvent1() {


  while (esp.available()) {
    incomingByte = esp.readString();



    if (  incomingByte.startsWith(OpenDoorMsg) ) {
      if (door == 0) {
        dpg.println("open the door");
        open_TheDoor();

      }
    }

    else if ( incomingByte.startsWith(ClosDoorMsg ) ) {
      if (door == 1) {
        dpg.println("colse the door");
        close_TheDoor();

      }
    }

    else if (incomingByte.startsWith(OpenDoorMsgCA) ) {
      //      digitalWrite(Capin1, LOW);
      uno.println("IOPC");
      dpg.println("open carage door");
      //      delay(200);
      //      digitalWrite(Capin1, HIGH);
    }

    else if ( incomingByte.startsWith(ClosDoorMsgCA) ) {
      //      digitalWrite(Capin2, LOW);
      uno.println("ICLC");
      dpg.println("close carage door");
      //      delay(200);
      //      digitalWrite(Capin2, HIGH);
    }
    else if (incomingByte.startsWith(NcontrolLight) ) {
      cool = 0;
      dpg.println("automatic light on");
    }

    else if ( incomingByte.startsWith(controlLight) ) {
      cool = 1;

      dpg.println("user is controling the light");
    }
    else if ( incomingByte.startsWith ( LightOnMsg1) ) {
      digitalWrite (Light1LVR , HIGH);
      dpg.println("light 1 is on");
    }
    else if ( incomingByte.startsWith ( LightOnMsg2) ) {
      digitalWrite (Light2BDR , HIGH);
      dpg.println("light 2 is on");
    }
    else if ( incomingByte.startsWith ( LightOnMsg3) ) {
      digitalWrite (Light3PTR , HIGH);
      dpg.println("light 3 is on");
    }
    else if ( incomingByte.startsWith ( LightOnMsg4) ) {
      digitalWrite (Light4KTR, HIGH);
      dpg.println("light 4 is on");
    }
    else if ( incomingByte.startsWith ( LightOffMsg1) ) {
      digitalWrite (Light1LVR , LOW);
      dpg.println("light 1 is off");
    }
    else if ( incomingByte.startsWith ( LightOffMsg2) ) {
      digitalWrite (Light2BDR , LOW);
      dpg.println("light 2 is off");
    }
    else if ( incomingByte.startsWith ( LightOffMsg3) ) {
      digitalWrite (Light3PTR , LOW);
      dpg.println("light 3 is off");
    }
    else if ( incomingByte.startsWith ( LightOffMsg4) ) {
      digitalWrite (Light4KTR , LOW);
      dpg.println("light 4 is off");
    }

    else if ( incomingByte.startsWith(color) ) {

      int indexOfRed = incomingByte.indexOf("r");
      int indexOfGreen = incomingByte.indexOf("g");
      int   indexOfBlue  = incomingByte.indexOf("b");
      String r1 = incomingByte.substring(indexOfRed + 1, indexOfGreen);
      String g1 = incomingByte.substring(indexOfGreen + 1, indexOfBlue);
      String b1 = incomingByte.substring(indexOfBlue + 1);
      int red = r1.toInt();
      int green = g1.toInt();
      int blue = b1.toInt();
      dpg.println(red);
      setColor( red, green, blue);
    }

    else if (incomingByte.startsWith (BuzzerOffMsg)) {
      digitalWrite(Buzzer, LOW);
      dpg.println("Buzzer is off");
    }
    else if (incomingByte.startsWith (UltrasonicOn)) {
      Thief = 1;
      dpg.println("security code is on");
    }
    else if (incomingByte.startsWith (UltrasonicOff)) {
      Thief = 0;
      digitalWrite(Buzzer, LOW);
      dpg.println("security code is off");
    }

    else {
      dpg.println(esp.readString());
    }

    esp.flush();
  }
}


//................................................................................

void ultrasonic () {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.034029; // 340.29 * (100/1000000) = 340.29/10000

  if (distance >= 200 || distance <= 0) {
  }

  else {
    dpg.print(distance);
    dpg.println(" cm");
  }

  delay(500);

  if (distance <= 20) {
    esp.println("s1.");
    digitalWrite(Buzzer, 1);

  }
}

//...............................................................................
void  AutomaticLightControl() {


  sensorValueLdr0 = analogRead(sensorPinLdr0);
  sensorValueLdr0 = map( sensorValueLdr0, 0, 1024, 0, 100);
  if (sensorValueLdr0 <= 30)
    digitalWrite(Light1LVR, HIGH);
  else
    digitalWrite(Light1LVR, LOW);
  sensorValueLdr1 = analogRead(sensorPinLdr1);
  sensorValueLdr1 = map( sensorValueLdr1, 0, 1024, 0, 100);
  if (sensorValueLdr1 <= 30)
    digitalWrite(Light2BDR, HIGH);
  else
    digitalWrite(Light2BDR, LOW);
}

//.................................................................................
void  open_TheDoor() {
  char key;
  while (key != '#' ) {

    key = keypad.getKey();

    if (key != 0)
    {
      dpg.print("GOT key");
      dpg.println(key);
      delay(10);
      switch (key)
      {
        case '#': CheckPassword(); delay(1); break;
        case '*': password.reset(); delay(1); break;
        default : password.append(key); delay(1);
      }

    }
  }
}

//...................................................................................
void CheckPassword()
{
  if (password.evaluate())
  {
    dpg.println("Accepted");
    delay(10);

    analogWrite(door1pin1, 255);
    analogWrite(door1pin2, 0);
    delay(DoreDelay);
    analogWrite(door1pin1, 0);
    analogWrite(door1pin2, 0);
    door = 1;
    password.reset();
    delay(10);
  }


}

void  close_TheDoor() {
  analogWrite(door1pin1, 0);
  analogWrite(door1pin2, 255);
  dpg.println("close the door");
  delay(DoreDelay);
  analogWrite(door1pin1, 0);
  analogWrite(door1pin2, 0);
  door = 0;


}
//....................................................................................................

void setColor(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
//.......................................................................................................
void takeReadingFromSensor1()
{
  int sensor1Reading = 0;

  // output the value of each analog input pin
  int analogChannel = 0;
  sensor1Reading = analogRead(analogChannel);
  celsius = ((sensor1Reading * 5000) / 1024) / 10;
  String temp = String("t");
  temp += String(celsius);
  temp += String(".");
  esp.println(temp);
  dpg.println(temp);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void takeReadingFromSensor2()
{
  int sensor2Reading = 0;

  // output the value of each analog input pin
  int analogChannel = 2 ;
  sensor2Reading = analogRead(analogChannel);
  if (sensor2Reading > maxGazValue) {
    digitalWrite(alarmRedLed, HIGH);
    setColor(0, 255, 255);
    digitalWrite(Buzzer, HIGH);
    esp.println("s2.");
    delay(250);
    digitalWrite(alarmRedLed, LOW);
    digitalWrite(Buzzer, LOW);
  }
    String gaz = String("g");
    gaz += String(sensor2Reading);
    gaz += String(".");
    esp.println(gaz);
    dpg.println(gaz);
  timer1.restartTimer(timer1Id);

}

void takeReadingFromSensor3()
{
  int Sensor3Reading = 0;

  Sensor3Reading = dht.readHumidity() / 20;

  String Humidity = String("h");
  Humidity += String(Sensor3Reading);
  Humidity += String(".");
  esp.println(Humidity);
  dpg.println(Humidity);


  if (isnan(Sensor3Reading)) {
    Serial.println("Failed to read from DHT");
  }


  timer2.restartTimer(timer2Id);

  timer1.restartTimer(timer1Id);
}

