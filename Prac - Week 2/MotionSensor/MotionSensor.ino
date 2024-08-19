const int SENSOR_PIN = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(SENSOR_PIN) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("We have motion!");
  }
  else // if (digitalRead(SENSOR_PIN) == LOW)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("There is no motion");
  }
  delay(1000);
}
