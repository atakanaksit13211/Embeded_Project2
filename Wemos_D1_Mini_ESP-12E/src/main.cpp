#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#include "loraHelper.h"


#define NSS  D3
#define RST  D4
#define DIO0 D8

#define SERVO_ENABLE    D1
#define SERVO_STEP      D2
#define SERVO_DIRECTION D0
#define SERVO_STEP_WAIT 1000 /* in microseconds */


String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBC;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void rotateServo(int steps, bool direction){
  digitalWrite(SERVO_ENABLE, HIGH); //enable servo
  digitalWrite(SERVO_DIRECTION, (direction?HIGH:LOW) ); //set direction 

  long lastStepTime = millis(); // start time


  for(int i = 0; i < steps; i++){
    digitalWrite(SERVO_STEP, HIGH);
    delayMicroseconds(SERVO_STEP_WAIT);
    digitalWrite(SERVO_STEP, LOW);
    delayMicroseconds(SERVO_STEP_WAIT);
  }

  //digitalWrite(SERVO_ENABLE, LOW); //disable servo
}


void setup() {
  Serial.begin(9600);                   // initialize serial
  
  while (!Serial);

  Serial.println("LoRa Duplex with callback");


  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(NSS, RST, DIO0);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true){delay(10);}                       // if failed, do nothing
  }

  setDeviceAddress(localAddress);

  LoRa.onReceive(onStringReceive);
  LoRa.receive();
  Serial.println("LoRa init succeeded.");
}

void loop() {
  if (millis() - lastSendTime > interval) {
    rotateServo(100, 1);
    delay(1000);
    rotateServo(100, 0);

    lastSendTime = millis();
  }

  /*
  if (millis() - lastSendTime > interval) {
    String message = "Message from Wemos D1 Mini!";   // send a message
    sendString(message, &msgCount, localAddress, destination);
    Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;     // 
    LoRa.receive();                     // go back into receive mode
  }
  */
}
