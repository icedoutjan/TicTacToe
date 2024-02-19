#include "print.h"
#include <LedControl.h>  //by Eberhard

// Led Matrix Stuff
  // Datin  = grün
  // CLK    = weis
  // CS     = orange 
  const int translate1 = 3;
  const int translate2 = 6;
  LedControl lc0 = LedControl(2, 3, 4, 1);     // unten rechts
  LedControl lc1 = LedControl(11, 12, 13, 1);  // unten links
  LedControl lc2 = LedControl(8, 9, 10, 1);    // oben  rechts
  LedControl lc3 = LedControl(5, 6, 7, 1);     // oben  links
  // Matrix Array
  LedControl lc[4] = { lc0, lc1, lc2, lc3 };
void print::setup(){
  for (int matrix = 0; matrix < 4; matrix++) {
    lc[matrix].shutdown(0, false);
    lc[matrix].setIntensity(0, 2);
    lc[matrix].clearDisplay(0);
  }
}
void print::clear(){
  for (int matrix = 0; matrix < 4; matrix++) {
    lc[matrix].clearDisplay(0);
  }
}
void print::symbol(int translateX, int translateY ,int spieler){
  switch (spieler) {
    case 1:
      printLed(2 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
      printLed(3 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
      break;
    case 2:
      printLed(2 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
      printLed(3 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
      printLed(2 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
      printLed(3 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
      break;
  }
}
void print::backSlach(int x,int y, int lang){ //gibt einen schrägstrich auf der martix aus 
  lang = lang + x;
  for(;x < lang;x++){
    printLed(x,y);
    y--;
  }
}
void print::slach(int x,int y, int lang){  //gibt einen schrägstrich auf der martix aus 
    lang = lang + x;
  for(;x < lang;x++){
    printLed(x,y);
    y++;
 }
}
void print::krone(int spieler) {
  for (int xTemp = 1; xTemp <= 14; xTemp++) {  // Floor line
    printLed(xTemp, 1);
  }
  for (int yTemp = 1; yTemp <= 14; yTemp++) {  // Site Lines
    printLed(1, yTemp);
    printLed(14, yTemp);
  }

  backSlach(2,13,3);
  backSlach(8,14,4);
  slach(5,12,3);
  slach(12,12,3);
  
  switch(spieler){
    case 1: // gibt eine "1" in der krone aus 
      slach(5,9,3);
      for(int i = 11;i > 3;i--){
        printLed(8,i);
      }
      break;
    case 2: // gibt eine "2" in der krone aus 
      slach(5,9,3);
      backSlach(8,11,2);
      slach(6,5,5);
      for(int i = 5; i < 10;i++){
        printLed(i,4);
      }
    break;
  }
}
void print::printLed(int x, int y) { // funktion damit man vier martixen zu einem feld verbinden kann 
  int xRevers = 15 - x;
  int yRevers = 15 - y;
  if (x <= 7 && y <= 7) {
    lc[0].setLed(0, x, y, true);
  }
  if (x >= 8 && y <= 7) {
    lc[1].setLed(0, x - 8, y, true);
  }
  if (x >= 8 && y >= 8) {
    lc[2].setLed(0, xRevers, yRevers, true);
  }
  if (x <= 7 && y >= 8) {
    lc[3].setLed(0, xRevers - 8, yRevers, true);
  }
}
void print::rahmen() {
  for (int yTemp = 0; yTemp <= 15; yTemp++) {  // Vertikaler Rahmen
    printLed(0, yTemp);
    printLed(5, yTemp);
    printLed(10, yTemp);
    printLed(15, yTemp);
  }
  for (int xTemp = 0; xTemp <= 15; xTemp++) {   // Horizontaler Rahmen
    printLed(xTemp, 0);
    printLed(xTemp, 5);
    printLed(xTemp, 10);
    printLed(xTemp, 15);
  }
}
int print::translateToMatrixCoords(int translateCoordinate) {
  return translateCoordinate * 5;
}
