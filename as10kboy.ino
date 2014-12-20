#include "LedControl.h"

/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */

int ledrowmap[4][2] = {{7,6},{5,4},{3,2},{1,0}};
 
int sequence1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sequence2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sequence3[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sequence4[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
LedControl lc=LedControl(12,11,10,1);

#include <binary.h>

//black
const int blackdown = 30;     // down
const int blackright = 32;     // right
const int blackup = 34;     // up
const int blackleft = 36;     // left

//red
const int redright = 26;     // right
const int redleft = 28;     // left

/* we always wait a bit between updates of the display */
unsigned long delaytime=0;

int interval = 100;
unsigned long time;
unsigned long lasttime;
int currentstep = -1;

//to control the cursor position;
int cursorrow=0;
int cursorcol=0;
int lastreadcontrols=0;

void setup() {
  //setup the ledmatrix
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  lasttime = millis();
  lc.setLed(0,0,0,false);
  
  //setup the buttons
  pinMode(blackup, INPUT);
  pinMode(blackdown, INPUT);
  pinMode(blackright, INPUT);
  pinMode(blackleft, INPUT);
  pinMode(redleft, INPUT);
  pinMode(redright, INPUT);
  
  Serial.begin(9600);
}

void loop() {

  time = millis();
  
  if(time-lastreadcontrols>100){
    lastreadcontrols=time;
    readControls();
  }
  
  if(time - lasttime > interval){
    currentstep++;  
    if(currentstep>15){
      currentstep=0;
    } 
    lasttime = time;
    tick();
  }
  printSequenceAndCursor();
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
  //printing the current step
  printStep(0,rowtoprint,coltoprint,true);
  printStep(1,rowtoprint,coltoprint,true);
  printStep(2,rowtoprint,coltoprint,true);
  printStep(3,rowtoprint,coltoprint,true);
  //printing the cursor
  delay(10);
  printStep(0,rowtoprint,coltoprint,false);
  printStep(1,rowtoprint,coltoprint,false);
  printStep(2,rowtoprint,coltoprint,false);
  printStep(3,rowtoprint,coltoprint,false);
}

void readControls(){
  interval = analogRead(0);
  if(digitalRead(blackright) == HIGH and cursorrow<7) {
    cursorrow++; 
  }
  if(digitalRead(blackleft) == HIGH and cursorrow>0) {
    cursorrow--; 
  }  
  if(digitalRead(blackdown) == HIGH and cursorcol<7) {
    cursorcol++; 
  }
  if(digitalRead(blackup) == HIGH and cursorcol>0) {
    cursorcol--; 
  }
}

void printSequenceAndCursor(){ 
  lc.clearDisplay(0);
  //lc.setColumn(0,7,B10110000);

  

  lc.setLed(0,cursorrow,7-cursorcol, true);
}

void printStep(int sequence, int rowtoprint, int coltoprint, boolean state){
  lc.setLed(0,rowtoprint,ledrowmap[sequence][coltoprint],state);
}

