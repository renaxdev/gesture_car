/*#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "Arduino.h"

class Motor {
  public:
    Motor(int pwm1, int in1, int in2, int pwm2, int in3, int in4);
    void begin();
    void forward();
    void backward();
    void stop();
    void setSpeed();

  private:
    int _in1, _in2, _in3, _in4, _pwm1, _pwm2;
}

#endif*/