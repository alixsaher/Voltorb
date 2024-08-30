int irMotionPin = 9; 
int pirMotionPin = 2; 
int relayPin = 13;   
void setup() {
  Serial.begin(9600);     
  pinMode(pirMotionPin, INPUT);
  digitalWrite(relayPin, LOW); 


}

void loop() {
 int irValue = digitalRead(irMotionPin);
  int pirValue = digitalRead(pirMotionPin);

  Serial.print("IR Value: ");
  Serial.print(irValue);
  Serial.print(" | PIR Value: ");
  Serial.println(pirValue);

  if (irValue == HIGH && pirValue == HIGH)a {  // Ensure correct logic for PIR value
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump ON");
    delay(1000);  // Wait for 5 seconds
    digitalWrite(relayPin, HIGH);   
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Pump OFF"); 
  }

}
