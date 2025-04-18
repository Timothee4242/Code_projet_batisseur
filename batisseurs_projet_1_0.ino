#include <LiquidCrystal.h>//écran LCD

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //pins utilisés (?)


//Variables pour le joystick:
bool inputs[5]={false,false,false,false,false}; //haut, bas, droite, gauche, boutton
bool input_zero=false; //indique si on est passé par 0 (uniquement pour la fonction majinput()
bool input_sw_zero=false; //de mme pour le bouton
unsigned long dateappui; //détecte un long appui (enregistre supermillis())
const int tempsmax = 1500;//temps avant répétition de l'appui
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

//Gestion du programme
int modes[4]={0,0,1,2};//mode actuel, 3 modes programmés --> 0=auto, 1=off, 2=on
int modeactif=-1; //mode en cours de réglage --> -1=rien, 0;1;2;3=modes0,1,2,3, 4=réglage luminosité
int reglagetype=-1; //-1=juste séléctionné, 0=mode/lum_min, 1=heures/lum_max, 2=minutes
unsigned long modes_date[3]={1,3500000,6000000};//date d'activation des modes
bool lampe=false;
int lum_min=30;//points de luminosité critique où la lampe s'éteint / s'allume (mode auto)
int lum_max=60;
int lum=50;//luminosité
const int lampe_pin=13;

void setup(){
  pinMode(SW_pin, INPUT);
  pinMode(lampe_pin, OUTPUT);
  digitalWrite(SW_pin, HIGH);
  lcd.begin(16,2);//16 colonnes, 2 lignes
  lcd.setCursor(0,0);//colonne 0, ligne 0
  lcd.print("                ");//vide l'écran sur la ligne 1
  Serial.begin(9600);
  Serial.println("hallo");
}

void loop(){
  majinput();
//  Serial.print(modes_date[0]);Serial.print(", ");Serial.print(modes_date[1]);Serial.print(", ");Serial.print(modes_date[2]);Serial.print(" .   ");/**/
  if (reglagetype==-1){
    if(inputs[2]){modeactif++;}
    if(inputs[3]){modeactif-=1;}
    if(modeactif==-2){modeactif=4;}
    if(modeactif==5){modeactif=0;}
    if (inputs[4] && modeactif!=-1){
      reglagetype=0;
      if(modeactif!=0){if(modes_date[modeactif-1]<millis()){modes_date[modeactif-1]=millis()+60000;}}
    }
  }
  else{
    if(inputs[2]){reglagetype++;}
    if(inputs[3]){reglagetype--;}
      if(modeactif==0){reglagetype=0;}
      else{if(modeactif==4){
        if(reglagetype==-1){reglagetype=1;}
        if(reglagetype==2){reglagetype=0;}}
      else{
        if(reglagetype==-1){reglagetype=3;}
        if(reglagetype==4){reglagetype=0;}}}
    if (inputs[4]){reglagetype=-1;}
  }
  if(reglagetype!=-1 && modeactif!=-1){
    if(inputs[0]){
      if(modeactif==4){if(reglagetype==0){lum_min++;}else{lum_max++;}}
      else{
        if(reglagetype==0){modes[modeactif]++;if(modes[modeactif]==3){modes[modeactif]=0;}}
        if(reglagetype==1){modes_date[modeactif-1]+=3600000;}
        if(reglagetype==2){modes_date[modeactif-1]+=60000;}
        if(reglagetype==3){modes_date[modeactif-1]+=1000;}
      }
    }
    if(inputs[1]){
      if(modeactif==4){if(reglagetype==0){lum_min--;}else{lum_max--;}}
      else{
        unsigned long a=modes_date[modeactif-1]-millis();
        if(reglagetype==0){modes[modeactif]--;if(modes[modeactif]==-1 && modeactif==0){modes[modeactif]=0;}}
        if(reglagetype==1){modes_date[modeactif-1]-=3600000;}
        if(reglagetype==2){modes_date[modeactif-1]-=60000;}
        if(reglagetype==3){modes_date[modeactif-1]-=1000;}
      //  if(a<modes_date[modeactif-1]-millis()){modes_date[modeactif-1]=millis();}
      }
    }
  }
  for (int i=0; i<3; i++){
    if(modes_date[i]<millis() && modes[i+1]!=-1){
      modes[0]=modes[i+1];
      modes[i+1]=-1;
    }
  }
  if(modes[0]==1){lampe=false;}
  if(modes[0]==2){lampe=true;}
  if(modes[0]==0){
    if(lum>lum_max){lampe=false;}
    if(lum<lum_min){lampe=true;}
  }
  digitalWrite(lampe_pin, lampe);
  //Serial.println(modeactif);
  majaffiche();
  delay(50);
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
    dateappui=0b1000000000000000000000000000000;
    //Serial.println("a");
  }
  else{if (input_zero || millis()-dateappui>tempsmax){
    //Serial.println("b");
    if(Y<150){inputs[0]=true;}
    else{if(Y>850){inputs[1]=true;}
    else{if(X<150){inputs[3]=true;}
    else{if(X>850){inputs[2]=true;}}}}
    if (input_zero){dateappui=millis();}
    input_zero=false;
  }
  }
 // Serial.print(Y);
  //Serial.print(", ");
  //Serial.println(input_zero);
}
void majaffiche(){
  /*Serial.print(inputs[0]);
  Serial.print(", ");
  Serial.print(inputs[1]);
  Serial.print(", ");
  Serial.print(inputs[2]);
  Serial.print(", ");
  Serial.print(inputs[3]);
  Serial.print(", ");
  Serial.println(inputs[4]);*/
  lcd.setCursor(0,0);lcd.print("                ");lcd.setCursor(0,1);lcd.print("                ");
  lcd.setCursor(0,0);
  if(modes[0]==0){lcd.print("A");}
  if(modes[0]==1){lcd.print("O");}
  if(modes[0]==2){lcd.print("I");}
  lcd.setCursor(0,1);
  if(lampe){lcd.print("I");}
  else{lcd.print("O");}
  
  lcd.setCursor(2,0);
  if(modeactif==-1 || reglagetype==-1){
    int a,b;
    for (int i=0; i<3; i++){
      if(i==0){a=2;b=0;}
      if(i==1){a=9;b=0;}
      if(i==2){a=2;b=1;}
      lcd.setCursor(a,b);
      if(modes[i+1]==-1){lcd.print("----- ");}else{
      if ((modes_date[i]-millis())>3600000){
        lcd.print(int((modes_date[i]-millis())/3600000));
        lcd.print("h");
      }
      else{if ((modes_date[i]-millis())>60000){
        lcd.print(int((modes_date[i]-millis())/60000));
        lcd.print("'");}
      else{
        lcd.print(int((modes_date[i]-millis())/1000));
        lcd.print('"');
      }}
      lcd.setCursor(a+4,b);lcd.print(":");
      if(modes[i+1]==0){lcd.print("A");}
      if(modes[i+1]==1){lcd.print("O");}
      if(modes[i+1]==2){lcd.print("I");}
      lcd.print(" ");
    }} 

    lcd.setCursor(9,1);
    lcd.print("Lum ");
    lcd.print(lum);
  }//fin de l'affichage de base
  //object select. clignote
  if (modeactif!=-1 && reglagetype==-1){
    if (modeactif==0){lcd.setCursor(0,0);}
    if (modeactif==1){lcd.setCursor(2,0);}
    if (modeactif==2){lcd.setCursor(9,0);}
    if (modeactif==3){lcd.setCursor(2,1);}
    if (modeactif==4){lcd.setCursor(9,1);}
    if (millis()%1000<500){                //modifier ici la fréquence
      if(modeactif==0){lcd.print(" ");}
      else{lcd.print("       ");}
    }
  }
  //réglages
  if(modeactif!=-1 && reglagetype!=-1){
    if(modeactif==4){
      lcd.setCursor(3,0);
      lcd.print("L-min: ");
      if(reglagetype==1 || millis()%700<500){lcd.print(lum_min);}
      lcd.setCursor(2,1);lcd.print(lum);
      lcd.setCursor(6,1);
      lcd.print("L-max: ");
      if(reglagetype==0 || millis()%700<500){lcd.print(lum_max);}
    }
    else{
      lcd.setCursor(3,0);
      //lcd.print("Mode ");
      if(modeactif==0){
        lcd.print("Mode actuel: ");
        lcd.setCursor(4,1);
        if(modes[modeactif]==0){lcd.print("Automatique");}
        if(modes[modeactif]==1){lcd.print("Manuel - OFF");}
        if(modes[modeactif]==2){lcd.print("Manuel - ON");}
      }
      else{
        lcd.print("Programme ");lcd.print(modeactif);
        lcd.setCursor(3,1);
        if (reglagetype!=0 || millis()%700<500){
          if(modes[modeactif]==0){lcd.print("A");}
          if(modes[modeactif]==1){lcd.print("O");}
          if(modes[modeactif]==2){lcd.print("I");}
        }else{lcd.print(" ");}  //if (modeactif!=0){}
        lcd.print("- ");
        lcd.setCursor(6,1);
        if(reglagetype!=1 || millis()%700<500){
          lcd.print(int((modes_date[modeactif-1]-millis())/3600000));
          lcd.print("h");}
        lcd.setCursor(10,1);
        if(reglagetype!=2 || millis()%700<500){
          lcd.print(int((modes_date[modeactif-1]-millis())/60000)%60);
          lcd.print("'");}
        lcd.setCursor(13,1);
        if(reglagetype!=3 || millis()%700<500){
          lcd.print(int((modes_date[modeactif-1]-millis())/1000)%60);
          lcd.print('"');}
      }
    }
  }
  
}
