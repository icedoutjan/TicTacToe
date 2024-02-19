#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C by Frank de Brabander
#include <Wire.h>
#include <Keypad.h>      //Keypad by Mark Stanley,...
#include "print.h"



class check{
  public:
  void eingabe();
  void kreutz();
  void achsen();
  //int getArray(int array[][]);
  //private:
  //int array[3][3];
};

//Debug
  //bool debug = false;

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

// LCD Display
  LiquidCrystal_I2C lcd(0x27, 16, 2);

// setup the print class
  print print;

// setup the check class
  check check;
void setup() {
  Serial.begin(9600);
  // LCD Setup
  lcd.init();
  lcd.backlight();

  // Matrix Setup
  print.setup();
}

void loop() {
  for (int xTemp = 0; xTemp < 3; xTemp++) {  //spiel feldzurücksetzen
    for (int yTemp = 0; yTemp < 3; yTemp++) {
      brett[xTemp][yTemp] = 0;
    }
  }
  gewonnen = false;   
  lcd.clear();       // zum aufräumen nach der letzten runde
  print.clear();  // zum aufräumen nach der letzten runde

  print.rahmen(); 

  for (int runde = 1; runde <= 9; runde++) {  //main spiel
    
    spielername = playerSwitch(runde); //variable für den entsprecheden spieler setzen 
    
    lcdPrint("warte auf eingabe");

    check.eingabe();  // eingabe bekommen und prüfen ob das feld verfügbar ist 
    check.achsen();  // überprüfen ob auf der x oder y achse drei gleiche zeichen neben einander sind
    check.kreutz();   // überprüfen ob quer gewonnen wurde
    
    print.symbol(x, y, player); //gibt den letzen zug aus bassierend auf der wiederholung der for schleife 
    
    if (gewonnen) {  // prüfen ob der spieler mit dem letzten zug einen sieg erziehlt hat 
      lcdPrint("hat gewonnen");
      print.clear();
      print.krone(player);
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
       
        KeyAsInt = (int)key;  //char to ascii
        
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
void check::eingabe() {
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
    break;
  }
}
void check::achsen() {
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
void check::kreutz() {
  if (brett[1][1] == player) {  // es wird nur weiter getestet wenn dem aktuellen spieler das mittlere feld gehört da jede runde geprüft wird 
    if ((brett[0][2] && brett[2][0] == player) | (brett[2][2] && brett[0][0] == player)) {
      gewonnen = true; 
    }
  }
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
void mainTikTakToe(){
  for (int xTemp = 0; xTemp < 3; xTemp++) {  //spiel feldzurücksetzen
    for (int yTemp = 0; yTemp < 3; yTemp++) {
      brett[xTemp][yTemp] = 0;
    }
  }
  gewonnen = false;   
  lcd.clear();       // zum aufräumen nach der letzten runde
  print.clear();  // zum aufräumen nach der letzten runde

  print.rahmen(); 

  for (int runde = 1; runde <= 9; runde++) {  //main spiel
    
    spielername = playerSwitch(runde); //variable für den entsprecheden spieler setzen 
    
    lcdPrint("warte auf eingabe");

    check.eingabe();  // eingabe bekommen und prüfen ob das feld verfügbar ist 
    check.achsen();  // überprüfen ob auf der x oder y achse drei gleiche zeichen neben einander sind
    check.kreutz();   // überprüfen ob quer gewonnen wurde
    
    print.symbol(x, y, player); //gibt den letzen zug aus bassierend auf der wiederholung der for schleife 
    
    if (gewonnen) {  // prüfen ob der spieler mit dem letzten zug einen sieg erziehlt hat 
      lcdPrint("hat gewonnen");
      print.clear();
      print.krone(player);
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
