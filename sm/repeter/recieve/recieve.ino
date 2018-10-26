

/*
nrf 24 pin  CE - D7 , SCK - D13, MISO - D12, CSN- D8, MoSI - D11 with 3.3V
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00010";  //ch2
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();



  
}

void loop() {
  
  if (stringComplete) {
    Serial.println(inputString);


    const char text[32];
    inputString.toCharArray(text, sizeof(text));
    radio.write(&text, sizeof(text));
  
     //delay(1000);
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}



void serialEvent() {
  while (Serial.available()) {
    
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
