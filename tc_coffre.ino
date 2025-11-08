//#include <LiquidCrystal.h>//écran LCD

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //pins utilisés (?)


//Variables pour le joystick:
bool inputs[5]={false,false,false,false,false}; //haut, bas, droite, gauche, boutton
bool input_zero=false; //indique si on est passé par 0 (uniquement pour la fonction majinput()
bool input_sw_zero=false; //de mme pour le bouton
const int tempsmax = 1500;//temps avant répétition de l'appui
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

//variables pour les OUTPUT
const int led1 = 4;
const int led2 = 7;
const int led3 = 3;
unsigned long timeLed2Off = 0;
unsigned long timeLed3Off = 0;
//servo
#include <Servo.h>
Servo myservo;  // create servo object to control a servo


//Gestion du programme
const int lampe_pin=13;
bool isopen = false;
bool enteringCode = false;
int code[16] = {0,3,1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int codesize = 4;
int codeInput[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};


void setup(){
  pinMode(SW_pin, INPUT);
  pinMode(lampe_pin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  myservo.attach(9);
  close();
  digitalWrite(SW_pin, HIGH);
 /* lcd.begin(16,2);//16 colonnes, 2 lignes
  lcd.setCursor(0,0);//colonne 0, ligne 0
  lcd.print("                ");//vide l'écran sur la ligne 1*/
  Serial.begin(9600);
  Serial.println("hallo");
}

void loop(){
  majinput();
  if (inputs[4]){
    if (enteringCode){for(int i=0;i<16;i++){codeInput[i]=-1;}}
    enteringCode = !enteringCode;}
  
  if (enteringCode && isopen == false){
    int d=-1;
    if (inputs[0]){d=0;}
    if (inputs[1]){d=1;}
    if (inputs[2]){d=2;}
    if (inputs[3]){d=3;}
    if (d!=-1){
      int rank=0;
      while (codeInput[rank]!=-1){rank++;if(rank==16){rank=0;}}
      codeInput[rank] = d;
      if (rank == codesize -1){
        bool ok=true;
        for(int i=0;i<codesize;i++){if(code[i]!=codeInput[i]){ok=false;}}
        if (ok){
          isopen = true;
          digitalWrite(led2,true);
          open();
          timeLed2Off = millis()+2000;
        }
        for(int i=0;i<16;i++){codeInput[i]=-1;}
        enteringCode = false;
      }
      else{timeLed3Off=500;}
    }
  }
  else{if(! enteringCode && inputs[4] && isopen == true){isopen = false;close();}}

  majaffiche();
  delay(10);
/**/}

void majinput(){
  inputs[0]=false;inputs[1]=false;inputs[2]=false;inputs[3]=false;inputs[4]=false;//={false,false,false,false,false};
  if (digitalRead(SW_pin)==0){
    if (input_sw_zero){
      inputs[4]=true;
      input_sw_zero=false;
      //Serial.println("a");
    }
  }
  else{
    input_sw_zero=true;
    //Serial.println("b");
  }
 // Serial.print("2: ");
  //affiche();
  int X=analogRead(X_pin);
  int Y=analogRead(Y_pin);
  if (300<X && X<700 && 300<Y && Y<700){
    input_zero=true;
    //Serial.println("a");
  }
  else{if (input_zero){
    //Serial.println("b");
    if(Y<150){inputs[0]=true;}
    else{if(Y>850){inputs[1]=true;}
    else{if(X<150){inputs[3]=true;}
    else{if(X>850){inputs[2]=true;}}}}
    input_zero=false;
  }
  }
 // Serial.print(Y);
  //Serial.print(", ");
  //Serial.println(input_zero);
}

void majaffiche(){
  for(int i=0;i<16;i++){ Serial.print(codeInput[i]);Serial.print("; ");}Serial.println(isopen);
  digitalWrite(led1,enteringCode);
  digitalWrite(led2,timeLed2Off > millis());
  digitalWrite(led3,timeLed3Off > millis());

}

void open(){
 /* for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }//*/
  myservo.write(100);
}
void close(){
  /*for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }//*/
  myservo.write(2);
}

