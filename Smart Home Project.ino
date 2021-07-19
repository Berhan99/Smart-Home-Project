#include <LiquidCrystal_I2C.h>
#include<Servo.h>
LiquidCrystal_I2C lcd(0x27,16,2) ;
Servo myServoMotor;
int ledPinOpen = 10;
int ledPinClose = 8;
int buzzerPin = 3;
int buttonPin = 2;
char deger;
int buttonState = 0;
float sicaklik;
float gerilim;
int sicaklikPin = 0;
int esikDegeri = 400;               //Gaz eşik değerini belirliyoruz.    
int buzzergazPin = 7;                  //Buzzerın takılacağı pin
int degergaz;                          //Sensörden okunan değer


void setup()
{
  lcd.begin();
  lcd.print("welcome home");
  delay(3000);
  lcd.clear();
  
  Serial.begin(9600);
  myServoMotor.attach(9);
  myServoMotor.write(0);
 
  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzergazPin, OUTPUT);       //Buzzer pinimizi çıkış olarak ayarlıyoruz.
  pinMode(buttonPin, INPUT);
  pinMode(ledPinOpen, OUTPUT);
  pinMode(ledPinClose, OUTPUT);
  digitalWrite(ledPinClose, HIGH);
}
void loop()
{
  gerilim = analogRead(sicaklikPin);
  gerilim = (gerilim / 1023) * 5000;
  sicaklik = gerilim / 10.0;
  buttonState = digitalRead(buttonPin);
  degergaz = analogRead(A3);           //Sensörden analog değer okuyoruz.

    if(degergaz > esikDegeri){           //Sensörden okunan değer eşik değerinden büyükse çalışır.
      lcd.clear();
      lcd.home();
      lcd.print("gaz alarmi !");
      Serial.println("gaz alarmi !");
    digitalWrite(buzzergazPin, HIGH);
    delay(500);
    digitalWrite(buzzergazPin, LOW);
    delay(100);
  }
  else{                             //Sensörden okunan değer eşik değerinin altındaysa çalışır.
    digitalWrite(buzzergazPin, LOW);
  }


  if (buttonState == LOW)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
  }
  else {
    digitalWrite(buzzerPin, LOW);
  }
  if (Serial.available())
  {
    deger = Serial.read();
    if (deger == 'b')
    {
      lcd.home();
      lcd.print("kapi kapandi");
      Serial.println("kapi kapandi");
      digitalWrite(ledPinOpen, LOW);
      digitalWrite(ledPinClose, HIGH);
      for (int derece = 0; derece < 90; derece++)
      {
        myServoMotor.write(derece);
      }
    }
    if (deger == 'a')
    {
      lcd.clear();
      lcd.home();
      lcd.print("kapi acildi");
      Serial.println("kapi acildi");
      digitalWrite(ledPinOpen, HIGH);
      digitalWrite(ledPinClose, LOW);
      for (int derece = 90; derece > 1; derece--)
      {
        myServoMotor.write(derece);
      }
    }
    if (deger == 'c')
    {
      lcd.clear();
      lcd.home();
      lcd.print("ortam sicakligi");
      lcd.setCursor(0, 1);
      lcd.print(sicaklik);
      Serial.print("ortam sicakligi");
      Serial.print(sicaklik);
      Serial.println("derece");
    }
    if (deger == 'd')
    {
      lcd.clear();
      lcd.home();
      lcd.print("kapi zili caldi...");
      Serial.println("kapi zili caldi...");
      digitalWrite(buzzergazPin, HIGH);
      delay(1000);
    }
    else
    {
      digitalWrite(buzzerPin, LOW);
    }
    delay(100);
  }
}
