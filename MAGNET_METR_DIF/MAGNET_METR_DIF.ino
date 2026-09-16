#include<differential_amplifier.h>
/*
    Parameter:      Reference:
    DEFAULT         VCC
    EXTERNAL        External Reference (REF)
    INTERNAL1V024   Internal 1.024 volts
    INTERNAL2V048   Internal 2.048 volts
    INTERNAL4V096   Internal 4.096 volts
  
  Citlivost(zesileni): B=0 -> 2,5V, max. rozsah A/D 4.096V
  senzor 5mV/G
  10 rozsahu k dispozici:
  Ind. rozsah +/-  nasobitel
  -9    2,50V       x 3,6 
  -7    2,25V       x 4
  -5    2,00V       x 4,5 
  -3    1,75V       x 5,14
  -1    1,50V       x 6 
  1     1,25V       x 7,2
  3     1,00V       x 9
  5     0,75V       x 12
  7     0,50V       x 18
  9     0,25V       x 36
*/
const float zesileni[10] = {3.6, 4, 4.5, 5.14, 6, 7.2, 9, 12, 18, 36};
const int zesLed[10] = {-9, -7, -5, -3, -1, 1, 3, 5, 7, 9};
float value0 = 2.4;  //hodnota napeti odpovidajici 0T  (predpokladana hodnota)
float value1 = 2;  //hodnota napeti odpovidajici vystupu DAC (prepokladan hodnota)
int zesind = 0; //vychozi zesileni (index)
int daci = 25; // nastaveni DAC prevodniku, střed
// *************************************************
// Zhasnutí všech LED
void LedOff() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}
// *************************************************
// Rozsvícení všech LED
void LedOn() {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}
// *************************************************
// Testování LED - probliknutí
void ledTest(){
   LedOn();
   delay(200);
   LedOff();
   delay(200);
   LedOn();
   delay(200);
   LedOff();
   delay(200);
   LedOn();
   delay(200);
   LedOff();

}
// *************************************************
// Aktualizace LED Bargrafu a zároveň výpis na RS232
// @param index rozsah -9 až +9 
// 9 - - - | x0x | - -  -9 
void LedOutput(int index) {
  switch (index) {
    case 0:
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      Serial.print(F("- - - - | x x | - - - -"));
    break;
    case 1:
      digitalWrite(7, HIGH);
      Serial.print(F("- - - - | x - | - - - -"));
    break;
    case 2:
      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      Serial.print(F("- - - x | x - | - - - -"));
    break;
    case 3:
      digitalWrite(6, HIGH);
      Serial.print(F("- - - x | - - | - - - -"));
    break;
    case 4:
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      Serial.print(F("- - x x | - - | - - - -"));
    break;
    case 5:
      digitalWrite(5, HIGH);
      Serial.print(F("- - x - | - - | - - - -"));
    break;
    case 6:
      digitalWrite(5, HIGH);
      digitalWrite(3, HIGH);
      Serial.print(F("- x x - | - - | - - - -"));
    break;
    case 7:
      digitalWrite(3, HIGH);
      Serial.print(F("- x - - | - - | - - - -"));
    break;
    case 8:
      digitalWrite(3, HIGH);
      digitalWrite(2, HIGH);
      Serial.print(F("x x - - | - - | - - - -"));
    break;
    case 9:
      digitalWrite(2, HIGH);
      Serial.print(F("x - - - | - - | - - - -"));
    break;
    case -1:
      digitalWrite(8, HIGH);
      Serial.print(F("- - - - | - x | - - - -"));
    break;
    case -2:
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      Serial.print(F("- - - - | - x | x - - -"));
    break;
    case -3:
      digitalWrite(9, HIGH);
      Serial.print(F("- - - - | - - | x - - -"));
    break;
    case -4:
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      Serial.print(F("- - - - | - - | x x - -"));
    break;
    case -5:
      digitalWrite(10, HIGH);
      Serial.print(F("- - - - | - - | - x - -"));
    break;
    case -6:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      Serial.print(F("- - - - | - - | - x x -"));
    break;
    case -7:
      digitalWrite(11, HIGH);
      Serial.print(F("- - - - | - - | - - x -"));
    break;
    case -8:
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      Serial.print(F("- - - - | - - | - - x x"));
    break;
    case -9:
      digitalWrite(12, HIGH);
     Serial.print(F("- - - - | - - | - - - x"));
    break;    
    default: 
      digitalWrite(13, HIGH);
      Serial.print(F("- -prekrocen rozsah- -"));
    break; 

  }
}
// *************************************************
// Nastavení citlivosti
// modifikuje zesind - index pro nastavení zesílení
// ovládání: stisk tlačítka, držet dokud nesvítí  LED
// odpovídající danému zesílení. Po RST zesind=0;
void citlivost() {
  LedOff();
  LedOutput(zesLed[zesind]);
  Serial.print(F(" Zes: "));
  Serial.println(zesind);
  while (!digitalRead(A7)) {
    if (!digitalRead(A7)) {
      if (zesind < 9 ) {
        zesind = zesind+1;
        } else {
        zesind = 0;
      };
      LedOff();
      LedOutput(zesLed[zesind]);
      Serial.print(F(" Zes: "));
      Serial.println(zesind);
      delay(1000);
    }
  }
}
// *************************************************
// SETUP systému, nastavení prostředí (začátek programu)
void setup() {
  // put your setup code here, to run once:
  int raw = 0;
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5,LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9,LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(A7,INPUT_PULLUP);
  // nastaveni PIN dokonceno
  analogReference(INTERNAL4V096);   //reference 4,096V
  analogReadResolution(12); // Resolution = 10, 11 or 12 Bit
  //možnost nastavit DAC pro diferenciální mód. 
  //Viz https://wolles-elektronikkiste.de/lgt8f328p-lqfp32-boards
  pinMode(DAC0, ANALOG);     
  analogWrite(DAC0, daci); // 0...255
  Serial.begin(9600);
  delay(1000);
  ledTest();  //probliknutí LED
  Serial.println(F("Boot... O.K."));
  while ((abs(raw-2048))>40) {    //smyčka nastavi offset sondy
    digitalWrite(13, HIGH);   // priznak nastavení, blikáme
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    raw = analogDiffRead(A0,A1,GAIN_1); // GAIN_x mit x = 1, 8, 16, 32
    Serial.print(F("RAW hodnota A0-A1: "));
    Serial.print(raw);
    Serial.print(F(" at DAC index: "));
    Serial.println(daci);
    if (raw>2048) {      //upravime DAC index
      daci = daci + 1;
      if (daci>254) daci=254;
    } else {
      daci = daci - 1;
      if (daci<1) daci=1;
    }
    analogWrite(DAC0, daci); // 0...255
  }

  Serial.println(F("DAC prevodnik nastaven"));
  raw = analogDiffRead(A0,A1,GAIN_1); // GAIN_x mit x = 1, 8, 16, 32
  Serial.println(raw);
  value0 = raw / 1024.0; // considers resolution, reference and gain
  Serial.print(F("Napeti nuloveho pole po korekci: "));
  Serial.print(value0,3);
  Serial.println(F(" V"));
  delay(1000);
}
// *************************************************
// Hlavní smyčka
void loop() {
  // put your main code here, to run repeatedly:
  delay(100); //cekani - uprava rychlosti smycky
  LedOff(); //zhasne LED - ctení AD bez rušení
  delay(1); //pauza, ustaleni zdroje
  int adcValue = analogDiffRead(A0,A1,GAIN_1); // GAIN_x mit x = 1, 8, 16, 32
  float voltage = (adcValue / 1024.0)- value0;  // Voltage calculation
  LedOutput(voltage * zesileni[zesind]); // aktualizace LED,bargraf i na RS232
  Serial.print(F(" Zes: ")); // tisky hodnot
  Serial.print(zesind);
  Serial.print(F("  Analog Value: "));
  Serial.print(adcValue);
  Serial.print(F("  Voltage [V]: "));
  Serial.println(voltage,3);
  if (!digitalRead(A7)) citlivost(); //obsluha tlacitka    
}
