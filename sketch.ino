int sensorPin = A0;
int relayPin = 8;

int lowTh = 300;   // Pump ON below this
int highTh = 700;  // Pump OFF above this

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Modbus-like frame fields
  byte address = 0x01;
  byte function = 0x03;
  byte dataHigh = highByte(sensorValue);
  byte dataLow  = lowByte(sensorValue);

  // Relay logic
  byte coilState;
  if (sensorValue < lowTh) {
    digitalWrite(relayPin, HIGH);   // Pump ON
    coilState = 0x01;
  } 
  else if (sensorValue > highTh) {
    digitalWrite(relayPin, LOW);    // Pump OFF
    coilState = 0x00;
  } 
  else {
    coilState = 0x00;
  }

  // Checksum (XOR method)
  byte checksum = address ^ function ^ dataHigh ^ dataLow ^ coilState;

  // Print Modbus-like packet in HEX
  Serial.print(": ");   // Start marker
  Serial.print(address, HEX); Serial.print(" ");
  Serial.print(function, HEX); Serial.print(" ");
  Serial.print(dataHigh, HEX); Serial.print(" ");
  Serial.print(dataLow, HEX); Serial.print(" ");
  Serial.print(coilState, HEX); Serial.print(" ");
  Serial.print(checksum, HEX);
  Serial.println();     // New line

  delay(500);
}
