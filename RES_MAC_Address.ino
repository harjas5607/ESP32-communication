#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.printf("Mode: %d\n", WiFi.getMode());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}