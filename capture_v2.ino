// 11 octobre 2017 
// UNO 
// acquisition de numberOfEntries points de mesures à 62,5 kHz 
// ATTENTION : 700 = maxi de la mémoire 
// version opérationnelle de l'échantillonnage et envoi par port série des données 
// captation des valeurs negatives 
// valeur nominale 2,5V + valeur lue ; nécessite le montage avec diviseur de tension et mise de la masse du piezo
// sur la sortie du diviseur


const int sensorPin = 0; 
const int baud = 19200; 
const int seuil = 540; 
const int numberOfEntries = 700; 
const int timer = 100; 
unsigned long microseconds; 
unsigned long last_blow,blow; 
float tcy; 
unsigned long duration; 
int results[numberOfEntries]; 
float periode_arret; 

void setup() { 
  // gestion communication 
  Serial.begin(baud); 
  
  // acceleration des performances d'acquisition analogique UNO uniquement
  bitClear(ADCSRA,ADPS0); 
  bitClear(ADCSRA,ADPS1); 
  bitSet(ADCSRA,ADPS2); 
} 

void loop() { 
  if (analogRead(sensorPin)>seuil) 
  { 
      blow = millis(); 
      tcy = (blow-last_blow)/1000.0; 
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
      if (duration/1000000<tcy) { 
        Serial.println("transmission ok"); 
      } 
      else { 
        Serial.println("attention : risque perte 1 signal"); 
      } 
      Serial.println(tcy); 
      delay(timer); 
    } 
} 
