// 07 octobre 2017
// acquisition de 600 points de mesures à 62,5 kHz
// ATTENTION : maxi de la mémoire
// version opérationnelle de l'échantillonnage et envoi par port série des données



const int sensorPin = 0;
const int numberOfEntries = 600;
const int timer = 1000;
unsigned long microseconds;
unsigned long duration;
int results[numberOfEntries];
float periode_arret;

void setup() {
  // gestion communication
  Serial.begin(9600);
 
  // acceleration des performances d'acquisition analogique
  bitClear(ADCSRA,ADPS0);
  bitClear(ADCSRA,ADPS1);
  bitSet(ADCSRA,ADPS2);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(sensorPin)>10)
  {
      microseconds = micros();
      for (int i = 0;i < numberOfEntries; i++)
      {
        results[i] = analogRead(sensorPin);
      }
      duration = micros()-microseconds;
      // Envoi des données
      double periode = duration/numberOfEntries;
      microseconds = micros();
      Serial.println("start");
      // période en µs
      Serial.println(periode);
      // fréquence en kHz
      Serial.println(1000/periode);
      for (int i = 0;i < numberOfEntries; i++)
      {
        Serial.println(results[i]);
      }
      duration = micros()-microseconds;
      // durée de l'envoi en s
      Serial.println(duration/1000000., DEC);
      delay(timer);
    }
}


