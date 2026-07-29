#include <esp_now.h>
#include <WiFi.h>

typedef struct
{
  uint8_t flag;
} Message;

Message msg;

uint8_t receiverMAC[] = {0x38, 0x18, 0x2B, 0xB2, 0xB2, 0x94};   // Receiver MAC

unsigned long lastSendTime = 0;
unsigned long lastSerialPrint = 0;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status)
{
  // Optional callback
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Peer Add Failed");
    return;
  }

  msg.flag = 0;

  Serial.println("Transmitter Ready");
}

void loop()
{
  // Change flag using Serial Monitor
  if (Serial.available())
  {
    char c = Serial.read();

    if (c == '1')
      msg.flag = 1;

    else if (c == '0')
      msg.flag = 0;
  }

  // Send every 50 ms
  if (millis() - lastSendTime >= 50)
  {
    lastSendTime = millis();

    esp_now_send(receiverMAC, (uint8_t *)&msg, sizeof(msg));
  }

  // Print every second
  if (millis() - lastSerialPrint >= 1000)
  {
    lastSerialPrint = millis();

    Serial.print("Sending Flag : ");
    Serial.println(msg.flag);
  }
}
