/**
 * Author:    Dipesh Banerjee
 * Created:   14.01.2023
 * Used in SDV 4.0 Workshop
 **/

#define enA 5//Enable1 L298 Pin enA 
#define in1 6 //Motor1  L298 Pin in1 
#define in2 7 //Motor1  L298 Pin in1 
#define in3 8 //Motor2  L298 Pin in1 
#define in4 9 //Motor2  L298 Pin in1 
#define enB 10 //Enable2 L298 Pin enB 

#define L_S 11 //Left  sensor out port Connect to Digital pin 11
#define R_S 12 //Right sensor out port Connect to Digital pin 12



#define echo 2    //Echo pin
#define trigger 3 //Trigger pin



#define servo A5
int distance_L, distance_R; 

int Set=60;
int turn_speed=60;
int distance_F;
int distance1=0;
int distance2=0;
int speed=60;
int pwm=50;

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(R_S, INPUT); // declare if sensor as input  
  pinMode(L_S, INPUT); // declare ir sensor as input


  pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  



  pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
  pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
  pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
  pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
  pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
  pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

  
  analogWrite(enA, pwm); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
  analogWrite(enB, pwm); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed

  
  pinMode(servo, OUTPUT); 

  
distance_F = Ultrasonic_read();
delay(400);
}


void loop() {
  Serial.print("pwm=");
  Serial.print(pwm);Serial.print("\n");
  // put your main code here, to run repeatedly:
  distance_F = Ultrasonic_read();
  Serial.print("dist=");
  Serial.print(distance_F);Serial.print("\n");
  // Serial.print("D F=");Serial.println(distance_F);
  if(distance_F < Set){
    if(checktraffic()){
      Check_side();overtake();pwm=speed; return;
      }}
  else{pwm=speed;}
  // if Right Sensor and Left Sensor are at White color then it will call forword function
  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){
    analogWrite(enA, pwm);analogWrite(enB, pwm);forward();delay(10);// If no traffic ahead, go forward with initial speed
    // else{check_traffic();}
  }  

    // if Right Sensor is Black and Left Sensor is White then it will call turn Right function
  if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){
    pwm=turn_speed; 
    analogWrite(enA, pwm);analogWrite(enB, pwm);
    pwm=speed;
    turnRight();delay(40);
    }  

  //  if Right Sensor is White and Left Sensor is Black then it will call turn Left function
  if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){
    pwm=turn_speed;
    analogWrite(enA, pwm);analogWrite(enB, pwm);
    pwm=speed;
    turnLeft();delay(40);
    }
  
  if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){Stop();delay(10);}
  
}

void servoPulse (int pin, int angle){
//  Serial.println("Servo pulse");
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}

long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}

int checktraffic(){ 
  distance1 = Ultrasonic_read();
  delay(50);
  distance2 = Ultrasonic_read();
  if(distance2<=30){
    Stop();
    analogWrite(enA, speed);analogWrite(enB, speed);
    // distance1 = Ultrasonic_read();
    // delay(50);
    // distance2 = Ultrasonic_read();
    //if(check()){return 1;}
    return 1;
  }
  if(distance2<=distance1){
    pwm=(pwm-10);
    if(pwm<0){pwm=0;}
    analogWrite(enA, pwm);analogWrite(enB, pwm);
    // forward();
  }
  else if(distance2>distance1){
    pwm=(pwm+10);
    if(pwm>speed){pwm=speed;}
    analogWrite(enA, pwm);analogWrite(enB, pwm);
    // forward();
  }
  // else{forward();}
  return 0;
}


void Check_side(){
    Stop();
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
   // overtake();
}

void forward(){  //forword
Serial.print("forward\n");
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void turnRight(){ //turnRight
Serial.print("right\n");
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}

void turnLeft(){ //turnLeft
Serial.print("left\n");
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void Stop(){ //stop
Serial.print("stop\n");
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void overtake(){
   if(distance_L < distance_R){
   pwm=turn_speed; 
   analogWrite(enA, pwm);analogWrite(enB, pwm);
   Serial.print("overtake\n");
   turnLeft();
   delay(1200);
   forward();
   delay(1000);
   turnRight();
   delay(1200);
   forward();
   delay(1000);  
   turnRight();
   delay(1200);
   }
  else{
  pwm=turn_speed; 
  analogWrite(enA, pwm);analogWrite(enB, pwm);
  Serial.print("overtake\n");
  turnRight();
  delay(1200);
  forward();
  delay(1200);
  turnLeft();
  delay(1200);
  forward();
  delay(1200);  
  turnLeft();
  delay(1200);}
}
