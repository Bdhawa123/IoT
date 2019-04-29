// pin numbers
int tempPin = 0;
int fanPin = 3;
int buzzerPin = 8;
int irReceiverPin = 10;
int buttonPin = 12;

int gameMode; // 0 is temperature sensor, 1 is memory game

#define temperatureSensor 0
#define memoryGame 1

int ON = LOW;
int OFF = HIGH;

float temperature;

void setup() 
{
  // temperature setup
  pinMode(tempPin, INPUT);

  // fan setup
  pinMode(fanPin, OUTPUT);

  // buzzer setup
  pinMode(buzzerPin, OUTPUT);

  // button setup
  pinMode(buttonPin,INPUT);

  // output setup
  Serial.begin(9600);

  // gameMode setup
  int gameMode = 0; // start in temperature sensor mode
}

void loop() 
{   
  // check the current gameMode then run required code
  switch(gameMode)
  {
    case temperatureSensor:
    
      readTemp();
      break;
  
    case memoryGame:
    
      game();
      break; 
  } 
}

void readTemp()
{
  // read the voltage from the pin
  temperature = analogRead(tempPin);
  
  // convert to celsius
  temperature = (temperature/1024.0)*500;
  
  // print the temperature
  Serial.println(temperature);

  delay(2000);

  // check if the mode button has been pressed
  checkButton();
}

void game()
{
  // this is where we need to past in memory game logic
  // the logic to store the user scores will be handled by Python Flask
  // currently it just prints "playing game" to simulate
  Serial.println("Playing game");
  delay(2000);

  // check if the mode button has been pressed
  checkButton();
}

void checkButton()
{
  // store the state of the button (HIGH or LOW)
  int val = digitalRead(buttonPin);

  // if it has been pressed, enter loop
  if(val == HIGH)
  {
    // if gamemode = 0, new gamemode will = 1, vice versa
    gameMode = 1 - gameMode;

    // play the switch tone to let user know
    gameSwitchTone();
  }
}

void gameSwitchTone()
{
  // loop used to play the tone twice
  for(int i = 0; i <= 1; i++)
  {
    tone(buzzerPin, 1000);
    delay(250);
    noTone(buzzerPin);
  }
}
