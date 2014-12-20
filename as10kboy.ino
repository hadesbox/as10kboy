#include "LedControl.h"

/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
 
LedControl lc=LedControl(12,11,10,1);

#include <binary.h>


//const int buttonPin = 30;     // down
//const int buttonPin = 32;     // right
//const int buttonPin = 34;     // up
//const int buttonPin = 36;     // left

//red
const int buttonPin = 26;     // right
//const int buttonPin = 28;     // left

/* we always wait a bit between updates of the display */
unsigned long delaytime=0;

int interval = 100;
unsigned long time;
unsigned long lasttime;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  lasttime = millis();
  lc.setLed(0,0,0,false);
}



void loop() { 
  //single();
  //r[random(0,7)][random(0,7)] = !r[random(0,7)][random(0,7)];
  //lc.clearDisplay(0);
  //lc.setColumn(0,2,B10110000);
  //lc.setChar(0,3,'a',false);
  
  time = millis();

  if(time - lasttime > interval){
    lasttime = time;
    tick();
  }

}


void tick(){
  interval = analogRead(0);
  //lc.setLed(0,row,col,r[row][col]);
  lc.setLed(0,0,0,true);
  delay(10);
  lc.setLed(0,0,0,false);

}
