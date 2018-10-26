/*

compas pin SCL - A5, sda - A4 with 5v
nrf 24 pin  CE - D7 , SCK - D13, MISO - D12, CSN- D8, MoSI - D11 with 3.3V


*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h> //I2C Arduino Library
#define addr 0x0D //I2C Address for The HMC5883

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
int i, x1, y1, z1, x, y, z, d1, d2, d3, defRang=30, defRang2=-30,temp1; //triple axis data
String dire, act,st,di,gas,temp2;

void setval(){
                 
            Wire.beginTransmission(addr);
            Wire.write(0x00); 
            Wire.endTransmission();
          
            //Read the data.. 2 bytes for each axis.. 6 total bytes
            Wire.requestFrom(addr, 6);
            if (6 <= Wire.available()) {
              x1 = Wire.read(); //MSB  x
              x1 |= Wire.read() << 8; //LSB  x
              z1 = Wire.read(); //MSB  z
              z1 |= Wire.read() << 8; //LSB z
              y1 = Wire.read(); //MSB y
              y1 |= Wire.read() << 8; //LSB y
            }
  }


void cdirection(){

  if(1){ //y>0&&y<900
    
     if(x<-600){
    // Serial.println("East");
     dire="East";
     di="1";
    
    }else if(x>600){
    // Serial.println("West");
       dire="West";
        di="2";
      }else
  {
    
    if(z>-1500){
     // Serial.println("North");
      dire="North";
       di="3";
      
      }else{
     //   Serial.println("South");
         dire="South";
          di="4";
        }
    
    }
    
    }
  else{
  //  Serial.println("Unstable");
    dire="Unstable";
    di="0";
    }
  
 
}



void check(){
    
    d1= x1-x;
    d2= y1-y;
    d3= z1-z;

    /*
    Serial.print("d1 value = " );

    Serial.print(d1);
    Serial.print("  ");

    Serial.print("d2 value = " );

    Serial.print(d2);
    Serial.print("  ");

    Serial.print("d3 value = " );

    Serial.print(d3);
    Serial.print("  ");
*/

      if( (d1>defRang||d1<defRang2)
      || (d2>defRang||d2<defRang2)
      || (d3>defRang||d3<defRang2) ){

      //  Serial.println("Active ");
        act="Active";
        st="1";
      }else{
    //    Serial.println("Offline ");
         act="Offline";
         st="0";

        }

    
    }


void setup() {

  Serial.begin(9600);
  Wire.begin();
  setval();

  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();

  


  Wire.beginTransmission(addr); //start talking
  Wire.write(0x0B); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x01); // Set the Register
  Wire.endTransmission();
  
  Wire.beginTransmission(addr); //start talking
  Wire.write(0x09); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x1D); // Set the Register
  Wire.endTransmission();

  

  
}


void loop() {

//---------------------


 radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();

  


  Wire.beginTransmission(addr); //start talking
  Wire.write(0x0B); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x01); // Set the Register
  Wire.endTransmission();
  
  Wire.beginTransmission(addr); //start talking
  Wire.write(0x09); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x1D); // Set the Register
  Wire.endTransmission();


//----------------------




  i++;
 if(i>5){
  setval();
  delay(1000);
  i=0;
  }

  

  Wire.beginTransmission(addr);
  Wire.write(0x00); 
  Wire.endTransmission();

  //Read the data.. 2 bytes for each axis.. 6 total bytes
  Wire.requestFrom(addr, 6);
  if (6 <= Wire.available()) {
    x = Wire.read(); //MSB  x
    x |= Wire.read() << 8; //LSB  x
    z = Wire.read(); //MSB  z
    z |= Wire.read() << 8; //LSB z
    y = Wire.read(); //MSB y
    y |= Wire.read() << 8; //LSB y
  }

/*
  // Show Values
  Serial.print("X Value: ");
  Serial.print(x);
  Serial.print("  Y Value: ");
  Serial.print(y);
  Serial.print("  Z Value: ");
  Serial.print(z);
  Serial.println();

*/

  gas = (String)analogRead(A0);

  temp1 = analogRead(A2);
  temp1-=40;
  if(temp1<23){
    temp1=23;
    }else if(temp1>99){
      
      temp1=99;
      }

  temp2 = (String)temp1;


  
  check();
  cdirection();
  String data = String(st+","+di+","+temp2+","+gas+","+act+", "+dire);
  Serial.println(data);


  const char text[32];
  data.toCharArray(text, sizeof(text));
  radio.write(&text, sizeof(text));
 
 /* 
  //cocat string
 
  String thisString1 = String(1365);
  String thisString2 = String(876.998);
  String thisString = String(thisString1 + thisString2);
  Serial.println(thisString);
 */
  

  delay(1000);
}
