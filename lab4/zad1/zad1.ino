// Napisz program, który będzie wysyłał informację o stanie przycisków czerwonego i 
// zielonego przez łącze szeregowe do komputera PC. Sprawdź jaką maksymalną szybkość 
// transmisji danych możesz skonﬁgurować. Spróbuj zwizualizować stany
// przycisków na wykresie w Serial Plotter

#define RED_BUTTON 2
#define GREEN_BUTTON 4

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void initSerial() {
    Serial.begin(9600);
    while (!Serial) {}
}

void setup() {
    initButtons();
    initSerial();
}


void loop()
{
    double r = digitalRead(RED_BUTTON);
    double g = digitalRead(GREEN_BUTTON);
    String str = "RED: "+ String(r) + "  GREEN: " + String(g);
    Serial.println(str);
}