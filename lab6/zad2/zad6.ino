#include <RgbHandler.h>
// Zadanie 2: Implementacja własnej biblioteki ze sterownikiem wybranego urządzenia

// Przygotuj własną bibliotekę, która będzie służyła do sterowania diodą RGB.
// Samodzielnie zaprojektuj interfejs programistyczny tej biblioteki, tak aby pozwalał podać,
// do których pinów podłączona jest dioda, oraz aby pozwalał ustawiać kolory
// poprzez podanie wartości komponentów RGB oraz przez nazwy kolorów:
// RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, BLACK, WHITE.
// Zadbaj o kolorowanie składni w Arduino IDE.
// Jest to zadanie przykładowe. Prowadzący może zmodyﬁkować lub zmienić treść zadania.

// constructor of RgbDiode()
RgbDiode myRGB(6, 5, 3);
String literals[] = { "RED", "YELLOW", "GREEN", "CYAN", "BLUE", "MAGENTA", "WHITE", "BLACK", "USER" };
int literalLen = 9;
int literalIndex = 0;

Color mycolor = new Color(0, 2, 3);

Color colors[] = { Color(0, 144, 255), Color(255, 144, 0) };
int colorsLen = 2;
int colorsIndex = 0;

void setup() {
  myRGB.initialize();
}

void loop() {
  myRGB.writeColor(literals[literalIndex]);
  literalIndex = ++literalIndex % 9;
  delay(500);
  myRGB.setColor(colors[colorsIndex]);
  colorsIndex = ++colorsIndex % 2;
}