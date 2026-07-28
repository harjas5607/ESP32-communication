#include <esp_now.h>
#include <WiFi.h>

typedef struct
{
  uint8_t flag;
} Message;

Message msg;

uint8_t receiverMAC[] = {0x38,0x18,0x2B,0xB2,0xB2,0x94};

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status)
{
  Serial.print("Send Status : ");

  if(status == ESP_NOW_SEND_SUCCESS)
    Serial.println("Success");
  else
    Serial.println("Failed");
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESPNow Init Failed");
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

  Serial.println("Transmitter Ready");
}

void loop()
{
  if (Serial.available())
  {
    char c = Serial.read();

    if (c == '1')
      msg.flag = 1;
    else if (c == '0')
      msg.flag = 0;
    else
      return;   // Ignore any other character

    esp_now_send(receiverMAC, (uint8_t *)&msg, sizeof(msg));

    Serial.print("Sent Flag : ");
    Serial.println(msg.flag);

    delay(1000);   // Wait 1 second before allowing another transmission
  }
}