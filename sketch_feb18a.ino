#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
float a, b, c, d, e, f, g;
#define rs 53
#define en 51
#define d4 49
#define d5 47
#define d6 45
#define d7 43
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String receivedData = "";
int pH;
int W;
int L;
int T;
//SoftwareSerial gsm(11, 12);
int ldr_pin = A3;  // Assuming you're using an analog pin for the LDR
int light_pin = 13;
const int dallasTempPin = 3;
int gas = A2;
int buzzer = 8;
String pHval;
String msg;
String uno;
OneWire oneWire(dallasTempPin);
DallasTemperature tempSensor(&oneWire);
float tempC;
DeviceAddress insideThermometer;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  tempSensor.begin();
  //gsm.begin(9600);

  pinMode(ldr_pin, INPUT);
  pinMode(gas, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(light_pin, OUTPUT);

  digitalWrite(light_pin, HIGH);
  digitalWrite(buzzer, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enhancing milk ");
  lcd.setCursor(0, 1);
  lcd.print("Quality");
  delay(1000);
}

void loop() {
  String data1 = "";

  if (Serial.available() > 0)  //If data is available on serial port
  {
    lcd.clear();
    lcd.print("Waiting for ML data");
    data1 = Serial.readString();  //Print character received on to the serial monitor
    // Check if the character 'a' is present in the received data and it's not a single 'a'
    if (data1.indexOf('t') != -1 && data1.length() > 1) {
      //            Serial.println(data1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RECEIVE DATA:)");
      lcd.setCursor(0, 1);
      lcd.print(data1);
      delay(2000);
      int indexA = data1.indexOf("t") + 1;
      int indexB = data1.indexOf("u") + 1;
      int indexC = data1.indexOf("v") + 1;
      int indexD = data1.indexOf("w") + 1;
      int indexE = data1.indexOf("x") + 1;
      int indexF = data1.indexOf("y") + 1;
      int indexG = data1.indexOf("z") + 1;
      delay(500);
      String valueA = data1.substring(indexA, indexB - 1);
      String valueB = data1.substring(indexB, indexC - 1);
      String valueC = data1.substring(indexC, indexD - 1);
      String valueD = data1.substring(indexD, indexE - 1);
      String valueE = data1.substring(indexE, indexF - 1);
      String valueF = data1.substring(indexF, indexG - 1);
      String valueG = data1.substring(indexG);
      a = valueA.toInt();
      b = valueB.toInt();
      c = valueC.toInt();
      d = valueD.toInt();
      e = valueE.toInt();
      f = valueF.toInt();
      g = valueG.toInt();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RECEIVE DATA:)");
    lcd.setCursor(0, 1);
    lcd.print(data1);
    delay(2000);
    if (a == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" spoiled milk");
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      msg = "spoiled milk detected";
      SendMessage();
      delay(1000);
    } else if (b == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("low density milk");
lcd.setCursor(0, 1);
      lcd.print("spoiled milk");
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      msg = "low density milk";
      SendMessage();
      delay(1000);
    } else if (c == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("high temp");
      lcd.setCursor(0, 1);
      lcd.print("spoiled milk");
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      msg = "high temperature";
      SendMessage();
      delay(1000);
    } else if (d == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("abnormal ph");
       lcd.setCursor(0, 1);
      lcd.print("spoiled milk");
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer, LOW);
      msg = "abnormal ph!";
      SendMessage();
      delay(1000);
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("fresh milk");
      delay(2000);
      msg = "fresh milk";
      SendMessage();
      delay(1000);
    }
  }
  // Temperature Reading
  tempSensor.requestTemperatures();
  tempC = tempSensor.getTempCByIndex(0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  delay(2000);

  // LDR value reading (Fat/Thickness Calculation)
  int ldrValue = analogRead(ldr_pin);                  // Read analog value from LDR pin
  //int fatPercentage = map(ldrValue, 0, 1023, 0, 100);  // Map LDR value to fat percentage

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LDR Value: ");
  lcd.print(ldrValue);

  // lcd.setCursor(0, 1);
  // lcd.print("Fat %: ");
  // lcd.print(fatPercentage);  // Display the mapped value (fat percentage)
   delay(2000);

  // Gas value reading (spoiled milk detection)
  int gasValue = analogRead(gas);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Value: ");
  lcd.print(gasValue);
  delay(1000);

  if (Serial2.available()) {
    // Read the incoming data
    receivedData = Serial2.readString();
    // Process the data once a full string is received
    parseData(receivedData);
    // Clear the buffer for the next data
    receivedData = "";
    // Serial.print("pH: ");
    // Serial.print(pH);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pH: ");
  lcd.print(pH);
  delay(1000);



  // pH value reading (Milk pH level)
  if (pH < 15) {

    String uno = "a" + String(gasValue) + "b" + String(ldrValue) + "c" + String(tempC) + "d" + String(pH) + "e";
    Serial.println(uno);
    delay(1000);
  }
}

void SendMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message sending........");
  Serial1.println("Setting the GSM in text mode");
  Serial1.println("AT+CMGF=1\r");
  delay(2000);
  Serial1.println("Sending SMS to the desired phone number!");
  Serial1.println("AT+CMGS=\"+917032534707\"\r");
  // Replace x with mobile number
  delay(2000);
  Serial1.println(msg);  // SMS Text
  delay(2000);
  Serial1.write(26);  // ASCII code of CTRL+Z
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message sent.......");
  delay(1000);
}
void parseData(String data) {
  // Example incoming data: "PH:37.76, W: 0, L: 128, T:  98"

  // Find the positions of the delimiters
  int pHIndex = data.indexOf("PH:");
  int WIndex = data.indexOf("W:");
  int LIndex = data.indexOf("L:");
  int TIndex = data.indexOf("T:");

  // Extract and convert each value
  pH = data.substring(pHIndex + 3, data.indexOf(",", pHIndex)).toFloat();
  W = data.substring(WIndex + 2, data.indexOf(",", WIndex)).toInt();
  L = data.substring(LIndex + 2, data.indexOf(",", LIndex)).toInt();
  T = data.substring(TIndex + 2).toInt();  // Since T is the last value, no comma is needed

  // Print the values to verify
  // Serial.print("pH: ");
  // Serial.print(pH);
  // Serial.print(" W: ");
  // Serial.print(W);
  // Serial.print(" L: ");
  // Serial.print(L);
  // Serial.print(" T: ");
  // Serial.print(T);
}
