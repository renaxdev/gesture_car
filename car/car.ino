#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

Servo servo;

int voltageWarner = 0;
int rawVal;
float voltage;
float batteryVoltage;

//Ultraschallsensor
int trig = 4; //D2
int echo = 2; //D4
long duration;
float distance;



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

float entfernungsmessung(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  return distance;
}


void setup() {
  Serial.begin( 115200 ); 
  WiFi.mode( WIFI_STA );
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(receivedDataCallback);
  servo.attach(5); //D1
  pinMode(voltageWarner, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}


void loop() {
//spannungsmessung --> led mit 555 IC 
  rawVal = analogRead(A0);
  voltage = rawVal * (3.3/ 1023.0); //Gemessene Spannung am Spannugsteiler
  batteryVoltage = voltage * ((22000 + 10000)/10000); //Eigentliche Spannung der Batterie
  Serial.println("Voltage: " + String(batteryVoltage) + " Volt");
  if(batteryVoltage <= 6.5){
    digitalWrite(voltageWarner, HIGH);
  }
  else {
    digitalWrite(voltageWarner, LOW);
  }

  int servoPos = map(incomingData.x * 100, -100, 100, 0, 180);
  Serial.println("Servopos: " + servoPos);

  if(entfernungsmessung() > 10){
    servo.write(servoPos);
    analogWrite(15, map(incomingData.y * 100, -100, 100, 0, 255));
    analogWrite(14, map(incomingData.y * 100, -100, 100, 0, 255));
    if(incomingData.y < -0.25){
      analogWrite(15, map(incomingData.y * 100, 0, -100, 0,255));
      analogWrite(14, map(incomingData.y * 100, 0, -100, 0,255));
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
    }

    if(incomingData.y > 0.25){
      analogWrite(13, map(incomingData.y * 100, 0, 100, 0,255));
      analogWrite(12, map(incomingData.y * 100, 0, 100, 0,255));
      digitalWrite(14, LOW);
      digitalWrite(15, LOW);
    }

    if(incomingData.y > -0.25 && incomingData.y < 0.25){
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      digitalWrite(14,LOW);
      digitalWrite(15,LOW);
    }
  }

  else {
    servo.write(map(incomingData.x * 100, -100, 100, 0, 180));
    analogWrite(13, 255);
    analogWrite(12, 255);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
  }

  delay(20);
}
