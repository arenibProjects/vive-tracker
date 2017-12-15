#define ENVELOPPE_PIN 6

unsigned long currentTime;
unsigned long previousTime;
unsigned int i = 100;
bool up = true;
unsigned char axis = 1;

void setup() {
  pinMode(ENVELOPPE_PIN, OUTPUT);
  digitalWrite(ENVELOPPE_PIN, LOW);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  //Serial.begin(115200);
  //Serial.println("Teensy 3.6 ready !");
  
  delay(1000);
}

void loop() {
  // -- Start of the cycle --

  digitalWrite(13, HIGH); // DEBUG : LED
  // Sync pulse
  digitalWrite(ENVELOPPE_PIN, HIGH);
  if(!axis)
  {
    delayMicroseconds(130);
    digitalWrite(ENVELOPPE_PIN, LOW);
    delayMicroseconds(70);
    axis = 1;
  }
  else
  {
    delayMicroseconds(60);
    digitalWrite(ENVELOPPE_PIN, LOW);
    delayMicroseconds(140);
    axis = 0;
  }

  delayMicroseconds(i);
  digitalWrite(13, LOW); // DEBUG : LED

  // Sweep
  digitalWrite(ENVELOPPE_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ENVELOPPE_PIN, LOW);

  unsigned int truc = 8223 - i;
  delayMicroseconds(truc);
  //Serial.println((int) i);
  // -- End of the cycle --

  // Sweeping the laser sweep pulse (lol)
  if(up)
    i+=50;
  else
    i-=50;
  
  if(i >= 7900)
    up = false;
  else if(i <= 0)
    up = true;
}
