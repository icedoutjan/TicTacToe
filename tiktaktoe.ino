#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C by Frank de Brabander
#include <Wire.h>
#include <Keypad.h>      //Keypad by Mark Stanley,...
#include <LedControl.h>  //by Eberhard

//Debug
  bool debug = false;

//spiel mechanig variablen
  int player;
  int x;  //achse
  int y;  //achse
  int input;
  int countPlayerFields = 0;
  bool gewonnen = false;
  int brett[3][3] = { 0 };
  String spielername; 

//variablen erweitertes spiel


// Keypad Configuration
// https://www.roboter-bausatz.de/projekte/4x4-tastenfeld-mit-arduino-ansteuern
  const byte ROWS = 4;  // vier Reihen
  const byte COLS = 4;  // vier Spalten
  char keys[ROWS][COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
  };
  byte rowPins[ROWS] = { 53, 52, 51, 50 };  // Pins fuer die Reihen
  byte colPins[COLS] = { 49, 48, 47, 46 };  // Pins fuer die Spalten
  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


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


// LCD Display
  LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  Serial.begin(9600);
  // LCD Setup
  lcd.init();
  lcd.backlight();

  // Matrix Setup
  for (int matrix = 0; matrix < 4; matrix++) {
    lc[matrix].shutdown(0, false);
    lc[matrix].setIntensity(0, 2);
    lc[matrix].clearDisplay(0);
  }
}

void loop() {
  for (int xTemp = 0; xTemp < 3; xTemp++) {  //spiel feldzurücksetzen
    for (int yTemp = 0; yTemp < 3; yTemp++) {
      brett[xTemp][yTemp] = 0;
    }
  }
  gewonnen = false;   
  lcd.clear();       // zum aufräumen nach der letzten runde
  for (int matrix = 0; matrix < 4; matrix++) {
    lc[matrix].clearDisplay(0);
  }  // zum aufräumen nach der letzten runde

  rahmen(); 

  for (int runde = 1; runde <= 9; runde++) {  //main spiel
    
    spielername = playerSwitch(runde); //variable für den entsprecheden spieler setzen 
    if (debug) {
      Serial.println("======");
      Serial.println(spielername);
    }

    lcdPrint("warte auf eingabe");

    eingabCheck();  // eingabe bekommen und prüfen ob das feld verfügbar ist 
    checkAchsen();  // überprüfen ob auf der x oder y achse drei gleiche zeichen neben einander sind
    checkKreuz();   // überprüfen ob quer gewonnen wurde
    
    if (runde % 2 == 0) { //gibt den letzen zug aus bassierend auf der wiederholung der for schleife 
      kreutz(x, y);
    } else {
      kreis(x, y);
    }
    if (gewonnen) {  // prüfen ob der spieler mit dem letzten zug einen sieg erziehlt hat
      if (debug) {
        Serial.print("spiel wurde gewonnen von spieler ");
        Serial.println(player);
      }
      lcdPrint("hat gewonnen");
      for (int matrix = 0; matrix < 4; matrix++) {
        lc[matrix].clearDisplay(0);
      }
      krone(player);
      inPutKeyPad();
      break;
    }
  }
  if (!gewonnen) {  // fals es zu einem unentschieden kommt 
    lcd.clear();
    lcd.println("unedschieden");
    lcd.setCursor(0, 1);
    lcd.println("press any key");
    inPutKeyPad();
  }
}
void lcdPrint(String meldung) { // vereinfacht die ausgabe auf dem lcd display 
  lcd.clear();
  lcd.print(spielername);
  lcd.setCursor(0, 1);
  lcd.println(meldung);
}
int inPutKeyPad() {
  for (;;) {
    int KeyAsInt = 0;
    for (;;) {
      char key = keypad.getKey(); 
      if (key != NO_KEY) {
        if (debug) {
          Serial.print("keypad input als char: ");
          Serial.println(key);
        }
        KeyAsInt = (int)key;  //char to ascii
        if (debug) {
          Serial.print("keypad input als int: ");
          Serial.println(KeyAsInt);
        }
        break;
      }
    }
    //prüfen ob eine zahl eingegeben wurde oder eine ungewünschte eingabe taste gedrückt wurde
    if (!(KeyAsInt >= 49 && KeyAsInt <= 57)) {  //ascii wert von 1-9
      //falsche eingabe dem spieler anzeigen
      lcdPrint("falsche eingabe");
      continue;
    }
    return KeyAsInt;
  }
}
void eingabCheck() {
  for (;;) {
    convertInputToArray(inPutKeyPad());
    //prüfen ob das feld belegt ist
    if (brett[x][y] == 1 | brett[x][y] == 2) {
      lcdPrint("feld belegt");
      continue;
    }
    //nach der funktion bei erfolgreicher eingabe die fehler message entfernen und den namen ausgben
    lcdPrint("");           //felher meldung entfernen
    brett[x][y] = player;  // Safe input to SpielfeldArray
  
    if (debug) {
      // Print Spielfeld aus der Datenbank
      Serial.println("======");
      for (int yTemp = 2; yTemp >= 0; yTemp--) {
        for (int xTemp = 0; xTemp <= 2; xTemp++) {
          Serial.print(brett[xTemp][yTemp]);
        }
        Serial.println("");
      }
      Serial.println("======");
    }
    break;
  }
}
void convertInputToArray(int inputInAscii) {
  switch (inputInAscii) {
    case 49: // 1
      x = 0;
      y = 2;
      break;

    case 50: // 2
      x = 1;
      y = 2;
      break;

    case 51: // 3
      x = 2;
      y = 2;
      break;

    case 52: // 4
      x = 0;
      y = 1;
      break;

    case 53: // 5
      x = 1;
      y = 1;
      break;

    case 54: // 6
      x = 2;
      y = 1;
      break;

    case 55: // 7
      x = 0;
      y = 0;
      break;

    case 56: // 8
      x = 1;
      y = 0;
      break;

    case 57: // 9
      x = 2;
      y = 0;
      break;
  }
}
void checkAchsen() {
  for (int xTemp = 0; xTemp < 3; xTemp++) {
    countPlayerFields = 0;
    for (int yTemp = 0; yTemp < 3; yTemp++) {
      if (brett[xTemp][yTemp] == player) { // Vertikaler check
        countPlayerFields++; 
      } else { break; }
      if (countPlayerFields == 3) {  // countPlayerFields = 3 heist drei felder in einer rheie gehören dem player
        gewonnen = true;
      }
    }
  }
  for (int yTemp = 0; yTemp < 3; yTemp++) {
    countPlayerFields = 0;
    for (int xTemp = 0; xTemp < 3; xTemp++) {
      if (brett[xTemp][yTemp] == player) { // Horizontal check
        countPlayerFields++; 
      } else { break; }
      if (countPlayerFields == 3) {  // countPlayerFields = 3 heist drei felder in einer Spalte gehören dem player
        gewonnen = true;
      }
    }
  }
}
void checkKreuz() {
  if (brett[1][1] == player) {  // es wird nur weiter getestet wenn dem aktuellen spieler das mittlere feld gehört da jede runde geprüft wird 
    if ((brett[0][2] && brett[2][0] == player) | (brett[2][2] && brett[0][0] == player)) {
      gewonnen = true; 
    }
  }
}
void printLed(int x, int y) { // funktion damit man vier martixen zu einem feld verbinden kann 
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
void rahmen() {
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
int translateToMatrixCoords(int translateCoordinate) {
  return translateCoordinate * 5;
}
void kreis(int translateX, int translateY) {  // gibt den kreis aus die paramenter bestimme um wie viele kasten verschoben vom startpunkt
  printLed(2 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
  printLed(3 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
  printLed(2 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
  printLed(3 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
}
void kreutz(int translateX, int translateY) { // gibt das kreutz die paramenter bestimme um wie viele kasten verschoben vom startpunkt
  printLed(2 + translateToMatrixCoords(translateX), 2 + translateToMatrixCoords(translateY));
  printLed(3 + translateToMatrixCoords(translateX), 3 + translateToMatrixCoords(translateY));
}
String playerSwitch(int runde) { // setzt spieler vairablen basierend auf der for schleife 
  if (runde % 2 == 0) {
    player = 1;
    spielername = "spieler 1";
  } else {
    player = 2;
    spielername = "spieler 2";
  }
  return spielername;
}
void krone(int spieler) {
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
void backSlach(int x,int y, int lang){ //gibt einen schrägstrich auf der martix aus 
  lang = lang + x;
  for(;x < lang;x++){
    printLed(x,y);
    y--;
  }
}
void slach(int x,int y, int lang){  //gibt einen schrägstrich auf der martix aus 
    lang = lang + x;
  for(;x < lang;x++){
    printLed(x,y);
    y++;
 }
}
