/* Copyright (c) 2017 by Sebastian Bleuel - subIT (https://github.com/subIT1) */

/* -Includes- */
#include <LiquidCrystal.h>
#include <dht11.h>
#include <IRremote.h>
#include <SPI.h>
#include <MFRC522.h>

/* -Relais- */
#define relais1 22
#define relais2 23
#define relais3 24
#define relais4 25

/* -Ultrasoniclsensor- */
#define trigger 7
#define echo 6
int duration = 0;
int distance = 0;
int alarmUltrasonic = 0;

/* -Timing-Control- */
long start0 = 0;
long start1 = 0;

/* -AlarmSiren- */
long durationAlarm = 0;
int alarm = 0;
int triggered = 0;
#define buzzerSound A7
String inputPass;

/* -LCD1- */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* -LCD2- */
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* -LCD3- */
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* -Temp-/Humiditysensor- */
dht11 DHT;
#define DHT11_sensor 42

/* -RemoteControl- */
#define RECV_sensor 40
IRrecv irrecv(RECV_sensor);
decode_results results;
//int alarmStatusInt = 0;
#define alarmStatus 46

/* -PasswortShield- */
#define passShield 38

/* -MagnetSensor- */
#define magnet1 32

/* -RFID- */
#define SS_PIN 53
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);
long card_1 =  KEY_Value; //should be added
long card_2 =  KEY_Value; //should be added
long key_1 =  KEY_Value; //should be added
long key_2 =  KEY_Value; //should be added

/* -Passwort- */
String passwortAlarm = "YOUR_PASSWORT"; //should be added


//----------------------
//-----END-INCLUDES-----
//----------------------

void setup() {

  Serial.begin (9600);
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  SPI.begin();
  mfrc522.PCD_Init();
  //In-/Outputs
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relais1, OUTPUT);
  pinMode(relais2, OUTPUT);
  pinMode(relais3, OUTPUT);
  pinMode(relais4, OUTPUT);
  pinMode(alarmStatus, OUTPUT);
  pinMode(magnet1, INPUT);

  //AlarmIdentification
  digitalWrite(alarmStatus, LOW)
  digitalWrite(relais1, HIGH);
  digitalWrite(relais2, HIGH);
  digitalWrite(relais3, HIGH);
  digitalWrite(relais4, HIGH);

  startScreen();

}

//----------------------
//------END-SETUP-------
//----------------------

void loop() {
  //Temperatur_Humidity();
  //alarmStart();
  //RemoteControl();
  //MagnetSensor();
  //PasswortShield();
  //UltrasonicSensor();
  //RFID();

  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 0) { //Activ
    RemoteControl();
    //MagnetSensor();
    //UltrasonicSensor();
    RFID();
  } else if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 1) { //Inactiv
    RFID();
    RemoteControl();
  }



}
//----------------------
//-------END-LOOP-------
//----------------------

void startScreen() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.println("Welcome!          ");
  lcd.setCursor(3, 1);
  lcd.println("Smart-Home           ");
}
//----------------------
//------END-Start-------
//----------------------

void alarmStart() {
  Serial.println("Alarm");

  //Siren
  digitalWrite(relais4, LOW);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.println(" ALARM!                ");
  lcd.setCursor(2, 1);
  lcd.println(" Time-Date            ");
  delay(5000);
  digitalWrite(relais4, HIGH);
  alarm = 0;
  triggered = 1;
  start0 = 0;
  start1 = 0;

  digitalWrite(relais1, HIGH);
  digitalWrite(relais2, HIGH);
  digitalWrite(relais3, HIGH);

  while (triggered == 1) {
    digitalWrite(relais4, LOW);
    digitalWrite(relais3, LOW);
    delay(900);
    digitalWrite(relais3, HIGH);
    delay(900);
    digitalWrite(relais2, LOW);
    digitalWrite(relais3, LOW);
    delay(900);
    digitalWrite(relais3, HIGH);
    digitalWrite(relais2, HIGH);
    delay(900);
    digitalWrite(relais3, LOW);
    delay(900);
    digitalWrite(relais3, HIGH);
    digitalWrite(relais4, HIGH);
  }

}

void AlarmON() {

  digitalWrite(alarmStatus, LOW);
  digitalWrite(relais1, LOW);
  digitalWrite(relais2, LOW);
  digitalWrite(relais3, LOW);
  digitalWrite(relais4, LOW);
  digitalWrite(magnet1, LOW);
  digitalWrite(trigger, LOW);
  digitalWrite(echo, LOW);
}

void AlarmOFF() {

  digitalWrite(alarmStatus, HIGH);
  digitalWrite(relais1, HIGH);
  digitalWrite(relais2, HIGH);
  digitalWrite(relais3, HIGH);
  digitalWrite(relais4, HIGH);
  digitalWrite(magnet1, HIGH);
  digitalWrite(trigger, HIGH);
  digitalWrite(echo, HIGH);
}
//----------------------
//------END-ALARM-------
//----------------------

void UltrasonicSensor() {
  Serial.println("Ultrasonic");

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance >= 500 || distance <= 0) {
  }
  else {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Ultrasonicsens.");
    lcd.setCursor(6, 1);
    lcd.print(distance);
    lcd.println(" cm             ");

    if (distance <= 20) {
      digitalWrite(relais2, LOW);
      digitalWrite(relais3, LOW);
      delay(1000);
    }

  }

  if (alarmUltrasonic == 0) {
    start0 = millis();
  } else if (alarmUltrasonic == 1) {
    start1 = millis();
  }
  if (digitalRead(relais2) == LOW && digitalRead(relais3) == LOW) {
    alarmUltrasonic = 1;

    if (start0 != 0 && start1 != 0) {

      if ((start1 - start0) > 5000) {
        alarmUltrasonic = 0;
        start0 = 0;
        start1 = 0;
      } else if ((start1 - start0) <= 5000) {
        alarmUltrasonic = 4;

        if (alarmUltrasonic = 4) {
          alarmUltrasonic = 0;
          triggered = 1;
          start0 = 0;
          start1 = 0;
          alarmStart();
        }
      }
    }
  }
  delay(1000);
}
//----------------------
//----END-Ultrasonic---
//----------------------

void Temperatur_Humidity() {
  Serial.println("Temp/Air");

  int chk = DHT.read(DHT11_sensor);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Airhumid.:");
  lcd.println(DHT.humidity, 1);
  lcd.setCursor(0, 1);
  lcd.println("Temperatur:");
  lcd.println(DHT.temperature, 1);
  delay(10000);
}
//----------------------
//-----END-Air/Temp----
//----------------------


void RemoteControl() {

  int auswahl = 0;
  int laengeEingabe = 0;

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
    delay(500);
  }

  switch (results.value) {

    //BREAK beachten !!
    case 0xFF629D: Serial.println(" FORWARD");
      UltrasonicSensor();
      break;
    case 0xFF22DD: Serial.println(" LEFT");
      alarmStart();
      break;
    case 0xFF02FD: Serial.println(" -OK-");
      lcd.clear();
      lcd.println(inputPass);
      delay(200);
      break;
    case 0xFFC23D: Serial.println(" RIGHT");
      Temperatur_Humidity();
      break;
    case 0xFFA857: Serial.println(" REVERSE");
      RFID();
      break;

    case 0xFF6897: Serial.println(" 1");
      auswahl = 1;
      inputPass = inputPass + "1";
      delay(200);
      break;

    case 0xFF9867: Serial.println(" 2");
      auswahl = 2;
      inputPass = inputPass + "2";
      delay(200);
      break;

    case 0xFFB04F: Serial.println(" 3");
      auswahl = 3;
      inputPass = inputPass + "3";
      delay(200);
      break;

    case 0xFF30CF: Serial.println(" 4");
      auswahl = 4;
      inputPass = inputPass + "4";
      delay(200);
      break;

    case 0xFF18E7: Serial.println(" 5");
      auswahl = 5;
      inputPass = inputPass + "5";
      delay(200);
      break;

    case 0xFF7A85: Serial.println(" 6");
      auswahl = 6;
      inputPass = inputPass + "6";
      delay(200);
      break;

    case 0xFF10EF: Serial.println(" 7");
      auswahl = 7;
      inputPass = inputPass + "7";
      delay(200);
      break;

    case 0xFF38C7: Serial.println(" 8");
      auswahl = 8;
      inputPass = inputPass + "8";
      delay(200);
      break;

    case 0xFF5AA5: Serial.println(" 9");
      auswahl = 9;
      inputPass = inputPass + "9";
      delay(200);
      break;

    case 0xFF4AB5: Serial.println(" 0");
      auswahl = 10;
      inputPass = inputPass + "0";
      delay(200);
      break;

    case 0xFF42BD: Serial.println(" *");
      if (passwortAlarm.equals(inputPass)) {
        if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) {
          Sounde(buzzerSound, 100, 800);
          delay(810);
          Sounde(buzzerSound, 1000, 800);
          noSounde(buzzerSound);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.println("Alarmsystem              ");
          lcd.setCursor(0, 1);
          lcd.println("Status:                  ");
          lcd.setCursor(11, 1);
          lcd.println("ACTIV                    ");
          inputPass = "";
          digitalWrite(relais2, LOW);
          delay(5000);
          digitalWrite(relais2, HIGH);
          digitalWrite(alarmStatus, HIGH);
          AlarmON();


        } else if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt == 1) {
          Sounde(buzzerSound, 100, 800);
          delay(810);
          Sounde(buzzerSound, 1000, 800);
          noSounde(buzzerSound);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.println("Alarmsystem               ");
          lcd.setCursor(0, 1);
          lcd.println("Status:                   ");
          lcd.setCursor(9, 1);
          lcd.println("INACTIV                   ");
          inputPass = "";
          digitalWrite(relais3, LOW);
          delay(5000);
          digitalWrite(relais3, HIGH);
          digitalWrite(alarmStatus, LOW);//alarmStatusInt = 0;
          AlarmOFF();
        }
        delay(200);
      }
      else {
        Sounde(buzzerSound, 100, 800);
        delay(810);
        Sounde(buzzerSound, 300, 800);
        noSounde(buzzerSound);
        inputPass = "";
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(5, 1);
        lcd.println("Wrong!          ");
        digitalWrite(relais2, LOW);
        digitalWrite(relais3, LOW);
        delay(3000);
        digitalWrite(relais2, HIGH);
        digitalWrite(relais3, HIGH);
        //alarmStart();
        delay(200);
        startScreen();
      }
      break;

    case 0xFF52AD: Serial.println(" #");
      inputPass = "";
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.println("Alarmsystem                    ");
      lcd.setCursor(0, 1);
      lcd.println("Status:                        ");
      if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt == 1) { 
        lcd.setCursor(11, 1);
        lcd.println("ACTIV           ");
      } else if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { 
        lcd.setCursor(9, 1);
        lcd.println("INACTIV         ");
      }
      noSounde(buzzerSound);
      delay(200);
      break;

      results.value = 100;

  }
  int anz = 0;
  if (results.value == 0xFF629D || results.value == 0xFF22DD || results.value == 0xFF02FD || results.value == 0xFFC23D || results.value == 0xFFA857 || results.value == 0xFF6897 || results.value == 0xFF9867 || results.value == 0xFFB04F ||
      results.value == 0xFF30CF || results.value == 0xFF18E7 || results.value == 0xFF7A85 || results.value == 0xFF10EF || results.value == 0xFF38C7 || results.value == 0xFF5AA5 || results.value == 0xFF42BD || results.value == 0xFF4AB5 ||
      results.value == 0xFF52AD) {
    laengeEingabe = inputPass.length();
    anz++;
    if (laengeEingabe > 0) {
      if (laengeEingabe == 1) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(8, 1);
        lcd.println("*               ");
      } else if (laengeEingabe == 2) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(8, 1);
        lcd.println("**              ");
      } else if (laengeEingabe == 3) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(7, 1);
        lcd.println("***             ");
      } else if (laengeEingabe == 4) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(6, 1);
        lcd.println("****            ");
      } else if (laengeEingabe == 5) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(6, 1);
        lcd.println("*****           ");
      } else if (laengeEingabe == 6) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(5, 1);
        lcd.println("******          ");
      } else if (laengeEingabe == 7) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(5, 1);
        lcd.println("*******         ");
      } else if (laengeEingabe == 8) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(4, 1);
        lcd.println("********        ");
      } else if (laengeEingabe == 9) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(4, 1);
        lcd.println("*********       ");
      } else if (laengeEingabe == 10) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(3, 1);
        lcd.println("**********      ");
      } else if (laengeEingabe == 11) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:        ");
        lcd.setCursor(3, 1);
        lcd.println("Maximum: 10      ");
      }
    }
  }
  if (anz > 10) {
    inputPass = "";
  }

  noSounde(buzzerSound);

}
//----------------------
//--END-RemoteControl---
//----------------------

void PasswortShield() {
  Serial.println("PasswortShield");

  if (digitalRead(passShield) == LOW) {
    //alarmStart();
    Serial.println("yes");
  } else {
    Serial.println("no");
  }
  delay(1000);

}
//----------------------
//--END-PasswortShield--
//----------------------


void MagnetSensor() {
  Serial.println("Magnet");

  if (digitalRead(magnet1) == HIGH) {
  } else if (digitalRead(magnet1) == LOW) {
    alarmStart();
  }

}
//----------------------
//------END-Magnet------
//----------------------


void RFID() {

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  long code = 0, saveCode = 0, secondCode = 0;

  //query of RFID-Values
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    code = ((code + mfrc522.uid.uidByte[i]) * 10);
    saveCode = ((code + mfrc522.uid.uidByte[i]) * 10);
  }
  //Event at card 1
  if (code == card_1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("identification:      ");
    lcd.setCursor(5, 1);
    lcd.println("NAME                 ");
    delay(2000);
    changeAlarm();
    
    //Event at card 2
  } else if (code == card_2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("identification:      ");
    lcd.setCursor(3, 1);
    lcd.println("NAME                 ");
    delay(2000);
    changeAlarm();

    //Event at key 1
  } else if (code == key_1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("identification:      ");
    lcd.setCursor(3, 1);
    lcd.println("NAME             ");
    delay(2000);
    changeAlarm();

    //Event at key 2
  } else if (code == key_2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("identification:      ");
    lcd.setCursor(3, 1);
    lcd.println("NAME                 ");
    delay(2000);
    changeAlarm();

  } else {
    delay(2000);
    Serial.println("no agreement");
    startScreen();
  }
  delay(2000);

  for (byte t = 0; t < mfrc522.uid.size; t++) {
    secondCode = ((secondCode + mfrc522.uid.uidByte[t]) * 10);
  }

  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) { //ACTIV
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("Alarmsystem is     ");
    lcd.setCursor(3, 1);
    lcd.println("now INACTIV          ");
    digitalWrite(alarmStatus, LOW);
    delay(3000);
    startScreen();

  }
  if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { //InACTIV
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("Alarmsystem is      ");
    lcd.setCursor(4, 1);
    lcd.println("now ACTIV       ");
    digitalWrite(alarmStatus, HIGH);
    delay(3000);
    startScreen();
  }



  if (code != 0 && secondCode != 0) {
    if (code == secondCode) {
      if (secondCode == card_1 || secondCode == key_1 && secondCode == saveCode) {
        if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) { //ACTIV
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.println("Alarmsystem is     ");
          lcd.setCursor(3, 1);
          lcd.println("now INACTIV          ");
          digitalWrite(alarmStatus, LOW);
          delay(3000);
          startScreen();

        }
        if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { //InACTIV
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.println("Alarmsystem is      ");
          lcd.setCursor(4, 1);
          lcd.println("now ACTIV       ");
          digitalWrite(alarmStatus, HIGH);
          delay(3000);
          startScreen();
        }
      } else {
        startScreen();
      }
    } else {
      startScreen();
    }
  } else {
    startScreen();
  }
  code = 0;
  saveCode = 0;
  secondCode = 0;

}

void changeAlarm() {

  Serial.println("Status: " + digitalRead(alarmStatus));
  //Anzeige des Status
  if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt = 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.println("Alarmsystem          ");
    lcd.setCursor(3, 1);
    lcd.println("ACTIVATE?        ");
    //AlarmON();
    delay(2000);
  }
  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.println("Alarmsystem          ");
    lcd.setCursor(2, 1);
    lcd.println("DEACTIVATE?        ");
    //AlarmOFF();
    delay(2000);
  }
}
//----------------------
//-------END-RFID-------
//----------------------


