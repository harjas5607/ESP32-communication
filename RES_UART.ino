#include <SoftwareSerial.h>

SoftwareSerial ESPSerial(2, 3);

void setup()
{
  Serial.begin(115200);
  ESPSerial.begin(115200);

  Serial.println("Waiting...");
}

void loop()
{
  if (ESPSerial.available())
  {
    byte flag = ESPSerial.read();

    Serial.print("UART Received : ");
    Serial.println(flag);
  }
}