#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
int led[] = {LED_RED, LED_GREEN, LED_BLUE};

void initRGB() {
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);
}




void initSerial() {
    Serial.begin(9600);
    while (!Serial) {}
}

void initLCD()
{
  lcd.init();
  lcd.backlight();
}


void setup() {
    initRGB();
    initSerial();
    initLCD();
    lcd.setCursor(0,0);
    lcd.print("Zadanie 2");

}

 void loop()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    Serial.println(command.substring(0,3)); // czy set

    if (command.substring(0,3) == "set")
    {
      command = command.substring(4);
      //Serial.println(command);
      char str[command.length()+1];
      command.toCharArray(str,command.length()+1);
     	char * pch;
      pch = strtok (str,",");
  		while (pch != NULL)
  		{
          String com = pch;
          com.trim();
          if(com.charAt(0)=='r'){
            int val = com.substring(2).toInt();
            if(val <= 255 && val >= 0){
              analogWrite(LED_RED, val);
              Serial.println(String("LED RED: ") + String(val));
            } else {
              Serial.println("Unknown LED RED value");
            }
          } else if (com.charAt(0)=='g'){
            int val = com.substring(2).toInt();
            if(val <= 255 && val >= 0){
              analogWrite(LED_GREEN, val);
              Serial.println(String("LED GREEN: ") + String(val));
            } else {
              Serial.println("Unknown led value");
            }
          } else if (com.charAt(0)=='b'){
            int val = com.substring(2).toInt();
            if(val <= 255 && val >= 0){
              analogWrite(LED_BLUE, val);
              Serial.println(String("LED BLUE: ") + String(val));
            } else {
              Serial.println("Unknown led value");
            }
          } else {
            Serial.println(String("Unknown led color ’") + command + "’");
            pch = NULL;
          }
          //Serial.println(com);
    	  pch = strtok (NULL, ",");
  		}
    }
    else if (command == "off")
    {
    	digitalWrite(LED_RED, LOW);
    	digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    	Serial.println("LED OFF");
    }
    else
    {
    Serial.println(String("Unknown command ’") + command + "’");
    }
  }
}
