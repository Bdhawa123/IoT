#include <IRremote.h>
#include <stdio.h>
#include <time.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
int currentNumber = 0;
int buzzer = 12;
int tripleled; // rgb light 
int analog_bl = 0; // blue
int analog_rd = 1;  //red
int analog_gr = 2; // green

int blok; //random number
int laststored =  0; //last stored no 
int level = 1;
int correct = 0;
String userid = "";

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
  randomSeed(analogRead(0));  //setting random  
  
  pinMode(A0,OUTPUT); //blue
  pinMode(A1,OUTPUT);//red
  pinMode(A2,OUTPUT);//green
  pinMode(buzzer,OUTPUT); //buzzer
  irrecv.enableIRIn();  

  for(int pin = 2 ; pin <= 9 ; pin++){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
  }

   digitalWrite(A0, HIGH); 
    UserID();
  digitalWrite(A0,LOW);
  delay(1000);
  
  //starts buzzer on purple
  digitalWrite(A0,HIGH);
  digitalWrite(A1,HIGH);
  
  Serial.print("User ID: ");
  Serial.println(userid);
  //rgb green about to start 
  //delay timer for 3 sec 

  buzzes(delayone,delaytwo);
  digitalWrite(A1,LOW);

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
      
        for(int i = 0; i <= 11; i++)
           {   
         
           if(results.value == codes[i]&& i <= 9){
                 numberShow(1);  // display number 0~9 on the digital control
                 userid += String(i);
                 
                 numberShow(i); 
                 break;
              } 
           }
           irrecv.resume();           
         }           
  }
}

void buzzes(int dl1, int dl2)
{
  //turn all the lights off
    digitalWrite(A0,LOW);
    digitalWrite(A1,LOW);
    digitalWrite(A2,LOW);
    

  //turns purple to let it buzz
    digitalWrite(A0,HIGH);
    digitalWrite(A1,HIGH);
  //random numbers
  blok = random(10);
  
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

    digitalWrite(A1,LOW);//blue on red off
    //digitalWrite(A1,LOW); 
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
                  Serial.println("Congrats you're right");
                  digitalWrite(A0,LOW); //blue down
                  digitalWrite(A2,HIGH);//green up correct
                  delay(500);
                  correct++;
                  
                  
                  Serial.println("correct No ");
                  Serial.println(correct);

                  numberShow(blok);

                 
                  //rgb green for printing correct answer
                  
                  if (correct==3)
                  {
                    correct= 0; //reset correct counter
                    level++;    //level up 

              
                   digitalWrite(A0,LOW);
                     
                    digitalWrite(A0,HIGH);
                    digitalWrite(A2,HIGH);
                    digitalWrite(A1,LOW);
                 
                    delay(3000);
                    
                    delayone = 300/level;
                    delaytwo = 200/level; 
                    ///rgb green blink 5 times rapid succession  next level
                  }
                  buzzes(delayone,delaytwo);       
               }  
               else
               {
                  Serial.println("Haha You're wrong");
                  level = 0;
                  correct =0;
                  delayone = 300;
                  delaytwo = 200;
                
                  ///rgb red wrong answer 
                  digitalWrite(A0, LOW); //blue down 
                  digitalWrite(A1,HIGH); // red up wrong
                  delay(500);
                  numberShow(blok);
               }
               
                 break;
         }
        
    }

   Serial.println(results.value, HEX);
   irrecv.resume();
  }
}
