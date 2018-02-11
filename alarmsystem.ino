

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

/* -Ultraschallsensor- */
#define trigger 7
#define echo 6
int dauer = 0;
int entfernung = 0;
int alarmUltraschall = 0;

/* -Zeitliche-Kontrolle- */
long start0 = 0;
long start1 = 0;

/* -AlarmSirene- */
long dauerAlarm = 0;
int alarm = 0;
int ausgeloest = 0;
#define buzzerTon A7
String eingabePass;

/* -LCD1- */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* -LCD2- */
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* -LCD3- */
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* -Temp-/Feuchtigkeitssensor- */
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
long card_1 = KEY_Value; //should be added
long card_2 = KEY_Value; //should be added
long key_1 = KEY_Value; //should be added
long key_2 = KEY_Value; //should be added


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

  //AlarmIdentifizierung
  digitalWrite(alarmStatus, LOW);//Inaktiv
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
  //Temperatur_Feuchtigkeit();
  //alarmStart();
  //RemoteControl();
  //MagnetSensor();
  //PasswortShield();
  //Ultraschallsensor();
  //RFID();

  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 0) { //Aktiv
    RemoteControl();
    //MagnetSensor();
    //Ultraschallsensor();
    RFID();
  } else if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 1) { //Inaktiv
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
  lcd.println("Willkommen!          ");
  lcd.setCursor(3, 1);
  lcd.println("Smart-Home           ");
}
//----------------------
//------END-Start-------
//----------------------

void alarmStart() {
  Serial.println("Alarm");

  //Sirene
  digitalWrite(relais4, LOW);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.println(" ALARM!                ");
  lcd.setCursor(2, 1);
  lcd.println(" Zeit-Datum            ");
  delay(5000);
  digitalWrite(relais4, HIGH);
  alarm = 0;
  ausgeloest = 1;
  start0 = 0;
  start1 = 0;

  digitalWrite(relais1, HIGH);
  digitalWrite(relais2, HIGH);
  digitalWrite(relais3, HIGH);

  while (ausgeloest == 1) {
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

void AlarmAN() {

  digitalWrite(alarmStatus, LOW);
  digitalWrite(relais1, LOW);
  digitalWrite(relais2, LOW);
  digitalWrite(relais3, LOW);
  digitalWrite(relais4, LOW);
  digitalWrite(magnet1, LOW);
  digitalWrite(trigger, LOW);
  digitalWrite(echo, LOW);
}

void AlarmAUS() {

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

void Ultraschallsensor() {
  Serial.println("Ultraschall");

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);
  entfernung = (dauer / 2) / 29.1;

  if (entfernung >= 500 || entfernung <= 0) {
  }
  else {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Ultraschallsens.");
    lcd.setCursor(6, 1);
    lcd.print(entfernung);
    lcd.println(" cm             ");

    if (entfernung <= 20) {
      digitalWrite(relais2, LOW);
      digitalWrite(relais3, LOW);
      delay(1000);
    }

  }

  if (alarmUltraschall == 0) {
    start0 = millis();
  } else if (alarmUltraschall == 1) {
    start1 = millis();
  }
  if (digitalRead(relais2) == LOW && digitalRead(relais3) == LOW) {
    alarmUltraschall = 1;

    if (start0 != 0 && start1 != 0) {

      if ((start1 - start0) > 5000) {
        alarmUltraschall = 0;
        start0 = 0;
        start1 = 0;
      } else if ((start1 - start0) <= 5000) {
        alarmUltraschall = 4;

        if (alarmUltraschall = 4) {
          alarmUltraschall = 0;
          ausgeloest = 1;
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
//----END-ULTRASCHALL---
//----------------------

void Temperatur_Feuchtigkeit() {
  Serial.println("Temp/Luft");

  int chk = DHT.read(DHT11_sensor);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Luftfeuch.:");
  lcd.println(DHT.humidity, 1);
  lcd.setCursor(0, 1);
  lcd.println("Temperatur:");
  lcd.println(DHT.temperature, 1);
  delay(10000);
}
//----------------------
//-----END-Luft/Temp----
//----------------------


void RemoteControl() {

  String passwortAlarm = "426875";

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
      Ultraschallsensor();
      break;
    case 0xFF22DD: Serial.println(" LEFT");
      alarmStart();
      break;
    case 0xFF02FD: Serial.println(" -OK-");
      lcd.clear();
      lcd.println(eingabePass);
      delay(200);
      break;
    case 0xFFC23D: Serial.println(" RIGHT");
      Temperatur_Feuchtigkeit();
      break;
    case 0xFFA857: Serial.println(" REVERSE");
      RFID();
      break;

    case 0xFF6897: Serial.println(" 1");
      auswahl = 1;
      eingabePass = eingabePass + "1";
      delay(200);
      break;

    case 0xFF9867: Serial.println(" 2");
      auswahl = 2;
      eingabePass = eingabePass + "2";
      delay(200);
      break;

    case 0xFFB04F: Serial.println(" 3");
      auswahl = 3;
      eingabePass = eingabePass + "3";
      delay(200);
      break;

    case 0xFF30CF: Serial.println(" 4");
      auswahl = 4;
      eingabePass = eingabePass + "4";
      delay(200);
      break;

    case 0xFF18E7: Serial.println(" 5");
      auswahl = 5;
      eingabePass = eingabePass + "5";
      delay(200);
      break;

    case 0xFF7A85: Serial.println(" 6");
      auswahl = 6;
      eingabePass = eingabePass + "6";
      delay(200);
      break;

    case 0xFF10EF: Serial.println(" 7");
      auswahl = 7;
      eingabePass = eingabePass + "7";
      delay(200);
      break;

    case 0xFF38C7: Serial.println(" 8");
      auswahl = 8;
      eingabePass = eingabePass + "8";
      delay(200);
      break;

    case 0xFF5AA5: Serial.println(" 9");
      auswahl = 9;
      eingabePass = eingabePass + "9";
      delay(200);
      break;

    case 0xFF4AB5: Serial.println(" 0");
      auswahl = 10;
      eingabePass = eingabePass + "0";
      delay(200);
      break;

    case 0xFF42BD: Serial.println(" *");
      if (passwortAlarm.equals(eingabePass)) {
        if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) {
          tone(buzzerTon, 100, 800);
          delay(810);
          tone(buzzerTon, 1000, 800);
          noTone(buzzerTon);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.println("Alarmanlage              ");
          lcd.setCursor(0, 1);
          lcd.println("Status:                  ");
          lcd.setCursor(11, 1);
          lcd.println("AKTIV                    ");
          eingabePass = "";
          digitalWrite(relais2, LOW);
          delay(5000);
          digitalWrite(relais2, HIGH);
          digitalWrite(alarmStatus, HIGH);
          AlarmAN();


        } else if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt == 1) {
          tone(buzzerTon, 100, 800);
          delay(810);
          tone(buzzerTon, 1000, 800);
          noTone(buzzerTon);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.println("Alarmanlage               ");
          lcd.setCursor(0, 1);
          lcd.println("Status:                   ");
          lcd.setCursor(9, 1);
          lcd.println("INAKTIV                   ");
          eingabePass = "";
          digitalWrite(relais3, LOW);
          delay(5000);
          digitalWrite(relais3, HIGH);
          digitalWrite(alarmStatus, LOW);//alarmStatusInt = 0;
          AlarmAUS();
        }
        delay(200);
      }
      else {
        tone(buzzerTon, 100, 800);
        delay(810);
        tone(buzzerTon, 300, 800);
        noTone(buzzerTon);
        eingabePass = "";
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.println("Passwort:       ");
        lcd.setCursor(5, 1);
        lcd.println("FALSCH!          ");
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
      eingabePass = "";
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.println("Alarmanlage                    ");
      lcd.setCursor(0, 1);
      lcd.println("Status:                        ");
      if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt == 1) { //negiert
        lcd.setCursor(11, 1);
        lcd.println("AKTIV           ");
      } else if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { //negiert
        lcd.setCursor(9, 1);
        lcd.println("INAKTIV         ");
      }
      noTone(buzzerTon);
      delay(200);
      break;

      results.value = 100;

  }
  int anz = 0;
  if (results.value == 0xFF629D || results.value == 0xFF22DD || results.value == 0xFF02FD || results.value == 0xFFC23D || results.value == 0xFFA857 || results.value == 0xFF6897 || results.value == 0xFF9867 || results.value == 0xFFB04F ||
      results.value == 0xFF30CF || results.value == 0xFF18E7 || results.value == 0xFF7A85 || results.value == 0xFF10EF || results.value == 0xFF38C7 || results.value == 0xFF5AA5 || results.value == 0xFF42BD || results.value == 0xFF4AB5 ||
      results.value == 0xFF52AD) {
    laengeEingabe = eingabePass.length();
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
    eingabePass = "";
  }

  noTone(buzzerTon);

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

  //Abfrage der RFID-Werte
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    code = ((code + mfrc522.uid.uidByte[i]) * 10);
    saveCode = ((code + mfrc522.uid.uidByte[i]) * 10);
  }
  //Ereignis bei Karte 1
  if (code == card_1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Identifizierung:      ");
    lcd.setCursor(5, 1);
    lcd.println("Name                 ");
    delay(2000);
    changeAlarm();
    
    //Ereignis bei Karte 2
  } else if (code == card_2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Identifizierung:      ");
    lcd.setCursor(3, 1);
    lcd.println("Name                 ");
    delay(2000);
    changeAlarm();

    //Ereignis bei Schlüssel 1
  } else if (code == key_1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Identifizierung:      ");
    lcd.setCursor(3, 1);
    lcd.println("Name             ");
    delay(2000);
    changeAlarm();

    //Ereignis bei Schlüssel 2
  } else if (code == key_2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Identifizierung:      ");
    lcd.setCursor(3, 1);
    lcd.println("Name                 ");
    delay(2000);
    changeAlarm();

  } else {
    delay(2000);
    Serial.println("keine Übereinstiummung");
    startScreen();
  }
  delay(2000);

  for (byte t = 0; t < mfrc522.uid.size; t++) {
    secondCode = ((secondCode + mfrc522.uid.uidByte[t]) * 10);
  }

  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) { //Aktiv
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("Alarmanlage ist     ");
    lcd.setCursor(3, 1);
    lcd.println("nun Inaktiv          ");
    digitalWrite(alarmStatus, LOW);
    delay(3000);
    startScreen();

  }
  if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { //Inaktiv
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("Alarmanlage ist      ");
    lcd.setCursor(4, 1);
    lcd.println("nun Aktiv       ");
    digitalWrite(alarmStatus, HIGH);
    delay(3000);
    startScreen();
  }



  if (code != 0 && secondCode != 0) {
    if (code == secondCode) {
      if (secondCode == card_1 || secondCode == key_1 && secondCode == saveCode) {
        if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) { //Aktiv
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.println("Alarmanlage ist     ");
          lcd.setCursor(3, 1);
          lcd.println("nun Inaktiv          ");
          digitalWrite(alarmStatus, LOW);
          delay(3000);
          startScreen();

        }
        if (digitalRead(alarmStatus == LOW)) { //alarmStatusInt == 0) { //Inaktiv
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.println("Alarmanlage ist      ");
          lcd.setCursor(4, 1);
          lcd.println("nun Aktiv       ");
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
    lcd.println("Alarmanlage          ");
    lcd.setCursor(3, 1);
    lcd.println("AKTIVIEREN?        ");
    //AlarmAN();
    delay(2000);
  }
  if (digitalRead(alarmStatus == HIGH)) { //alarmStatusInt = 1) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.println("Alarmanlage          ");
    lcd.setCursor(2, 1);
    lcd.println("DEAKTIVIEREN?        ");
    //AlarmAUS();
    delay(2000);
  }
}
//----------------------
//-------END-RFID-------
//----------------------


