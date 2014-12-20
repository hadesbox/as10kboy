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
  
  if(digitalRead(redleft) == HIGH){
  
    if(cursorcol == 0) {
      sequence1[cursorrow]=!sequence1[cursorrow];
    }
    else if(cursorcol == 1) {
      sequence1[8+cursorrow]=!sequence1[8+cursorrow];
    } 
    
    else if(cursorcol == 2) {
      sequence2[cursorrow]=!sequence2[cursorrow];
    } 
    else if(cursorcol == 3) {
      sequence2[8+cursorrow]=!sequence2[8+cursorrow];
    } 
    
    else if(cursorcol == 4) {
      sequence3[cursorrow]=!sequence2[cursorrow];
    } 
    else if(cursorcol == 5) {
      sequence3[8+cursorrow]=!sequence2[8+cursorrow];
    }
    
    else if(cursorcol == 6) {
      sequence4[cursorrow]=!sequence2[cursorrow];
    } 
    else if(cursorcol == 7) {
      sequence4[8+cursorrow]=!sequence2[8+cursorrow];
    }
  
  }  
  
}

void printSequenceAndCursor(){ 
  lc.clearDisplay(0);
  //lc.setColumn(0,7,B10110000);

  int seq1a = sequence1[0] << 7 | sequence1[1] << 6 |  sequence1[2] << 5 | sequence1[3] << 4  |sequence1[4] << 3   | sequence1[5] << 2  | sequence1[6] << 1  | sequence1[7];
  int seq1b = sequence1[8] << 7 | sequence1[9] << 6 | sequence1[10] << 5 | sequence1[11] << 4 | sequence1[12] << 3 | sequence1[13] << 2 | sequence1[14] << 1 | sequence1[15];

  int seq2a = sequence2[0] << 7 | sequence2[1] << 6 |  sequence2[2] << 5 | sequence2[3] << 4  | sequence2[4] << 3  | sequence2[5] << 2  | sequence2[6] << 1  | sequence2[7];
  int seq2b = sequence2[8] << 7 | sequence2[9] << 6 | sequence2[10] << 5 | sequence2[11] << 4 | sequence2[12] << 3 | sequence2[13] << 2 | sequence2[14] << 1 | sequence2[15];


  int seq3a = sequence3[0] << 7 | sequence3[1] << 6 |  sequence3[2] << 5 | sequence3[3] << 4  | sequence3[4] << 3  | sequence3[5] << 2  | sequence3[6] << 1  | sequence3[7];
  int seq3b = sequence3[8] << 7 | sequence3[9] << 6 | sequence3[10] << 5 | sequence3[11] << 4 | sequence3[12] << 3 | sequence3[13] << 2 | sequence3[14] << 1 | sequence3[15];


  int seq4a = sequence4[0] << 7 | sequence4[1] << 6 |  sequence4[2] << 5 | sequence4[3] << 4  | sequence4[4] << 3  | sequence4[5] << 2  | sequence4[6] << 1  | sequence4[7];
  int seq4b = sequence4[8] << 7 | sequence4[9] << 6 | sequence4[10] << 5 | sequence4[11] << 4 | sequence4[12] << 3 | sequence4[13] << 2 | sequence4[14] << 1 | sequence4[15];



  //Serial.print(seq1a,BIN);
  //Serial.println(seq1b,BIN);
  
  lc.setLed(0,cursorrow,7-cursorcol, true);
  
  lc.setColumn(0,7,seq1a);
  lc.setColumn(0,6,seq1b);
  lc.setColumn(0,5,seq2a);
  lc.setColumn(0,4,seq2b);
  
  lc.setColumn(0,3,seq3a);
  lc.setColumn(0,2,seq3b);
  
  lc.setColumn(0,1,seq4a);
  lc.setColumn(0,0,seq4b);

  
}

void printStep(int sequence, int rowtoprint, int coltoprint, boolean state){
  lc.setLed(0,rowtoprint,ledrowmap[sequence][coltoprint],state);
}

