int waterSensorPin = A0;
int blueLedPin = 13;
int greenLedPin = 12;
int Delay = 100;
String readString;

// The setup function runs once, at the beginning:
void setup() {
  // Serial. begin() take the argument data rate (baud rate), in bits per second. This is the speed of the serial 
  // communication:
  Serial.begin(9600);
  // pinMode() set a specific pin to act either as an input or as an output:
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin,OUTPUT);
}

// The loop function runs repeatedly:
void loop() {
  // analogRead - as the name implies, read analog data from the specified pin number.
  int WaterSensorValue=analogRead(waterSensorPin);
  // We must print the data to the serial port so that the Python program can read it:
  Serial.println(WaterSensorValue);
  char readChar;
  if (Serial.available() > 0) // If there are bytes available to read on the Arduino's serial port:
  {
    readChar = Serial.read(); // Read the serial data waiting in the serial buffer.
    readString += readChar;
    if (readString[0] == 'l')
    {
      // Before setting the blue led to blink we must make sure that the green led is turned off (in case the water 
      // level was higher than '450' beforehand).
      digitalWrite(greenLedPin,LOW);
      // digitalWrite set a pin value to be high or low. The pins in this sketch are set to be output pins, so writing
      // high will set the voltage in this pin to 5v, and writing low will set it to 0v:
      digitalWrite(blueLedPin, HIGH);
      // The delay method pauses the program for the amount specified in milliseconds. This delay is what causes the 
      // led to blink, without it it would turn from on to off instantly (and invisibly).
      delay(15);
      digitalWrite(blueLedPin, LOW);
      readString = "";
    }
    else if (readString[0] == 'm')
    {
      digitalWrite(greenLedPin,LOW);
      digitalWrite(blueLedPin,HIGH);
      readString = "";
    }
    else if (readString[0] == 'h')
    {
      digitalWrite(blueLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
      readString = "";
    }
    else {}
  }
  readString = "";
  // We need a delay at the end of each loop because without it the microcontroller runs too fast and data is not
  // transferred correctly to the Python program.
  delay(Delay);
}
