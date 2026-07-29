#include <esp_now.h>
#include <WiFi.h>

HardwareSerial UART_Device(2);

typedef struct
{
  uint8_t flag;
} Message;

Message incomingData;

volatile uint8_t currentFlag = 1;   // Fail-safe default

unsigned long lastPacketTime = 0;

const unsigned long timeout = 200;   // ms

unsigned long lastUARTSend = 0;
unsigned long lastSerialPrint = 0;

void OnDataRecv(const esp_now_recv_info_t *recv_info,
                const uint8_t *incomingDataBuffer,
                int len)
{
  memcpy(&incomingData, incomingDataBuffer, sizeof(incomingData));

  currentFlag = incomingData.flag;

  lastPacketTime = millis();
}

void setup()
{
  Serial.begin(115200);

  // UART2
  // RX = GPIO16
  // TX = GPIO17
  UART_Device.begin(115200, SERIAL_8N1, 16, 17);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver Ready");
}

void loop()
{
  // Communication lost
  if (millis() - lastPacketTime > timeout)
  {
    currentFlag = 1;
  }

  // Send UART every 10 ms
  if (millis() - lastUARTSend >= 10)
  {
    lastUARTSend = millis();

    UART_Device.write(currentFlag);
  }

  // Print once every second
  if (millis() - lastSerialPrint >= 1000)
  {
    lastSerialPrint = millis();

    Serial.print("Current Flag : ");
    Serial.println(currentFlag);

    if (currentFlag == 1 && (millis() - lastPacketTime > timeout))
      Serial.println("Status : Communication Lost");
    else
      Serial.println("Status : Communication OK");
  }
}
