#include <AFMotor.h>
#include <Servo.h>

#define echopin A4 
#define trigpin A5 

Servo myservo1;
Servo myservo2;  // Renamed for consistency

const int MOTOR_1 = 1; 
const int MOTOR_2 = 2; 
const int MOTOR_3 = 3; 
const int MOTOR_4 = 4; 
int irMotionPin = A0; 
int pirMotionPin = 2; 
int relayPin = 13;   

AF_DCMotor motor1(MOTOR_1, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor2(MOTOR_2, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor3(MOTOR_3, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor4(MOTOR_4, MOTOR12_64KHZ); // create motor object, 64KHz pwm

int distance_L, distance_F, distance_R;
long distance;
int set = 20;

void setup() {
  Serial.begin(9600);           
  Serial.println("Start");

  myservo1.attach(10); 
  myservo2.attach(9);
  myservo1.write(90);  // Set initial position for myservo1
  myservo2.write(90);  // Set initial position for myservo2

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(relayPin, OUTPUT);

  pinMode(pirMotionPin, INPUT);
  digitalWrite(relayPin, LOW); 

  motor1.setSpeed(180);          // set the motor speed to 0-255
  motor2.setSpeed(180);
  motor3.setSpeed(180);
  motor4.setSpeed(180);
}
 
void loop() {
  myservo2.write(0);
  delay(2000);

  myservo2.write(180);
  delay(1000); 

  myservo1.write(0);
  delay(2000);

  myservo1.write(180);
  delay(2000); 

  int irValue = analogRead(irMotionPin);
  int pirValue = digitalRead(pirMotionPin);

  Serial.print("IR Value: ");
  Serial.print(irValue);
  Serial.print(" | PIR Value: ");
  Serial.println(pirValue);

  if (irValue > 500 && pirValue == HIGH) {  // Ensure correct logic for PIR value
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump ON");
    stopMotors();  // Stop the rover
    delay(5000);  // Wait for 5 seconds
    digitalWrite(relayPin, LOW);  
    hc_sr4();  
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Pump OFF");
    hc_sr4();  
  }

  distance_F = data();
  Serial.print("S=");
  Serial.println(distance_F);
  if (distance_F > set) {
    Serial.println("Forward");
    moveForward();
  } else {
    hc_sr4();
  }
}

long data() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  distance = pulseIn(echopin, HIGH);
  return distance / 29 / 2;
}

void compareDistance() {
  if (distance_L > distance_R) {
    moveBackward();
    delay(350);
  } else if (distance_R > distance_L) {
    moveRight();
    delay(350);
  } else {
    moveBackward();
    delay(300);
    turnLeft();
    delay(500);
  }
}

void hc_sr4() {
  Serial.println("Stop");
  stopMotors();

  myservo1.write(0);
  delay(300);
  distance_R = data();
  delay(100);
  myservo1.write(170);
  delay(500);
  distance_L = data();
  delay(100);
  myservo1.write(90);
  delay(300);
  compareDistance();
}

void moveForward() {
  motor1.run(FORWARD);         
  motor2.run(FORWARD); 
  motor3.run(FORWARD); 
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.run(BACKWARD);   
  motor2.run(BACKWARD);
  motor3.run(BACKWARD); 
  motor4.run(BACKWARD); 
}

void moveRight() {
  motor1.run(FORWARD);   
  motor2.run(FORWARD); 
  motor3.run(BACKWARD); 
  motor4.run(BACKWARD); 
}

void turnLeft() {
  motor1.run(BACKWARD);   
  motor2.run(BACKWARD);
  motor3.run(FORWARD); 
  motor4.run(FORWARD); 
}

void stopMotors() {
  motor1.run(RELEASE);         
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
