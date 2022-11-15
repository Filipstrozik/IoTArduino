// Zadanie 2: Implementacja własnej biblioteki ze sterownikiem wybranego urządzenia

// Przygotuj własną bibliotekę, która będzie służyła do sterowania diodą RGB. 
// Samodzielnie zaprojektuj interfejs programistyczny tej biblioteki, tak aby pozwalał podać, 
// do których pinów podłączona jest dioda, oraz aby pozwalał ustawiać kolory
// poprzez podanie wartości komponentów RGB oraz przez nazwy kolorów: 
// RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, BLACK, WHITE. 
// Zadbaj o kolorowanie składni w Arduino IDE.
// Jest to zadanie przykładowe. Prowadzący może zmodyﬁkować lub zmienić treść zadania.

#include "RgbHandler.h"

RgbDiode myRGB(6, 5, 3);

void setup() {
    myRGB.initialize();
}

void loop() {
    myRGB.writeColor("red");
    delay(1000);
    myRGB.writeColor("green");
    delay(1000);
    myRGB.writeColor("blue");
    delay(1000);
    myRGB.writeColor("yellow");
    delay(1000);
    myRGB.writeColor("cyan");
    delay(1000);
    myRGB.writeColor("magenta");
    delay(1000);
    myRGB.writeColor("white");
    delay(1000);
    myRGB.writeColor("black");
    delay(1000);
    myRGB.writeAll(50, 34, 77);
    delay(1000);
}