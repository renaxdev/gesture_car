#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

Servo servo;

/*
#include <MotorControl.h>

Motor motor(1,2,3,4,5,5)*/

typedef struct struct_inc_data {
  float x;
  float y;
  float z;
} struct_inc_data;

struct_inc_data incomingData;
void receivedDataCallback( uint8_t *mac, uint8_t *data, uint8_t len ){
  memcpy(&incomingData, data, sizeof(incomingData));
  Serial.print("Received:\nX: ");
  Serial.print(incomingData.x, 2);
  Serial.print("\nY: ");
  Serial.print(incomingData.y, 2);
  Serial.print("\nZ: ");
  Serial.println(incomingData.z, 2);
}


void setup() {
  Serial.begin( 115200 ); 
  //nix yalmotor.begin();
  WiFi.mode( WIFI_STA );
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(receivedDataCallback);
  servo.attach(5);
}


void loop() {
  int servoPos = map(incomingData.x * 100, -100, 100, 0, 180);
  servo.write(servoPos);
  /*
  if(incomingData.x > 0.25){
    //servo links motor vorne
  }
  
  if(incomingData.x < -0.25){
    //servo rechts motor vorne
  }
  */
  delay(20);
}