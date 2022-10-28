#include <Servo.h>

// Declaring all the variables
int frontdist = 0;

int leftdist = 0;

int rightdist = 0;

Servo servo_9;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  servo_9.attach(9, 500, 2500);

  Serial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  servo_9.write(90);
  delay(1000); // Wait for 1000 millisecond(s)
  frontdist = 0.01723 * readUltrasonicDistance(11, 10);
  //if the front sensor doesnt detects obstacle
  if (frontdist > 20) {
    // forward
    Serial.println("forward");
    servo_9.write(90);
    delay(1000); // Wait for 1000 millisecond(s)
    analogWrite(6, 255);
    analogWrite(5, 255);
    digitalWrite(8, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(3, LOW);
  } else {
    //detects the obstacle
    // 1stop
    Serial.println("1stop");
    analogWrite(6, 0);
    analogWrite(5, 0);
    digitalWrite(8, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(3, LOW);
    delay(2000); // Wait for 2000 millisecond(s)
    // 1reverse
    Serial.println("1reverse");
    analogWrite(6, 127);
    analogWrite(5, 127);
    digitalWrite(8, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(3, HIGH);
    delay(2000); // Wait for 2000 millisecond(s)
    leftdist = 0.01723 * readUltrasonicDistance(13, 12);
    rightdist = 0.01723 * readUltrasonicDistance(A0, A1);
    //checks if theres any obstacle in the left. if yes, right
    if (leftdist < 20 && rightdist > 20) {
      
      // right
      Serial.println("right");
      servo_9.write(0);
      delay(1000); // Wait for 1000 millisecond(s)
      analogWrite(6, 127);
      analogWrite(5, 127);
      digitalWrite(8, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(3, LOW);
      delay(2000); // Wait for 2000 millisecond(s)
      
    } else {
      
      //checks if theres any obstacle in the right. if yes, left
      if (leftdist > 20 && rightdist < 20) {
        
        // left
        Serial.println("left");
        servo_9.write(180);
        delay(1000); // Wait for 1000 millisecond(s)
        analogWrite(6, 127);
        analogWrite(5, 127);
        digitalWrite(8, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(7, LOW);
        digitalWrite(3, LOW);
        delay(2000); // Wait for 2000 millisecond(s)
      
      } else {
        
        //checks if theres any obstacle in the both sides. if no, right(priority)
        if (leftdist > 20 && rightdist > 20) {
          Serial.println("right");
          servo_9.write(0);
          delay(1000); // Wait for 1000 millisecond(s)
          analogWrite(6, 127);
          analogWrite(5, 127);
          digitalWrite(8, HIGH);
          digitalWrite(4, HIGH);
          digitalWrite(7, LOW);
          digitalWrite(3, LOW);
          delay(2000); // Wait for 2000 millisecond(s)
          
        } else {
          //detects that there's no way to move forward and sideward,
          //so it goes in the rev direction
          
          
          // 2stop
          Serial.println("2stop");
          analogWrite(6, 0);
          analogWrite(5, 0);
          digitalWrite(8, LOW);
          digitalWrite(4, LOW);
          digitalWrite(7, LOW);
          digitalWrite(3, LOW);
          delay(1000); // Wait for 2000 millisecond(s)
          
          // 2reverse
          Serial.println("2reverse");
          analogWrite(6, 127);
          analogWrite(5, 127);
          digitalWrite(8, LOW);
          digitalWrite(4, LOW);
          digitalWrite(7, HIGH);
          digitalWrite(3, HIGH);
          delay(1000); // Wait for 2000 millisecond(s)

          // uturn
          Serial.println("uturn");
          analogWrite(6, 200);
          analogWrite(5, 150);
          digitalWrite(8, HIGH);
          digitalWrite(4, LOW);
          digitalWrite(7, LOW);
          digitalWrite(3, HIGH);
          delay(3000); // Wait for 3000 millisecond(s)
        }
      }
    }
  }
}