#include <Servo.h>

Servo myservo1;
Servo myservo2;

void setupServos() {
    myservo1.attach(4);
    myservo2.attach(2);
}

void hiServo() {
    myservo1.write(270);
    delay(1000);
    myservo1.write(180);
    delay(1000);
    myservo1.write(360);
    delay(1000);
    myservo1.write(180);
}

void twoHand() {
    myservo1.write(90);
    myservo2.write(0);
    delay(1000);
    myservo1.write(0);
    myservo2.write(90);
    delay(1000);
    myservo1.write(90);
    myservo2.write(0);
    delay(1000);
    myservo1.write(0);
    myservo2.write(90);
}