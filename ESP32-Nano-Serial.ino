#include <Servo.h>
Servo myservo; 
Servo myservo2; 
String data1;
int value1,value2;
void setup() {
Serial.begin(9600);
myservo.attach(9);  // attaches the servo on pin 9 to the servo object}
myservo2.attach(10);  // attaches the servo on pin 9 to the servo object}
}
void loop() {
  if(Serial.available()){
    data1= Serial.readStringUntil('\n');;
    }
    Serial.println(data1);

     int delimiterIndex = data1.indexOf(',');
    if (delimiterIndex != -1) {
      // Extract the first value
      String value1_str = data1.substring(0, delimiterIndex);
       value1 = value1_str.toInt();

      // Extract the second value
      String value2_str = data1.substring(delimiterIndex + 1);
       value2 = value2_str.toInt();
      Serial.print("data=");
      Serial.println(value1);
      Serial.print("data2=");
      Serial.println(value2);
}
myservo.write(value1);
myservo2.write(value2);
}
