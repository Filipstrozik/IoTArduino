
// Napisz program, który dla napięcia zadanego potencjometrem, na wyświetlaczu LCD wyświetli 
// wartość, którą odczytał ADC i rzeczywistą wartość napięcia zadanego potencjometrem.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define POTENT A0

void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void setup()
{
    initLCD();

    lcd.setCursor(0,0);
    lcd.print("Zadanie 3");
    delay(3000);
    lcd.clear();
}

void display(String adc, String real_voltage)
{
    lcd.clear();
    lcd.setCursor(0,0);
  	lcd.print("adc: ");
    lcd.print(adc);
    lcd.setCursor(0,1);
  	lcd.print("real: ");
    lcd.print(real_voltage);
}

float value_curr = -1.0;
void loop()
{
    int value_new = analogRead(POTENT);
    int other = value_new;
    float j = (float) value_new;
    float volt = j*(5.0/1024.0);
    // value_new = map(value_new, 0, 1023, 0, 500);
    if(value_curr != volt) {
      value_curr = volt;
      // double temp = value_curr;
      // temp = temp/100;
      display(String(other), String(value_curr));
    }
    delay(100);
}