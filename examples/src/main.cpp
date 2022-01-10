/*
 * example for esp32 boards using a tinypico
 * train 1 word and then switches into operator mode on train 6
 *
 * this is by no means a great example but works to prove this library can be used on an esp32 board
 * i build and test this sketch using platformio
 */
#include <TinyPICO.h>
#include "VoiceRecognitionV3.h"
/** temprory data */
uint8_t buf[255];
uint8_t records[7]; // save record

VR myVR(2,3);    // for now these values are ignored just set the right pins when calling begin
bool didTrain = false; // always start untrained
/**
 * @brief   Print "train" command return value.
 * @param   buf  -->  "train" command return value
 * buf[0]    -->  number of records which are trained successfully.
 * buf[2i+1]  -->  record number
 * buf[2i+2]  -->  record train status.
 * 00 --> Trained 
 * FE --> Train Time Out
 * FF --> Value out of range"
 * (i = 0 ~ len-1 )
 * len  -->  length of buf
 */
void printTrain(uint8_t *buf, uint8_t len)
{
  if(len == 0){
    Serial.println(F("Train Finish."));
    return;
  }
  else{
    Serial.print(F("Train success: "));
    Serial.println(buf[0], DEC);
  }
  for(int i=0; i<len-1; i += 2){
    Serial.print(F("Record "));
    Serial.print(buf[i+1], DEC);
    Serial.print(F("\t"));
    switch(buf[i+2]){
    case 0:
      Serial.println(F("Trained"));
      break;
    case 0xFE:
      Serial.println(F("Train Time Out"));
      break;
    case 0xFF:
      Serial.println(F("Value out of range"));
      break;
    default:
      Serial.print(F("Unknown status "));
      Serial.println(buf[i+2], HEX);
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  myVR.begin(9600, 21, 32);

}

void loop() {
  if (didTrain) {
    int ret = myVR.recognize(buf, 50);
    if (ret>0) {
      switch (buf[1]) {
      case 6:
        Serial.println("command matched!");
        break;
      default:
        Serial.println("another command maybe?");
        break;
      }
    }
  } else {
    Serial.println("running train on 6");
    int ret = myVR.train(records, 6, buf);
    if (ret >= 0) {
      Serial.println("success - now test it\n");
      //printTrain(buf, ret);
      didTrain = true;
      myVR.load(6);
    } else if (ret == -1) {
      Serial.println(F("Train failed."));
    } else if (ret == -2) {
      Serial.println(F("Train Timeout."));
    }
  }
}
