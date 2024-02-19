#ifndef print_h
#define print_h
#include "Arduino.h"
class print{
  public:
  void rahmen();
  void krone(int spieler);
  void symbol(int translateX, int translateY,int spieler);
  void setup();
  void clear();
  private:
  int translateToMatrixCoords(int translateCoordinate);
  void backSlach(int x,int y, int lang);
  void slach(int x,int y, int lang);
  void printLed(int x, int y);
};
#endif
