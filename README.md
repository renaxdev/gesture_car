# gesture_car
ESP8266-powered RC-Car controlled by gestures

### How does this project work?
The remote-controlled car is operated with an ESP8266, which controls two DC motors and a servo motor for steering.
To control the car, an M5 Stick C PLUS2 with an integrated inertial measurement unit (IMU) is used, which is capable of processing movements to the left, right, front and rear.

The M5 controller is connected wirelessly to the ESP8266 on the car and the “ESP-NOW” protocol is used to transmit the data from the IMU, as it is quite fast - control takes place in real time.

Two DC motors are used to drive the car and a small SG90 servo motor is responsible for the steering.
