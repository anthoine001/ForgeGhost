// 17 octobre 2017 
// UNO 
// acquisition de numberOfEntries points de mesures à 62,5 kHz 
// ATTENTION : 700 = maxi de la mémoire 
// version opérationnelle de l'échantillonnage et envoi par port série des données 
// captation des valeurs negatives 
// valeur nominale 2,5V + valeur lue ; nécessite le montage avec diviseur de tension et mise de la masse du piezo
// sur la sortie du diviseur
// ajout ecran lcd i2c sur les ports A4 et A5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);
//LiquidCrystal_I2C lcd(0x27, 20, 4);

const int sensorPin = 0; 
const int baud = 19200; 
const int seuil = 540; 
const int numberOfEntries = 700; 
const int timer = 100; 
unsigned long microseconds; 
unsigned long last_blow,blow; 
float tcy;
boolean arret; 
unsigned long duration; 
int results[numberOfEntries]; 
const int periode_arret = 10; 

void setup() { 
  // gestion communication 
  Serial.begin(baud); 
  
  // acceleration des performances d'acquisition analogique UNO uniquement
  bitClear(ADCSRA,ADPS0); 
  bitClear(ADCSRA,ADPS1); 
  bitSet(ADCSRA,ADPS2); 

  // ecran
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" ForgeGhost 1.0");
  delay(3000);
  lcd.clear();

  // autre
  arret = true;
} 

void loop() { 

  if (analogRead(sensorPin)>seuil) 
  { 
      blow = millis(); 
      if (arret){
        tcy = 0;
      }
      else{
         tcy = (blow-last_blow)/1000.0;
      }
      arret = false;
      last_blow = blow; 

      microseconds = micros(); 
      for (int i = 0;i < numberOfEntries; i++) 
      { 
        results[i] = analogRead(sensorPin); 
      } 
      duration = micros()-microseconds; 
      // Envoi des données 
      double periode = duration/numberOfEntries; 
      microseconds = micros();  
      for (int i = 0;i < numberOfEntries; i++) 
      { 
        Serial.println(results[i]); 
      } 
      duration = micros()-microseconds;  
      lcd.setCursor(0,1);
      lcd.print(" comm : ");
      lcd.print(duration/1000000.);
      lcd.print(" s"); 
      Serial.println(tcy); 
      delay(timer);
      lcd.setCursor(0,0);
      lcd.print(" Tcy : ");
      lcd.setCursor(7,0);
      lcd.print(tcy);
    } 
    if (millis()-last_blow > (periode_arret * 1000)){
      if (!arret){
        lcd.setCursor(0,0);
        lcd.print(" Presse a       ");
        lcd.setCursor(0,1);
        lcd.print(" l'arret        ");
      }
      arret = true;
    }
    
} 
