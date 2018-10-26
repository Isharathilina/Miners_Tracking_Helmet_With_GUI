  
// Example by Tom Igoe

import processing.serial.*;
import meter.*;

Meter m, m2;
Serial myPort;  // The serial port
char st, di;
char[] gval = new char[3];
char[] temp = new char[2];
int i=0,cx=80,cy=400,rcx=80,rcy=55,intgval1, tempval;

void setup() {
    size(1280, 768);  
   // fullScreen();
   line(0, 60, 1500, 60);
 //  height widthg
 
  // ellipse(75, 40, 50, 50);
 
 
   
   textSize(32);
    fill(0, 0, 255 );
   text("Smart Helmet Monitoring System", 400, 30); 
   fill(255, 0, 0 );
   //line(400, 30, 900, 30);
   
   textSize(20);
    fill(255, 0, 0 );
   text("offline:-",0,55); 
  
   
   
  // List all the available serial ports:
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);   //select com port
  
  
  m = new Meter(this, 1030, 60); // Instantiate a meter class. position
  m.setDisplayDigitalMeterValue(true);
  // Set a warning if sensor value is too high.
  m.setHighSensorWarningActive(true);
  m.setHighSensorWarningValue((int)150);
  
  m.setMeterWidth(250);
  m.setMaxInputSignal(1000);
  m.setShortTicsBetweenLongTics(0);
  m.setTitle("Gas level");
  String[] scaleLabelsAV = {"0", "50", "100", 
    "150", "200", "250", "300"};
  m.setScaleLabels(scaleLabelsAV);
  m.setMaxScaleValue(300);
  m.updateMeter(0);
  
  //m2 for temp
  
  
  m2 = new Meter(this, 1030, 260); // Instantiate a meter class. position
  m2.setDisplayDigitalMeterValue(true);
  // Set a warning if sensor value is too high.
  m2.setHighSensorWarningActive(true);
  m2.setHighSensorWarningValue((int)150);
  
  m2.setMeterWidth(250);
  m2.setMaxInputSignal(100);
  m2.setShortTicsBetweenLongTics(0);
  m2.setTitle("Temperature");
  String[] scaleLabelsAV2 = {"10", "20", "30","40", "50", "60", 
    "70", "80", "90", "100"};
  m2.setScaleLabels(scaleLabelsAV2);
  m2.setMaxScaleValue(100);
  m2.updateMeter(0);
  
  
  
 
}

void draw() {
  
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      println(inBuffer);
      st= inBuffer.charAt(0);
      di= inBuffer.charAt(2);
   
      // meter update
     inBuffer.getChars(7, 10, gval, 0);
     int a = gval[0];
     a=a-48;
     int b = gval[1];
     b=b-48;     
     
     if(gval[2]==','){
     
     intgval1 = (a*10)+b;
     }else{
     
     int c = (int)gval[2];
     c=c-48;
     intgval1 = (a*100)+(b*10)+c;
     }
     
     // temp value setting
     
     inBuffer.getChars(4, 6, temp, 0);
     int a1 = temp[0];
     a1=a1-48;
     int b1 = temp[1];
     b1=b1-48;     
     
     tempval = (a1*10)+b1;
     
    
     
     
     println(intgval1);
      println(tempval);
     m.updateMeter(intgval1);
     m2.updateMeter(tempval-7);
     
     //end m update
     
      if(st=='1'){
      
      switch(di){
      case '1': cx+=7;
      break;
      case '2': cx-=7;
      break;
      case '3': cy-=7;
      break;
      case '4': cy+=7;
      break;
      
      }
      
       ellipse(cx, cy, 15, 15);
      fill(0,0,0);
      
      }else{
       
       ellipse(cx, cy, 15, 15);
       fill(200,100,0);
       
       ellipse(rcx, rcy, 15, 15);
       fill(255,0,0);
       rcx = rcx+7;
      
      
      }
      
      
       
      
    //  i=i+7;
      
      
    }
  }
}
