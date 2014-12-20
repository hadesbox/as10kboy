#include "LedControl.h"

/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */

int ledrowmap[4][2] = {{7,6},{5,4},{3,2},{1,0}};
 
LedControl lc=LedControl(12,11,10,1);

#include <binary.h>


//const int buttonPin = 30;     // down
//const int buttonPin = 32;     // right
//const int buttonPin = 34;     // up
//const int buttonPin = 36;     // left

//red
//const int buttonPin = 26;     // right
//const int buttonPin = 28;     // left

/* we always wait a bit between updates of the display */
unsigned long delaytime=0;

int interval = 100;
unsigned long time;
unsigned long lasttime;
int currentstep = -1;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  lasttime = millis();
  lc.setLed(0,0,0,false);
  
  Serial.begin(9600);
}

void loop() {
  //single();
  //r[random(0,7)][random(0,7)] = !r[random(0,7)][random(0,7)];
  //lc.clearDisplay(0);
  //lc.setColumn(0,2,B10110000);
  //lc.setChar(0,3,'a',false);

  interval = analogRead(0);
  
  time = millis();

  if(time - lasttime > interval){
    currentstep++;  
    if(currentstep>15){
      currentstep=0;
    } 

    lasttime = time;
    tick();
  }

}

void tick(){
  //lc.setLed(0,row,col,r[row][col]);
  int rowtoprint = currentstep;
  int coltoprint = 0;
  if(rowtoprint >7){
    rowtoprint-=8;
    coltoprint = 1;
  }
  Serial.print("current=");
  Serial.print(currentstep);
  Serial.print("row=");
  Serial.print(rowtoprint);
  Serial.print(" col=");
  Serial.println(coltoprint);
  printStep(0,rowtoprint,coltoprint,true);
  printStep(1,rowtoprint,coltoprint,true);
  printStep(2,rowtoprint,coltoprint,true);
  printStep(3,rowtoprint,coltoprint,true);
  delay(10);
  printStep(0,rowtoprint,coltoprint,false);
  printStep(1,rowtoprint,coltoprint,false);
  printStep(2,rowtoprint,coltoprint,false);
  printStep(3,rowtoprint,coltoprint,false);
}


void printStep(int sequence, int rowtoprint, int coltoprint, boolean state){
  lc.setLed(0,rowtoprint,ledrowmap[sequence][coltoprint],state);
}

