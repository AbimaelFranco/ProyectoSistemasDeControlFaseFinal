#include <Servo.h>
#define motor 10
#define motor2 11
#define Boton_incremento 23

Servo servoMotor1;
Servo servoMotor2;

const int Buzzer = 2;
const int Push = 3;
const int Led = 4;
const int Trigger = 5;
const int Echo = 6;
const int sensorPin = 7;
const int boton_motor= 22;
const int boton_motor2= 23;



long t; //timepo que demora en llegar el eco
long distanciactual; //distancia en centimetros
bool PushState;
bool MotorState;
bool MotorState2;
int SystemState;    //0: rampa arriba-bajando  1: rampa abajo-subiendo
int angle;
bool boton1;
int aux;
int IFState = 1;

int aux2;


void Ultrasonico();
void Alerta();

void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación

  pinMode(Buzzer, OUTPUT); //pin como salida
  pinMode(Push, INPUT); //pin como salida
  pinMode(Led, OUTPUT); //pin como salida
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  pinMode(boton_motor, INPUT);
  pinMode(boton_motor2, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(Boton_incremento, INPUT);
  
  servoMotor1.attach(9);
  servoMotor2.attach(8);
  
  digitalWrite(Trigger, LOW);
  digitalWrite(Buzzer, LOW);
  digitalWrite(Boton_incremento, LOW);
  servoMotor1.write(180);
  servoMotor2.write(0);
  SystemState=0;
  t=0;
  distanciactual=0;
  aux = 0;
  aux2 = 0;
}

void loop(){

  PushState = digitalRead(Push);
  MotorState = digitalRead(boton_motor);
  MotorState2 = digitalRead(boton_motor2);
  IFState = digitalRead(sensorPin);
  boton1 = digitalRead(Boton_incremento);
  
  Serial.print("Rampa en: " );
  Serial.println(SystemState);
  Serial.print("Aux en: " );
  Serial.println(aux);


  if ( MotorState == true){
    aux = 1;
    aux2 = 0;
    Serial.println(aux);
    }
  else if ( IFState == LOW){
    aux = 0;  
    Serial.println(aux);
    }

  if (MotorState2 == true){
    aux2 = 1;
  }
    
  if (SystemState==1 && aux == 1){
    analogWrite(motor,255);
    delay(4.25);
    analogWrite(motor,30);
    }
  else if ( SystemState==1 && aux == 0){
    analogWrite(motor, 0);
    }


  if (SystemState==1 && aux == 0 && aux2 == 1){
    analogWrite(motor2,20);
    delay(1000);
    aux2 = 0;
    }
  else if ( SystemState==1 && aux == 0 && aux2 == 0){
    analogWrite(motor2, 0);
    }
  

  if(SystemState==0 && PushState==true){            //////Rampa arriba-bajando
      SystemState=1;
      Ultrasonico();
      angle=0;
      while(angle<=180 /*&& distanciactual>4*/){
          servoMotor1.write(180-angle);
          servoMotor2.write(angle);
          Alerta();    
          angle+=15;
          Ultrasonico();
          Serial.println(angle);
          //delay(50);
      }
    }else if(SystemState==1 && PushState==true){    //////Rampa abajo-subiendo
      SystemState=0;
      angle=0;
      while(angle<=180 /*&& distanciactual>4*/){
          servoMotor1.write(angle);
          servoMotor2.write(180-angle);
          Alerta();    
          angle+=15;
          Ultrasonico();
          Serial.println(angle);
          //delay(50);
      }     
      /*while(angle-i>=0){
          servoMotor1.write(angle-i);
          servoMotor2.write(90-angle+i);
          Alerta();    
          i+=5;
      }*/
    }

      
delay(15);
}

void Ultrasonico(){


  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  distanciactual = t/59;             //escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(distanciactual);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  Serial.println(SystemState);
  delay(100);          //Hacemos una pausa de 100ms
  
  ////////////////
  //led();
  ////////////////
}


void Alerta(){
  digitalWrite(Led, HIGH);
  digitalWrite(Buzzer, HIGH);
  delay(150);
  digitalWrite(Led, LOW);
  digitalWrite(Buzzer, LOW);
  delay(150);
  }
