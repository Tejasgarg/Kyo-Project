#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define ENA 5   //Enable left
#define IN1 6   // Left forward
#define IN2 7   // Left backward
#define IN3 8   // Right forward
#define IN4 9   // Right backward
#define ENB 10  // Enable right


#define FAN_PIN 11


#define IR_LEFT 2  
#define IR_RIGHT 3  


#define AQI_SENSOR A0  


LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    pinMode(FAN_PIN, OUTPUT);
    
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    
    pinMode(AQI_SENSOR, INPUT);
    
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Kyo Air Purifier");
    delay(2000);
    lcd.clear();
}

void loop() {
    int leftSensor = digitalRead(IR_LEFT);
    int rightSensor = digitalRead(IR_RIGHT);
    int airQuality = analogRead(AQI_SENSOR);
    
    int AQI = map(airQuality, 0, 1023, 0, 500);  
    
    lcd.setCursor(0, 0);
    lcd.print("AQI: ");
    lcd.print(AQI);
    lcd.print("    "); 
    
    if (AQI > 250) {
        digitalWrite(FAN_PIN, HIGH);
    } else {
        digitalWrite(FAN_PIN, LOW);
    }
    //If AQI is above 250, the air purifier will turn on


    if (leftSensor == LOW && rightSensor == LOW) { 
        moveForward();
    } 
    else if (leftSensor == LOW && rightSensor == HIGH) { 
        turnLeft();
    } 
    else if (leftSensor == HIGH && rightSensor == LOW) { 
        turnRight();
    } 
    else {  
        moveBackward();
        delay(500);
        turnRight();
        delay(500);
    }
}

void moveForward() {
    analogWrite(ENA, 150); 
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moveBackward() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
