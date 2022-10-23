
#define POT A0

void initSerial() {
    Serial.begin(9600);
    while (!Serial) {}
}

void setup() {
    initSerial();
}


void loop()
{
  double pot = analogRead(POT);
    String str = String(pot);
    Serial.println(str);
}