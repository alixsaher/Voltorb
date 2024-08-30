#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#define DHTPIN 7      
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD with I2C address 0x27 and dimensions 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);  

int gas_sensorValue;
int h;  
int t; 
int moist_sensor_pin = A1;
int moist_output_value;
const int ldrPin = A2;  // Define the analog pin where the LDR is connected
int ldrValue = 0; 
int relay_pin = 3; 

void setup() {
  lcd.init();            
  lcd.backlight();
  
  dht.begin();   

  Serial.begin(9600);     // Start serial communication at 9600 baud rate
}

void loop() {
  gas_sensorValue = analogRead(A0); 
  h = dht.readHumidity();
  t = dht.readTemperature(); 
  moist_output_value = analogRead(moist_sensor_pin);
  moist_output_value = map(moist_output_value, 550, 0, 0, 100); 
  ldrValue = analogRead(ldrPin);
  ldrValue = map(ldrValue, 1020, 0, 80, 100);

  pinMode(relay_pin, OUTPUT);
  if (moist_output_value <-10) {
    digitalWrite(relay_pin, LOW); 
  } else {
    digitalWrite(relay_pin, HIGH); 
  }

  delay(1000);

  Serial.print("AirQua=");
  Serial.print(gas_sensorValue);       // Print the value in decimal format
  Serial.println(" PPM");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%, Temp: ");
  Serial.print(t);
  Serial.println(" ° Celsius");
  Serial.print("Moisture:");
  Serial.print(moist_output_value);
  Serial.println("%");
  Serial.print("LDR Value: ");
  Serial.println(ldrValue); 

 lcd.setCursor(0, 1);            
 lcd.print("ArQ=");               // Print "ArQ=" on the first line
 lcd.print(gas_sensorValue);  
 lcd.print("PPM ");          // Print the sensor value on the first line
   
 
 lcd.setCursor(8, 0);
 lcd.print("Sun:");+++++++++++++++++++++++
 
 lcd.print(ldrValue);
 lcd.print("");         

 lcd.setCursor(0, 0);             // Move to the second line
 lcd.print("T:");                 // Print "T:" on the second line
 lcd.print(t);                    // Print the temperature value
 lcd.print("C  ");                // Ensure space for "C" and clear any leftover characters

 lcd.setCursor(10, 1);             // Move the cursor to the middle of the second line
 lcd.print("H:");                 // Print "H:" on the second line
 lcd.print(h);                    
 lcd.print("%   "); 


  delay(2000);                     // Wait 2 seconds before the next reading
}
