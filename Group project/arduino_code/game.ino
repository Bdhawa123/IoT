#include <IRremote.h>
#include <stdio.h>
#include <time.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
int currentNumber = 0;
int buzzer = 12;

int laststored =  0;
int buzzedno = 0;
int level = 1;
int correct = 0;
String userid = "";       //for user to input 

int delayone = 300;
int delaytwo = 200;

long codes[12]=             // this array is used to store infrared codes
{
   0xFD30CF,0xFD08F7,               // 0 ,1
   0xFD8877,0xFD48B7,               // 2 ,3
   0xFD28D7,0xFDA857,               // 4 ,5
   0xFD6897,0xFD18E7,               // 6 ,7
   0xFD9867,0xFD58A7,               // 8 ,9
   0xFD20DF,0xFD609F,               // advance, move back
};

int number[10][8] =            //the array is used to store the number 0~9
{
  {0,0,0,1,0,0,0,1},//0
  {0,1,1,1,1,1,0,1},//1
  {0,0,1,0,0,0,1,1},//2
  {0,0,1,0,1,0,0,1},//3
  {0,1,0,0,1,1,0,1},//4
  {1,0,0,0,1,0,0,1},//5
  {1,0,0,0,0,0,0,1},//6
  {0,0,1,1,1,1,0,1},//7
  {0,0,0,0,0,0,0,1},//8
  {0,0,0,0,1,1,0,1} //9
};

void numberShow(int i) {                  //this function is used to display numbers
  for(int pin = 2; pin <= 9 ; pin++){
     digitalWrite(pin, number[i][pin - 2]);
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  irrecv.enableIRIn();

  for(int pin = 2 ; pin <= 9 ; pin++){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
  }

  UserID();
  Serial.print("User ID: ");
  Serial.println(userid);
  
  buzzes(delayone,delaytwo);
  randomSeed(analogRead(0));
}

/*
 * game logic
 * random number of blinks 
 * user enters the number through the remote 
 * if the number is correct
 *    another set of number of blinks is carried out
 *    the time of blink is shorter 
 *    when the user enters a wrong output, the game is finished
 *    the screen shows the number of right answers
 *
 */

void UserID() {
  while (userid.length()<3)
  {
    //Serial.print("Length userid: ");
    //Serial.println(userid.length());

    if (irrecv.decode(&results)) 
    {
      
        //Serial.println("First if");
        for(int i = 0; i <= 11; i++)
           {   
           //Serial.println("for statement");
           //Serial.println(results.value,HEX);
           if(results.value == codes[i]&& i <= 9){
                 numberShow(1);  // display number 0~9 on the digital control
                 userid += String(i);
                 //Serial.println(i);
                 numberShow(i); 
                 break;
                // Serial.print("Double Shit");
                 
           
           } 
                  
             //delay(200);
          
           }
           irrecv.resume();
           
         }
           
  }
}

void buzzes(int dl1, int dl2)
{
  //random numbers
  int blok = random(10);
  
  //int blok = rand() % 10;  
  laststored = blok; //setting the value of global variable to the current no of times of blink
  Serial.print("Random:");
  Serial.println(blok);
  

  for (int n=0; n<blok; n++)
  {
    tone(buzzer, 500);
    delay(dl1);
    noTone(buzzer);
    delay(dl2);
    
  }
  
}


void loop() {
  
 
  if (irrecv.decode(&results )&& userid!="") 
  {
      for(int i = 0; i <= 11; i++){

         if(results.value == codes[i]&& i <= 9){
               //numberShow(8);  // display number 0~9 on the digital control
               currentNumber = i;
               //Serial.println(i); //selected no 

               if (laststored==currentNumber)
               {
                  Serial.println("Congrats you're roght");
                  buzzes(delayone,delaytwo);
                  correct++;
                  Serial.println("correct No ");
                  Serial.println(correct);
                  if (correct==3)
                  {
                    correct= 0;
                    level++;
                    delayone = delayone/2;
                    delaytwo = delaytwo/2; 
                    
                  }       
               }  
               else
               {
                  Serial.println("Haha You're wrong");
                  level = 0;
                  correct =0;
               }

          
               
                 break;
         }
        
    }

   Serial.println(results.value, HEX);
   irrecv.resume();
  }
}
