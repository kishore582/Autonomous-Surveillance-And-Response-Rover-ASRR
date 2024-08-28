#define BLYNK_TEMPLATE_ID "TMPL3GBnx7Mi7"
#define BLYNK_TEMPLATE_NAME "rover"
#define BLYNK_AUTH_TOKEN "brQLoKvNjS50VmpBYwBgIT6970w8QMad"

// put your setup code here, to run once:

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;

#define in1 5
#define in2 18
#define in3 19
#define in4 21
#define echoPin1 14
#define trigPin1 13
#define echoPin2 26
#define trigPin2 27
#define echoPin3 22
#define trigPin3 25
long duration1,duration2,duration3;
int distance1,distance2,distance3; 

#include <dht11.h>
#define DHT11PIN 23

dht11 DHT11;
float humidity,temp;
int ROBO_F_B,ROBO_L_R,ROBO_BASE_L,ROBO_BASE_R;
int GUN;
int gas,Mode;
int CAM_360,CAM_XY;
String S_data;
WiFiClient client;
char auth[] = "brQLoKvNjS50VmpBYwBgIT6970w8QMad";
char ssid[] = "rover";
char pass[] = "12345678";

BLYNK_WRITE(V3)
{
  ROBO_F_B = param.asInt(); // Get value as integer
  Serial.println("ROBO_F_B");
  Serial.println(ROBO_F_B);  
}

BLYNK_WRITE(V2)
{
  ROBO_L_R = param.asInt(); // Get value as integer
  Serial.println("ROBO_L_R");
  Serial.println(ROBO_L_R);  
}

BLYNK_WRITE(V4)
{
  ROBO_BASE_L = param.asInt(); // Get value as integer
Serial.print(ROBO_BASE_L);
Serial.print(" , ");
}

BLYNK_WRITE(V5)
{
  ROBO_BASE_R = param.asInt(); // Get value as integer
Serial.println(ROBO_BASE_R);
}

BLYNK_WRITE(V6)
{
  GUN = param.asInt(); // Get value as integer
  GUN=map(GUN,0,100,180,0);
  Serial.print("GUN=");
  Serial.println (GUN);
}
BLYNK_WRITE(V7)
{
  CAM_360 = param.asInt(); // Get value as integer
  CAM_360=map(CAM_360,0,100,180,0);
}

BLYNK_WRITE(V8)
{
  CAM_XY = param.asInt(); // Get value as integer
  CAM_XY=map(CAM_XY,0,100,180,0);
}

BLYNK_WRITE(V9)
{
  Mode= param.asInt(); // Get value as integer
  Serial.print("Mode =");
  Serial.println(Mode);
}

void setup(){

  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT); 
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT); 
  
  digitalWrite(in1,LOW);
   digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Blynk.begin(auth, ssid, pass);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  //----------servo 1-------------------
  myservo1.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.attach(4, 1000, 2000); // attaches the servo on pin 18 to the servo object
  //----------servo 2---------------
  myservo2.setPeriodHertz(50);    // standard 50 hz servo
  myservo2.attach(2, 1000, 2000); // attaches the servo on pin 18 to the servo object
  //----------servo 3----------------
  myservo3.setPeriodHertz(50);    // standard 50 hz servo 
  myservo3.attach(15, 1000, 2000); // attaches the servo on pin 18 to the servo object
  
  }

void loop() {

Blynk.run();

//======================Serial Cam==============
 if(Serial2.available()){
    Serial2.print(CAM_360);
    Serial2.print(",");
    Serial2.println(CAM_XY);
    }

//=====================front sonic==============

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  //Serial.print("Distance1: ");
  //Serial.print(distance1);
  //Serial.println(" cm");

//====================left sonic===================

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2= pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  //Serial.print("Distance2: ");
  //Serial.print(distance2);
  //Serial.println(" cm");

//=====================right sonic================


  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  //Serial.print("Distance3: ");
  //Serial.print(distance3);
  //Serial.println(" cm");


if(Mode==0){
    Serial.println("AUTOMATIC MODE ACTIVATED");
    //Blynk.logEvent("notify","MANUAL MODE ACIVATED");
  if(distance1>=20){
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
  }
  else if((distance1<=20)&&(distance2>=20))
   {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
  }
  else if((distance1<=20)&&(distance2<=20)&&(distance3>=20))
   {
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);
  }
  else if((distance1<=20)&&(distance2<=20)&&(distance3<=20)){
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
      Blynk.logEvent("warning"," Obstacle D etected Change To Manual Mode");
  }
 }
 
 if(Mode==1){
  
  Serial.println("MANUAL MODE ACTIVATED");
  //Blynk.logEvent("notify","MANUAL MODE ACIVATED");
   if(ROBO_F_B>=200)
  {
  Serial.println("FRONT");
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  }
  if(ROBO_F_B<=-200)
  {
  Serial.println("BACK");
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW); 
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  }
 
  if(ROBO_L_R>=200)
  {
  Serial.println("LEFT");
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  }
  if(ROBO_L_R<=-200)
  {
  Serial.println("RIGHT");
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  }
  if((ROBO_F_B==0)&&(ROBO_L_R==0)){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  }

}

 

    
gas=analogRead(32);
Serial.print("Gas =");

Serial.println(gas);
gas=map(gas,0,4095,100,0);

if(gas>=70){
  Blynk.logEvent("warning","Gas Level Is High");
  }

int chk = DHT11.read(DHT11PIN);
humidity=DHT11.humidity;
temp=DHT11.temperature;


Serial.print("Humidity (%): ");
Serial.println(humidity);
Serial.print("Temperature  (C): ");
Serial.println(temp);
Serial.print(ROBO_BASE_L);
Serial.print(" , ");
Serial.print(ROBO_BASE_R);

myservo2.write(GUN);
  
if(ROBO_BASE_R==1){
  myservo3.write(0); 
}
else if(ROBO_BASE_L==1){  
  myservo3.write(180);  
  }
  else{
   myservo3.write(90);
  }
Blynk.virtualWrite(V1,gas); 
Blynk.virtualWrite(V0,temp);
Blynk.virtualWrite(V6,humidity);
Blynk.syncAll();
} 
