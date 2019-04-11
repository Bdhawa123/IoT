// TEMPERATURE VARIABLES
int tempPin = 3;
unsigned int tempTimer; // define how long in between each temperature update
int tempInput; // store the input from the analog pin
double tempConverted; // store the converted temperature (from voltage to temperature)

// FAN VARIABLES
int relayPin = 3; // the digital pin that controls the relay

void setup()
{
  // GENERAL SETUP
  Serial.begin(9600); // start serial to be able to output the temp into console
  
  // TEMPERATURE SENSOR SETUP
  pinMode(A0, INPUT); // using analog pin 0 for temp input
  tempTimer = 1000;  // set the update rate to 10 seconds
  tempInput = 0;
  tempConverted = 0.0;

  // FAN SETUP
  pinMode(tempPin, OUTPUT);  // setup fan pin as output
  digitalWrite(tempPin, LOW); // start the fan in off mode
}

void loop()
{
  tempInput = analogRead(0);  // read the input from the pin
  tempConverted = (double) tempInput * (5/10.24); // convert voltage to temp. Got this from google
  delay(tempTimer);
  Serial.print("Temperature: " + (String) tempConverted + "\n");
  
  if(tempConverted > 25)
  {
    digitalWrite(relayPin, HIGH);
  }
  else
  {
    digitalWrite(relayPin, LOW);
    }
}
