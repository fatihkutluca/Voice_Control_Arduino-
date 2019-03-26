

/*
  ------------------------------------------------------------------------
                            Akilli Ev
  ------------------------------------------------------------------------
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic1(11, 12);  // An ultrasonic sensor HC-04

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal_I2C lcd1(0x26, 16, 4);

byte aktif[] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B10001,
  B10001,
  B10001,
  B11111
};
byte pasif[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B10101,
  B11111
};
int uzaklik = 0;
int ruzgarValue = 0;
int gunesValue = 0;

Servo kapi1;
Servo kapi2;
Servo garaj;
Servo pencere1;
Servo pencere2;
Servo pencere3;

String voice;
#define role1 22
#define role2 24
#define role3 26
#define role4 28
#define role5 30
#define role6 32
#define role7 34
#define role8 36
#define pir1 0
#define pir2 1
#define pir3 2
#define pir4 3
#define pinBuzzer 13
#define RuzgarsensorPin A0
#define GunessensorPin A10
int sensorValue = 0;
volatile byte alarmDurumu = LOW, alarmAcik = LOW;
boolean RoleDurumu[] = {false, false, false, false, false, false, false, false};
boolean ServoDurumu[] = {false, false, false, false, false, false};

void setup()
{
  lcd.begin();
  lcd.backlight();
  lcd1.begin();
  lcd1.backlight();
  lcd.createChar(0, pasif);
  lcd.createChar(1, aktif);

  Serial2.begin(9600);


  {
    kapi1.attach(5);
    kapi2.attach(6);
    garaj.attach(7);
    pencere1.attach(8);
    pencere2.attach(9);
    pencere3.attach(10);
  }
  {

    pinMode(43, OUTPUT);
    pinMode(45, OUTPUT);
    pinMode(47, OUTPUT);
    pinMode(49, OUTPUT);
    pinMode(51, OUTPUT);
    pinMode(53, OUTPUT);

    pinMode(role1, OUTPUT);
    pinMode(role2, OUTPUT);
    pinMode(role3, OUTPUT);
    pinMode(role4, OUTPUT);
    pinMode(role5, OUTPUT);
    pinMode(role6, OUTPUT);
    pinMode(role7, OUTPUT);
    pinMode(role8, OUTPUT);
    pinMode(pinBuzzer, OUTPUT);
  }
  {
    attachInterrupt(digitalPinToInterrupt(2), alarmON, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), alarmON, CHANGE);
    attachInterrupt(digitalPinToInterrupt(18), alarmON, CHANGE);
    attachInterrupt(digitalPinToInterrupt(19), alarmON, CHANGE);
  }
  {
    digitalWrite(role1, LOW);
    digitalWrite(role2, LOW);
    digitalWrite(role3, LOW);
    digitalWrite(role4, LOW);
    digitalWrite(role5, LOW);
    digitalWrite(role6, LOW);
    digitalWrite(role7, LOW);
    digitalWrite(role8, LOW);
    digitalWrite(pinBuzzer, LOW);

    kapi1.write(90);
    kapi2.write(90);
    garaj.write(90);
    pencere1.write(90);
    pencere2.write(90);
    pencere3.write(90);
  }
  {
    lcd1.setCursor(0, 0);
    lcd1.print("Okan CIDEM");
    lcd.setCursor(5, 0);
    lcd.print("Okan CIDEM");
    lcd.setCursor(1, 1);
    lcd.print("Bitirme Projesi II");
    lcd.setCursor(5, 2);
    lcd.print("16.01.2018");
    delay(5000);
    lcd.clear();
    lcd1.clear();
    lcd.setCursor(3, 0);
    lcd.print("Role Durumlari");
    lcd.setCursor(3, 2);
    lcd.print("ServoDurumlari");

    lcd1.setCursor(0, 0);
    lcd1.print("Ruzgar:");
    lcd1.setCursor(15, 0);
    lcd1.print("V");
    lcd1.setCursor(0, 1);
    lcd1.print("Gunes :");
    lcd1.setCursor(15, 1);
    lcd1.print("V");
    /*
      int i = 8, j = 6;
      while (i--) {
      lcd.setCursor(13 - i, 1);
      lcd.write(0);
      }
      while (j--) {
      lcd.setCursor(12 - j, 3);
      lcd.write(0);
      }*/
    LCD_durumYAZ();


  }
  //bahce_Aydinlatma(true);
  // garaj_Aydinlatma(true);
}
void loop()
{

  while (Serial2.available())
  {
    delay(10);
    char c = Serial2.read();
    if (c == '#') {
      break;
    }
    voice += c;
  }
  if (voice.length() > 0)
  {
    Serial2.println(voice);
    if (voice == "*acil durum Protokolü aktif") {
      switchon();
    }
    else if (voice == "*acil durum Protokolü devre dışı") {
      switchoff();
    }
    else if (voice == "*alarm aktif") {
      alarmAcik = HIGH;
      delay(500);
    }
    else if (voice == "*alarm devre dışı" || voice == "*Alarm kapat") {
      digitalWrite(pinBuzzer, LOW);
      alarmAcik = LOW;
    }
    else if (voice == "*mutfak lamba aç") {
      digitalWrite(role1, HIGH);
      RoleDurumu[0] = true;
    }
    else if (voice == "*mutfak lamba kapat") {
      digitalWrite(role1, LOW);
      RoleDurumu[0] = false;
    }
    else if (voice == "*tuvalet lamba aç") {
      digitalWrite(role2, HIGH);
      RoleDurumu[1] = true;
    }
    else if (voice == "*tuvalet lamba kapat") {
      digitalWrite(role2, LOW);
      RoleDurumu[1] = false;
    }
    else if (voice == "*salon lamba aç") {
      digitalWrite(role3, HIGH);
      RoleDurumu[2] = true;
    }
    else if (voice == "*salon lamba kapat") {
      digitalWrite(role3, LOW);
      RoleDurumu[2] = false;
    }
    else if (voice == "*garaj lamba aç") {
      digitalWrite(role4, HIGH);
      RoleDurumu[3] = true;
    }
    else if (voice == "*garaj lamba kapat") {
      digitalWrite(role4, LOW);
      RoleDurumu[3] = false;
    }
    else if (voice == "*depo lamba aç") {
      digitalWrite(role5, HIGH);
      RoleDurumu[4] = true;
    }
    else if (voice == "*depo lamba kapat") {
      digitalWrite(role5, LOW);
      RoleDurumu[4] = false;
    }
    else if (voice == "*mutfak havalandırma aç") {
      digitalWrite(role6, HIGH);
      RoleDurumu[5] = true;
    }
    else if (voice == "*mutfak havalandırma kapat" || voice == "*Klima 1 kapat") {
      digitalWrite(role6, LOW);
      RoleDurumu[5] = false;
    }
    else if (voice == "*salon havalandırma aç") {
      digitalWrite(role7, HIGH);
      RoleDurumu[6] = true;
    }
    else if (voice == "*salon havalandırma kapat" || voice == "*Klima 2 kapat") {
      digitalWrite(role7, LOW);
      RoleDurumu[6] = false;
    }
    else if (voice == "*garaj havalandırma aç") {
      digitalWrite(role8, HIGH);
      RoleDurumu[7] = true;
    }
    else if (voice == "*garaj havalandırma kapat" || voice == "*Klima 3 kapat") {
      digitalWrite(role8, LOW);
      RoleDurumu[7] = false;
    }
    else if (voice == "*tuvalet pencere aç" || voice == "*pencere 1 aç") {
      pencere1.write(160);
      ServoDurumu[0] = true;
    }
    else if (voice == "*tuvalet pencere kapat" || voice == "*pencere 1 kapat") {
      pencere1.write(90);
      ServoDurumu[0] = false;
    }
    else if (voice == "*salon pencere aç" ||voice == "*Salon pencere aç" || voice == "*pencere 2 aç") {
      pencere2.write(160);
      ServoDurumu[1] = true;
    }
    else if (voice == "*salon pencere kapat" ||voice == "*Salon pencere kapat" || voice == "*pencere 2 kapat") {
      pencere2.write(90);
      ServoDurumu[1] = false;
    }
    else if (voice == "*Mutfak pencere aç" ||voice == "*mutfak pencere aç" || voice == "*pencere 3 aç") {
      pencere3.write(160);
      ServoDurumu[2] = true;
    }
    else if (voice == "*Mutfak pencere kapat" ||voice == "*mutfak pencere kapat" || voice == "*pencere 3 kapat") {
      pencere3.write(90);
      ServoDurumu[2] = false;
    }
    else if (voice == "*Ana giriş aç" ||voice == "*ana giriş aç" || voice == "*kapı bir aç") {
      kapi1.write(160);
      ServoDurumu[3] = true;
    }
    else if (voice == "*Ana giriş kapat" ||voice == "*ana giriş kapat" || voice == "*kapı bir kapat") {
      kapi1.write(90);
      ServoDurumu[3] = false;
    }
    else if (voice == "*Mutfak kapı aç" ||voice == "*mutfak kapı aç" || voice == "*kapı iki aç" || voice == "*kapı 2 aç") {
      kapi2.write(160);
      ServoDurumu[4] = true;
    }
    else if (voice == "*Mutfak kapı kapat" ||voice == "*mutfak kapı kapat" || voice == "*kapı 2 kapat") {
      kapi2.write(90);
      ServoDurumu[4] = false;
    }
    else if (voice == "*garaj aç") {
      garaj.write(180);
      ServoDurumu[5] = true;
    }
    else if (voice == "*garaj kapat") {
      garaj.write(90);
      ServoDurumu[5] = false;
    }
    else if (voice == "*bahçe aydınlatma aç" || voice == "Bahçe aydınlatma aç") {
      bahce_Aydinlatma(true);
    }
    else if (voice == "*bahçe aydınlatma kapat" || voice == "Bahçe aydınlatma kapat") {
      bahce_Aydinlatma(false);
    }
    LCD_durumYAZ();
    voice = "";

  }
  uzaklik = ultrasonic1.distanceRead();

  if (uzaklik <= 6) {
    digitalWrite(47, 0);
    digitalWrite(49, 1);
    digitalWrite(53, 0);
    delay(50);


  }
  else if (uzaklik > 6 && uzaklik < 13) {
    digitalWrite(47, 0);
    digitalWrite(49, 0);
    digitalWrite(53, 1);
    delay(50);
  }
  else if (uzaklik >= 15 && uzaklik < 22 ) {
    digitalWrite(47, 1);
    digitalWrite(49, 0);
    digitalWrite(53, 0);
    delay(50);
  }
  else {
    digitalWrite(47, 0);
    digitalWrite(49, 0);
    digitalWrite(53, 0);
    delay(50);
  }
  ruzgarValue = analogRead(RuzgarsensorPin);
  gunesValue = analogRead(GunessensorPin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltageRuzgar = ruzgarValue * (5.0 / 1023.0);
  float voltageGunes = gunesValue * (22.5 / 1023.0);

  lcd1.setCursor(9, 0);
  lcd1.print(voltageRuzgar);

  lcd1.setCursor(9, 1);
  lcd1.print(22.5-2.0-voltageGunes);



}
void switchon()
{

  lcd.setCursor(0, 1);
  lcd.print("ACIL!");
  lcd.setCursor(0, 3);
  lcd.print("DURUM!");
  LCD_durumYAZ();

  digitalWrite(role1, HIGH); RoleDurumu[0] = true;
  digitalWrite(role2, HIGH); RoleDurumu[1] = true;
  digitalWrite(role3, HIGH); RoleDurumu[2] = true;
  digitalWrite(role4, HIGH); RoleDurumu[3] = true;
  digitalWrite(role5, HIGH); RoleDurumu[4] = true;
  digitalWrite(role6, HIGH); RoleDurumu[5] = true;
  digitalWrite(role7, HIGH); RoleDurumu[6] = true;
  digitalWrite(role8, HIGH); RoleDurumu[7] = true;
  kapi1.write(160); ServoDurumu[0] = true;
  kapi2.write(160); ServoDurumu[1] = true;
  garaj.write(160); ServoDurumu[2] = true;
  pencere1.write(160); ServoDurumu[3] = true;
  pencere2.write(160); ServoDurumu[4] = true;
  pencere3.write(160); ServoDurumu[5] = true;
  bahce_Aydinlatma(true);
  garaj_Aydinlatma(true);
}
void switchoff()
{
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 3);
  lcd.print("      ");
  LCD_durumYAZ();
  digitalWrite(role1, LOW); RoleDurumu[0] = false;
  digitalWrite(role2, LOW); RoleDurumu[1] = false;
  digitalWrite(role3, LOW); RoleDurumu[2] = false;
  digitalWrite(role4, LOW); RoleDurumu[3] = false;
  digitalWrite(role5, LOW); RoleDurumu[4] = false;
  digitalWrite(role6, LOW); RoleDurumu[5] = false;
  digitalWrite(role7, LOW); RoleDurumu[6] = false;
  digitalWrite(role8, LOW); RoleDurumu[7] = false;
  kapi1.write(90); ServoDurumu[0] = false;
  kapi2.write(90); ServoDurumu[1] = false;
  garaj.write(90); ServoDurumu[2] = false;
  pencere1.write(90); ServoDurumu[3] = false;
  pencere2.write(90); ServoDurumu[4] = false;
  pencere3.write(90); ServoDurumu[5] = false;
  bahce_Aydinlatma(false);
  garaj_Aydinlatma(false);
}
void alarmON()
{
  alarmDurumu = HIGH;
  alarmda_Mi();
}
void alarmda_Mi() {
  if (alarmAcik) {
    if (alarmDurumu)
      digitalWrite(pinBuzzer, HIGH);
    else
      digitalWrite(pinBuzzer, LOW);
  } else
    digitalWrite(pinBuzzer, LOW);
}
void LCD_durumYAZ() {
  int i = 8, j = 6;
  while (i--) {
    lcd.setCursor(13 - i, 1);
    if (RoleDurumu[7 - i] == true) {
      lcd.write(1);
    }
    if (RoleDurumu[7 - i] == false) {
      lcd.write(0);
    }
  }
  while (j--) {
    lcd.setCursor(12 - j, 3);
    if (ServoDurumu[5 - j] == true) {
      lcd.write(1);
    }
    if (ServoDurumu[5 - j] == false) {
      lcd.write(0);
    }
  }
}
void bahce_Aydinlatma(boolean x) {

  digitalWrite(43, x);
  digitalWrite(45, x);
  digitalWrite(51, x);

}
void garaj_Aydinlatma(boolean x) {

  digitalWrite(47, x);
  digitalWrite(49, x);
  digitalWrite(53, x);

}
