#define POT A0
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}


void initSerial() {
    Serial.begin(9600);
    while (!Serial) {}
}

void setup() {
    initSerial();
    initLCD();
    lcd.setCursor(0,0);
    lcd.print("Zadanie 4");
}


void loop()
{
  double pot = analogRead(POT);
    String str = String(pot);
    Serial.println(str);
    
}