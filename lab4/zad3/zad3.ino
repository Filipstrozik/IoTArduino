
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

int value_curr = -1;
void loop()
{
    int value_new = analogRead(POTENT);
    int other = value_new;
    value_new = map(value_new, 0, 1023, 0, 500);
    if(value_curr != value_new) {
      value_curr = value_new;
      double temp = value_curr;
      temp = temp/100;
      display(String(other), String(temp));
    }
    delay(100);
}