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

void setup() {
    initRGB();
    initSerial();
}

 void loop()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    Serial.println(command.substring(0,3));

    if (command.substring(0,3) == "set")
    {
      	command = command.substring(4);
      	Serial.println(command);
      	char str[command.length()+1];
      	command.toCharArray(str,command.length()+1);
     	char * pch;
       	pch = strtok (str,",");
  		while (pch != NULL)
  		{
          	String com = pch;
          	com.trim();
          if(com.charAt(0)=='r'){
            analogWrite(LED_RED, com.substring(2).toInt());
          } else if (com.charAt(0)=='g'){
          	analogWrite(LED_GREEN, com.substring(2).toInt());
          } else if (com.charAt(0)=='b'){
            analogWrite(LED_BLUE, com.substring(2).toInt());
          }
          	Serial.println(com);
    		pch = strtok (NULL, ",");
  		}
    	Serial.println("LED ON");
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
