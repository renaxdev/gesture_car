#include <M5StickC.h>
#include <esp_now.h>
#include <WiFi.h>


float x,y,z;

uint8_t recvAddress[] = {0x40,0x91,0x51,0x51,0x34,0xF2};
esp_now_peer_info_t peerInfo;

typedef struct struct_data {
  float x;
  float y;
  float z;
} struct_data;

struct_data accelValues;


void sentCb(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "ESP-NOW: Success": "ESP-NOW: Failed");
}

void setup() {
  WiFi.mode(WIFI_STA);
  M5.begin();
  M5.IMU.Init();

  esp_now_init();

  esp_now_register_send_cb(sentCb);
  Serial.println("Initialized successfully");

  memcpy(peerInfo.peer_addr, recvAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("ESP-NOW: Failed to add peer");
    return;
  }
}

void loop() {
  M5.IMU.getAccelData(&accelValues.x, &accelValues.y, &accelValues.z);
  if(accelValues.x > 0.25){
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("links ");
  }
  if(accelValues.x < -0.25){
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("rechts");
  }

  if (accelValues.x < 0.25 && accelValues.x > -0.25) {
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("Gerade");
  }

  if(accelValues.z < 0.90 && accelValues.y > 0.25){
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("Hinten");
  }

  if(accelValues.z < 0.90 && accelValues.y < -0.25){
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("Vorne");
  }


  esp_err_t res = esp_now_send(recvAddress, (uint8_t *) &accelValues, sizeof(accelValues));
  delay(50);
}
