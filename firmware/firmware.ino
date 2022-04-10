#include "src/Pedal.h"
#include "src/MIDIUSB.h"
#include <EEPROM.h>

String inputString = "";
int midiChannel = 1;
Pedal pedals[] = {Pedal(A0, A9, 14, 10),Pedal(A1, A8, 15, 16),Pedal(A2, A7, 7, 3),Pedal(A3, A6, 5, 2)};


void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Started");
  EEPROM.get(0, pedals);
  resetPedals();
  printDisplayData();
}

void resetPedals(){
    for (int i = 0; i < 4; i++){
      pedals[i].reset();
      pedals[i].setupPins();
    }
}

void sendPedalMidi(int pedalId){
  controlChange(pedals[pedalId-1].cc, pedals[pedalId-1].value);
}

void controlChange(byte control, byte value) {
  byte channel = byte(midiChannel-1);
  Serial.print("Sending CC: Channel:");
  Serial.print(midiChannel);
  Serial.print(" Control:");
  Serial.print(control);
  Serial.print(" Value:");
  Serial.println(value);
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

int count = 0;
int value = 0;

void loop() {
  delay(20);
  checkSerial();
  for (int i = 0; i < 4; i++){
    if(pedals[i].enabled){
      pedals[i].checkInput();
      if (pedals[i].hasBeenUpdated()){
        outputStatus();
        sendPedalMidi(i+1);
      }
    }
  }

}
void checkSerial(){
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      handleSerialMessage(inputString);
      inputString = "";
    }
    else{
      inputString += inChar;  
    }
  }
}

void printDisplayData(){
  Serial.print("Pedal1 M");
  Serial.print(pedals[0].mode);
  Serial.print(" T");
  Serial.print(pedals[0].type);
  Serial.print(" C");
  Serial.print(pedals[0].connected?1:0);
  Serial.print(" E");
  Serial.print(pedals[0].enabled?1:0);
  Serial.print(" P");
  Serial.print(pedals[0].polarity);
  Serial.print(" CC");
  Serial.print(pedals[0].cc);
  Serial.print(" V");
  Serial.print(pedals[0].value);
  
  Serial.print("  | Pedal2 M");
  Serial.print(pedals[1].mode);
  Serial.print(" T");
  Serial.print(pedals[1].type);
  Serial.print(" C");
  Serial.print(pedals[1].connected?1:0);
  Serial.print(" E");
  Serial.print(pedals[1].enabled?1:0);
  Serial.print(" P");
  Serial.print(pedals[1].polarity);
  Serial.print(" CC");
  Serial.print(pedals[1].cc);
  Serial.print(" V");
  Serial.print(pedals[1].value);
  
  Serial.print("  | Pedal3 M");
  Serial.print(pedals[2].mode);
  Serial.print(" T");
  Serial.print(pedals[2].type);
  Serial.print(" C");
  Serial.print(pedals[2].connected?1:0);
  Serial.print(" E");
  Serial.print(pedals[2].enabled?1:0);
  Serial.print(" P");
  Serial.print(pedals[2].polarity);
  Serial.print(" CC");
  Serial.print(pedals[2].cc);
  Serial.print(" V");
  Serial.print(pedals[2].value);
  
  Serial.print("  | Pedal4 M");
  Serial.print(pedals[3].mode);
  Serial.print(" T");
  Serial.print(pedals[3].type);
  Serial.print(" C");
  Serial.print(pedals[3].connected?1:0);
  Serial.print(" E");
  Serial.print(pedals[3].enabled?1:0);
  Serial.print(" P");
  Serial.print(pedals[3].polarity);
  Serial.print(" CC");
  Serial.print(pedals[3].cc);
  Serial.print(" V");
  Serial.println(pedals[3].value);
}

void outputStatus(){
  Serial.print("-");
  Serial.print(pedals[0].mode);
  Serial.print(",");
  Serial.print(pedals[0].type);
  Serial.print(",");
  Serial.print(pedals[0].connected?1:0);
  Serial.print(",");
  Serial.print(pedals[0].enabled?1:0);
  Serial.print(",");
  Serial.print(pedals[0].polarity);
  Serial.print(",");
  Serial.print(pedals[0].cc);
  Serial.print(",");
  Serial.print(pedals[0].value);

  Serial.print("-");
  Serial.print(pedals[1].mode);
  Serial.print(",");
  Serial.print(pedals[1].type);
  Serial.print(",");
  Serial.print(pedals[1].connected?1:0);
  Serial.print(",");
  Serial.print(pedals[1].enabled?1:0);
  Serial.print(",");
  Serial.print(pedals[1].polarity);
  Serial.print(",");
  Serial.print(pedals[1].cc);
  Serial.print(",");
  Serial.print(pedals[1].value);
  
  Serial.print("-");
  Serial.print(pedals[2].mode);
  Serial.print(",");
  Serial.print(pedals[2].type);
  Serial.print(",");
  Serial.print(pedals[2].connected?1:0);
  Serial.print(",");
  Serial.print(pedals[2].enabled?1:0);
  Serial.print(",");
  Serial.print(pedals[2].polarity);
  Serial.print(",");
  Serial.print(pedals[2].cc);
  Serial.print(",");
  Serial.print(pedals[2].value);
  
  Serial.print("-");
  Serial.print(pedals[3].mode);
  Serial.print(",");
  Serial.print(pedals[3].type);
  Serial.print(",");
  Serial.print(pedals[3].connected?1:0);
  Serial.print(",");
  Serial.print(pedals[3].enabled?1:0);
  Serial.print(",");
  Serial.print(pedals[3].polarity);
  Serial.print(",");
  Serial.print(pedals[3].cc);
  Serial.print(",");
  Serial.println(pedals[3].value);
}

void handleSerialMessage(String message){
  int messageType = message.substring(0, 3).toInt();
  if(messageType==1){
    int settingType = message.substring(3, 6).toInt();
    if(settingType==1){
      int pedalId = message.substring(6, 9).toInt();
      int parameter = message.substring(9, 12).toInt();
      int value = message.substring(12, 15).toInt();
      if(parameter==1){
        Serial.print("Setting Pedal: ");
        Serial.print(pedalId);
        Serial.print(" to CC: ");
        Serial.println(value);
        pedals[pedalId-1].setCC(value);
        EEPROM.put(0, pedals);
      }
      if(parameter==2){
        Serial.print("Setting Pedal: ");
        Serial.print(pedalId);
        Serial.print(" to Mode: ");
        Serial.println(value);
        pedals[pedalId-1].setMode(value);
        EEPROM.put(0, pedals);
      }
      if(parameter==3){
        Serial.print("Setting Pedal: ");
        Serial.print(pedalId);
        Serial.print(" to Type: ");
        Serial.println(value);
        pedals[pedalId-1].setType(value);
        EEPROM.put(0, pedals);
      }
      if(parameter==4){
        Serial.print("Setting Pedal: ");
        Serial.print(pedalId);
        Serial.print(" to Polarity: ");
        Serial.println(value);
        pedals[pedalId-1].setPolarity(value);
        EEPROM.put(0, pedals);
      }
      if(parameter==5){
        Serial.print("Setting Pedal: ");
        Serial.print(pedalId);
        Serial.print(" to Enabled: ");
        Serial.println(value);
        pedals[pedalId-1].setEnabled(value==1);
        EEPROM.put(0, pedals);
      }
    }
  }
  if(messageType==2){
    int settingType = message.substring(3, 6).toInt();
    if(settingType==1){
      int pedalId = message.substring(6, 9).toInt();
      int value = message.substring(9, 12).toInt();
      Serial.print("Testing Pedal: ");
      Serial.print(pedalId);
      Serial.print(" with Value: ");
      Serial.println(value);
      pedals[pedalId-1].setValue(value);
    }
  }
  if(messageType==3){
    printDisplayData();
  }
  if(messageType==4){
    Serial.println("Loading to memory");
    EEPROM.get(0, pedals);
    resetPedals();
  }
  if(messageType==5){
    Serial.println("Saving to memory");
    EEPROM.put(0, pedals);
  }
  if(messageType==6){
    Serial.println("Restting pedals");
    Pedal newPedals[] = {Pedal(A0, A9, 14, 10),Pedal(A1, A8, 15, 16),Pedal(A2, A7, 7, 3),Pedal(A3, A6, 5, 2)};
    EEPROM.put(0, newPedals);
    EEPROM.get(0, pedals);
  }
  outputStatus();
}
