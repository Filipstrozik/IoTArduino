//Korzystając z programu zamieszczonego jako kod 3, proszę zaobserwować w zestawie laboratoryjnym jak zachowują się
//wyprowadzenia enkodera przy obrocie w lewo i przy obrocie w prawo o jedną pozycję. Odnotować, a wręcz narysować sobie,
//sekwencję stanów przy obrocie do kolejnej pozycji spoczynkowej (o jeden „ząbek”), zarówno w lewo, jak i w prawo. Proszę
//odpowiedzieć jak, patrząc tylko na przebieg stanów wyprowadzeń w czasie, można stwierdzić, w którą stronę obrócono
//enkoder i o ile pozycji. Proszę opisać krótko słownie algorytm, który to realizuje.

#include <LiquidCrystal_I2C.h>
#define LED_RED 6
#define LED_BLUE 3
#define ENCODER1 A2
#define ENCODER2 A3
LiquidCrystal_I2C lcd(0x27, 16, 2);

void showExcerciseNumber(String sVal){
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sVal);
    // delay(1500);
}


void setup()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(ENCODER1, INPUT_PULLUP);
	pinMode(ENCODER2, INPUT_PULLUP);
  showExcerciseNumber("Zadanie 1");
}
void loop()
{
	digitalWrite(LED_RED, digitalRead(ENCODER1));
	digitalWrite(LED_BLUE, digitalRead(ENCODER2));
}
//Gdy przyjmiemy wage dla jednego podłączenia to obracając w lewo bedziemy miec inną sekwencje sumy sygnałów *  waga 
// a inna sekwenja obrotu w prawo. KOD GRAYA?
