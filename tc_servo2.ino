//#include <LiquidCrystal.h>//écran LCD

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //pins utilisés (?)


//Variables pour le joystick:
float inputs[5]; //position du joystick: 1 vertical, 1 horizontal, 2 vertical, 2 horizontal
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin1 = A0; // analog pin connected to X output
const int Y_pin1 = A1; // analog pin connected to Y output
const int X_pin2 = A2; // analog pin connected to X output
const int Y_pin2 = A3; // analog pin connected to Y output

//variables pour les OUTPUT
// connexions de la carte pwm servo controller: GND -> GND, OE rien, SCL -> A5, SDA -> A4, VCC -> + de la carte (3.3 ou 5V), V+ -> alim (5V de la carte sur le pin ou alim 5V sur l'entrée verte)
//#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
const float pas_max = 10;

//position des servo
const int nbservo = 4;
float position[nbservo];


//Gestion du programme
void setup(){
  pinMode(SW_pin, INPUT);
  pwm.begin();
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  Serial.println("hallo");
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  for(int i =0; i<nbservo;i++){position[i]=SERVOMIN;}
  majpos();

}

void loop(){
  majinput();
  for(int i=0;i<nbservo;i++){
    if(inputs[i]!=0){position[i]+=inputs[i]*pas_max/500;}
    position[i] = clamp(position[i],SERVOMIN, SERVOMAX);
  }
  majpos();
  delay(10);
/**/}

void majinput(){
  for(int i=0;i<nbservo;i++){inputs[i]=0;}

  int X=analogRead(X_pin1);
  int Y=analogRead(Y_pin1);
  if(Y<450||Y>550){inputs[0]=Y-500;}
  if(X<450||X>550){inputs[1]=X-500;}
  X=analogRead(X_pin2);
  Y=analogRead(Y_pin2);
  if(Y<450||Y>550){inputs[2]=Y-500;}
  if(X<450||X>550){inputs[3]=X-500;}
}

void majpos(){
  for(int i=0;i<nbservo;i++){
    pwm.setPWM(i,0,position[i]);
  }
}

float clamp(float a, float b, float c){
  return max(b,min(a,c));
}