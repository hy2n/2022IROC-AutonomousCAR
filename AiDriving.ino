// IROC AIDRIVING 2022 DONGHYUN.
// 2022-08-08 Created
#include <Servo.h> //servo
#include <stdio.h> //c
#define DriveCN 0x000001001 //aidriver.ino

//pixy
#include <SPI.h>
#include <Pixy2.h>
#include <Pixy2I2C.h>
int E1 = 3;  //Motor 1 power port
int M1 = 12; //Motor 1 Drection port
int E2 = 11; //Motor 2 power port
int M2 = 13; //Motor 2 Drection port
//servo
int servoMain = 90;
Pixy2 pixy;
Servo myservo;

// Airacing Pixy Engine Code
// 2022-08-08 Created
#define pixy_ccc_MaxBLock_TargetBl 2
#define pixy_ccc_objectCL_Terminal 120
// Motor Value Setup
char Motor1 = 0;
char Motor2 = 0;
int Motor1Power = 0;
int Motor2Power = 0;
int PixyBlock;

//Values
int TargetBlock_X = 0; //Targetblock id
int TargetBlock_Y = 0; //Targetblock id
int TargetBlock_SIZE = 0; //Targetblock id
int TargetBlock_HEIGHT = 0; //Targetblock id


int svo;

//PixyAiEngine
int PixyBlockisMid = 0;
int PixyMidCheck_default_range_mid = 10; //Default for size chai
int PixyMidCheck_default_range_max_x = 30; //Default for x-range

//math
int Pixy_cm = 0;

int Six = 0;


//PixyAiScan
int Pixyblock_define_x = 0;
int Pixyblock_define_y = 0;
int PixyBlock_define_size = 0;
int Pixyblock_define_height = 0;

int i; //int for scan.

int block1_pixy = 0;
int block2_pixy = 1;

int ScanPixy() { // return 0,1 (bigger block ID) / 2 : no block / 3 : straight
  
  //reset values
  TargetBlock_X = 0;
  TargetBlock_Y = 0;
  TargetBlock_SIZE = 0;
  TargetBlock_HEIGHT = 0;
  
  PixyBlock == 2; //reset pixyblocks for founding 0 blocks
  pixy.ccc.getBlocks(); //Scan Blocks
  if (pixy.ccc.numBlocks) {
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      //Serial.print("  block ");
      //Serial.print(i);
      //Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
    //Scan Finished
    //Print Status for debug
    //Serial.println("**");
    //Serial.print("[ScanPixy ENGINE 2] ");
    //Serial.print(pixy.ccc.numBlocks);
    //Serial.print("Blocks Deceted.");
    
    //하나의 블럭 y값이 높고 다른 블럭의 y값이 낮으면 더 가까운 블럭이므로 우선순위를 둔다
    
   
    if (pixy.ccc.numBlocks > 1) { //Chose 1 block when 2 blocks are found
      PixyBlock = PixyBiggerObject(0,1); //Call scan and set the bigger block to value "PixyBlock"
      //Serial.print("BIGGER BLOCK : "); //Debug
      //erial.println(PixyBlock); //Debug
    }
    else { //if 1 blocks are found, set the block to value "PixyBlock"
      PixyBlock = 0; //One Objects ID is "0"
    }
    
  }
  if (PixyBlock == 2) {
    PixyBlock = 2; //if no blocks are found return 2
   }
  return PixyBlock;
}

int PixyBiggerObject(int object1,int object2) { 
  if (Line_Y[object1] > Line_Y[object2]) { 
    return object1;
  }
  else if (Line_Y[object1] < Line_Y[object2]) {  
      return object2;
  }
}


//end of pixy

// Airacing Servo Control Code
// 2022-08-08 Created

int servoMin = 40;
int servoMax = 150;
int servoCur = 90;

//Servosetup() : 서보모터 초기화 코드
void Servosetup() {
  myservo.attach(9); //Servo on pin9
  myservo.write(servoMain); //Setup Main Servo Angle
}


//ServoSetbp(값) : 서보모터 각도 설정 [범위초과시 자동 반환]
int ServoSetbp(int servoADR) {
  if (servoADR < servoMin) {
    myservo.write(servoMin);
  }
  else if (servoADR > servoMax) {
    myservo.write(servoMax);
  }
  else {
    myservo.write(servoADR);
  }
  
}




 //end of servo

 // Airacing Motor Driver Code
// 2022-08-07 Created

//함수 Motor(A출력,B출력,A출력량,B출력량);
//ex> Motor(1,1,255,255);

//Motor Port Setup



int Motor(int Motor1)
{
  //Motor
  //1 = Forward, 0 = Stop

  //For this code
  //Fw = Forward
  //Bw = Backward

  //Motor(*)Power = 0 ~ 255 (controls motor's speed)
  analogWrite(5, 0); 

  analogWrite(6, Motor1);  

}


//CopyRight 2022 DONGHYUN.CC
//end of motor

// IROC AiDriving AI ENGINE
// 2022-08-08 Created
//
// This File is part of 'IROC AI Driving'

int Drive() {
  PixyBlock = ScanPixy(); //Get targeted block

  //Get block status
  TargetBlock_X = pixy.ccc.blocks[PixyBlock].m_x ;
  TargetBlock_Y = pixy.ccc.blocks[PixyBlock].m_y;
  TargetBlock_SIZE = pixy.ccc.blocks[PixyBlock].m_width ;
  TargetBlock_HEIGHT = pixy.ccc.blocks[PixyBlock].m_height;
  if (PixyBlock == 3) {
    Motor(255);
  }
  else if (PixyBlock == 2) {
    Motor(255);
  }
  else {
    //주행 실행
    //motor speed
    if (TargetBlock_Y > 100) {
      Motor(190);
    }
    else if ((TargetBlock_SIZE > 70)&&(TargetBlock_SIZE < 141)) {
      Motor(210);
    }
    else if (TargetBlock_SIZE > 140) {
      Motor(210);
    }
    else {
      Motor(170);
    }
    
   
    if (TargetBlock_SIZE < 51) { //블록이 큰가
     //Check block is L or R
      if (TargetBlock_X > 150) { //R
        svo = 90 + (TargetBlock_Y / 3.1) ; 
        ServoSetbp(svo);
        Serial.println(svo);
        Serial.print(" 0 ");
        Serial.println(svo);
      }
      else if (TargetBlock_X < 150) { //L
        svo = 90 - (TargetBlock_Y / 3.1) ; 
        ServoSetbp(svo);
        Serial.print(" 1 ");
        Serial.println(svo);
      }
      
    }
   //nl
    else if (TargetBlock_Y > 20) { // y > 20?
      if (TargetBlock_X > 150) { //R
        svo = 90 + (TargetBlock_Y / 2.2) ; 
        ServoSetbp(svo);
        Serial.println(svo);
        Serial.print(" 0 ");
        Serial.println(svo);
      }
      else if (TargetBlock_X < 150) { //L
        svo = 90 - (TargetBlock_Y / 2.2) ; 
        ServoSetbp(svo);
        Serial.print(" 1 ");
        Serial.println(svo);
      }
    }
  }
}
  
int pos;

void setup() {
  Serial.begin(115200); //Serial 115200
  
  myservo.attach(9); //Servo on pin9
  myservo.write(servoMain); //Setup Main Servo Angle
   
  pinMode(M1, OUTPUT);  
  pinMode(M2, OUTPUT);
  
  pixy.init(); //Setup Pixy
  
pinMode(5, OUTPUT); 

pinMode(6, OUTPUT);
  
}

void loop() {
  Drive();
}
