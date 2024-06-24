#include <LiquidCrystal.h>      // Header file for LCD
#include <dht.h>                // Header file for DHT22

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // pins of LCD connected to Arduino
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // lcd function from LiquidCrystal

int buz = 8;  // buzzer connected to pin 8
int led = 9;  // led connected to pin 9

const int aqsensor = A0;  // output of mq135 connected to A0 pin of Arduino
const int dhtPin = A1;   // DHT22 connected to A1 pin of Arduino
dht DHT;                 // DHT22 object

const int bluetoothTx = 1;  // HC-06 Bluetooth TX connected to Arduino pin 1
const int bluetoothRx = 0;  // HC-06 Bluetooth RX connected to Arduino pin 0

const int threshold = 250;  // Threshold level for Air Quality

void setup() {
  pinMode(buz, OUTPUT);     // buzzer is connected as Output from Arduino
  pinMode(led, OUTPUT);     // led is connected as output from Arduino
  pinMode(aqsensor, INPUT); // MQ135 is connected as INPUT to arduino
  pinMode(dhtPin, INPUT);   // DHT22 is connected as INPUT to arduino

  Serial.begin(9600);      // begin serial communication with baud rate of 9600
  lcd.clear();              // clear lcd
  lcd.begin(16, 2);         // consider 16,2 lcd
}

void loop() {
  int ppm = analogRead(aqsensor); // read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");  // print message in serial monitor
  Serial.println(ppm);            // print value of ppm in serial monitor

  lcd.setCursor(0, 0);             // set cursor of lcd to 1st row and 1st column
  lcd.print("Air Qualit: ");      // print message on lcd
  lcd.print(ppm);                 // print value of MQ135

  int chk = DHT.read11(dhtPin);
  float temp = DHT.temperature;
  float hum = DHT.humidity;

  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(hum);

  lcd.setCursor(0, 1);             // set cursor of lcd to 2nd row and 1st column
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C ");
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print("%");

  if (ppm > threshold) {
    lcd.setCursor(1, 1);         // jump here if ppm is greater than threshold
    lcd.print("AQ Level HIGH");
    Serial.println("AQ Level HIGH");
    tone(led, 1000, 200);         // blink led with turn on time 1000mS, turn off time 200mS
    digitalWrite(buz, HIGH);     // Turn ON Buzzer
  } else {
    digitalWrite(led, LOW);   // jump here if ppm is not greater than threshold and turn off LED
    digitalWrite(buz, LOW);   // Turn off Buzzer
    lcd.setCursor(1, 1);
    lcd.print("AQ Level Good");
    Serial.println("AQ Level Good");
  }

  // Send data to Bluetooth module
  Serial.print("AT+SEND=0,");  // Send data to connected device
  Serial.print("Air Quality: ");
  Serial.print(ppm);
  Serial.print(", Temperature: ");
  Serial.print(temp);
  Serial.print(", Humidity: ");
  Serial.println(hum);
  delay(500);
}