#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN , RST_PIN);

int pin1 = 3;
int pin2 = 4;
bool CarDoor = 0;
int O;
int c;



void setup() {
  Serial.begin(115200);
  Serial.println("Approximate your card to the reader...");
  SPI.begin();


  mfrc522.PCD_Init();
  Serial.println();
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
}

void loop() {

  //c=digitalRead(6);
  //O=digitalRead(5);
  //if(O==0&&CarDoor==0)
  //{
  //  CarDoor=1;
  //  Serial.println("open");
  //   digitalWrite(pin1,HIGH);
  //  digitalWrite(pin2,LOW);
  //  delay (5000);
  //  digitalWrite(pin1,LOW);
  //  digitalWrite(pin2,LOW);
  //
  //}

  //if(c==0&&CarDoor==1){
  //  CarDoor=0;
  //  Serial.println("close");
  //     digitalWrite(pin1,LOW);
  //  digitalWrite(pin2,HIGH);
  //  delay (5000);
  //  digitalWrite(pin1,LOW);
  //  digitalWrite(pin2,LOW);
  //
  //}

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID tag:");
  String content = "";
  byte letter;
  for (byte i = 0 ; i < mfrc522.uid.size ; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0X10 ? "0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0X10 ? "0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "16 1F 15 7E"  && CarDoor == 0)
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    delay (5000);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    CarDoor = 1;
  }

  else {
    Serial.println("Access denied");
    delay(3000);
  }
}

void serialEvent() {
  String OpenDoorMsg = "IOPC";
  String CloseDoorMsg = "ICLC";
  while (Serial.available()) {
    String incomingByte = Serial.readString();
    if (  incomingByte.startsWith(OpenDoorMsg) ) {
      if (CarDoor == 0)
      {
        CarDoor = 1;
        Serial.println("open");
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        delay (5000);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);

      }

    }
    else if (incomingByte.startsWith(CloseDoorMsg)) {
      if (CarDoor == 1) {
        CarDoor = 0;
        Serial.println("close");
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        delay (5000);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);

      }

    }

  }
}

