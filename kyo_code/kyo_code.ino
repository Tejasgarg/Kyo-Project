#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ENA 9
#define IN1 8
#define IN2 7
#define IN3 6
#define IN4 5
#define ENB 10

#define FAN_EN 11
#define FAN_IN1 12
#define FAN_IN2 13

#define IR_LEFT A0
#define IR_RIGHT A1
#define IR_FRONT A2

#define AQ_SENSOR A3

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(FAN_EN, OUTPUT);
    pinMode(FAN_IN1, OUTPUT);
    pinMode(FAN_IN2, OUTPUT);

    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    pinMode(IR_FRONT, INPUT);
    pinMode(AQ_SENSOR, INPUT);
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Air Purifier Bot");
    delay(2000);
    lcd.clear();
}

void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void stopMoving() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void controlFan(bool state) {
    if (state) {
        digitalWrite(FAN_IN1, HIGH);
        digitalWrite(FAN_IN2, LOW);
        analogWrite(FAN_EN, 255);
    } else {
        digitalWrite(FAN_IN1, LOW);
        digitalWrite(FAN_IN2, LOW);
        analogWrite(FAN_EN, 0);
    }
}

void loop() {
    int left = digitalRead(IR_LEFT);
    int right = digitalRead(IR_RIGHT);
    int front = digitalRead(IR_FRONT);
    int airQuality = analogRead(AQ_SENSOR) / 4; 

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AQI: ");
    lcd.print(airQuality);

    if (airQuality > 250) {
        controlFan(true);
    } else {
        controlFan(false);
    }

    if (front == HIGH) {
        stopMoving();
        delay(500);
        if (left == LOW) {
            turnLeft();
            delay(500);
        } else if (right == LOW) {
            turnRight();
            delay(500);
        }
    } else {
        moveForward();
    }
}
