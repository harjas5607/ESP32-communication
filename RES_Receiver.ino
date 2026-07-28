#include <esp_now.h>
#include <WiFi.h>

HardwareSerial UART_Device(2);

typedef struct
{
  uint8_t flag;
} Message;

Message incomingData;

volatile uint8_t currentFlag = 0;

unsigned long lastPacketTime = 0;

const unsigned long timeout = 100;     // communication timeout (ms)

unsigned long lastUARTSend = 0;

void OnDataRecv(const esp_now_recv_info_t *recv_info,
                const uint8_t *incomingDataBuffer,
                int len)
{
  memcpy(&incomingData, incomingDataBuffer, sizeof(incomingData));

  currentFlag = incomingData.flag;

  lastPacketTime = millis();

  Serial.print("Received Flag : ");
  Serial.println(currentFlag);
}

void setup()
{
  Serial.begin(115200);

  UART_Device.begin(115200, SERIAL_8N1, 16, 17);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESPNow Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver Ready");
}

void loop()
{
  // If communication lost

  if ((millis() - lastPacketTime) > timeout)
  {
    currentFlag = 0;
  }

  // UART every 10 ms

  if (millis() - lastUARTSend >= 10)
  {
    lastUARTSend = millis();

    UART_Device.write(currentFlag);

    Serial.print("UART Sent : ");
    Serial.println(currentFlag);
  }
}