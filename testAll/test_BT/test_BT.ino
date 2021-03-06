//blue tooth(HC05) module test
//Master:Arduino Mega 2560
//hardware seria
//RX1 19 TX1 18
//RX2 17 TX2 16
//RX3 15 TX3 14
//receive punch counter
//slave BT code is test_BT_slave
#include <punchBT_master.h>
#define AT_Mode 0
#define Master_Mode 1
#define showlog 1
char val_right;
char val_left;
//data format 
//left arm lxxxx. ex:r1000.
//right arm rxxxx. ex:r0888.
//!!First,you need use AT_Mode to pair BT echo other(master and slave),please refer to BT_pair document!!
//test step:
//step1:master waits for data(punch count) from slave
//step2:if slave changes mode(puase -> punch),start transmits data(punch count)
//step3:master receives data(1-5) from slave
//step4:master receives  data  is 5,transmits reset(lRSET. og rRSET.) to slave
//step5:slave punch count will be 0 and changes mode(punch -> pause), so loop step1
punchBT_master punch_BT_R,punch_BT_L;
bool already_resetL = false;
bool already_resetR = false;

void setup() {
  Serial.begin(9600);  
#if AT_Mode
  Serial.println("enter AT commands");
  punch_BT_R.punchBT_master_initial_set(serial2, AT_mode,right);
  punch_BT_L.punchBT_master_initial_set(serial1, AT_mode,left);
#else if   Master_Mode
 Serial.println("receive_Mode");
 punch_BT_R.punchBT_master_initial_set(serial2, Master_mode,right);
 punch_BT_L.punchBT_master_initial_set(serial1, Master_mode,left);
#endif
}

void loop() {
 #if AT_Mode  
    AT_Mode_function();
    punch_BT_R.AT_mode_function();
    punch_BT_L.AT_mode_function();
 #else if receive_Mode

  int reL = punch_BT_L.Master_mode_receive();
  if(reL >0 && reL <10000){
     Serial.print("receive val_left: ");
     Serial.println(reL); 
     if(reL == 5) {
         if(!already_resetL) {   
            Serial.println("master is reset send reset signal to slaveL");
            punch_BT_L.Master_mode_transmit_reset();
         }
         already_resetL = true;
     } else if(reL == 1) {
         already_resetL = false;
     }
  } else if (reL == 10000){
      Serial.println("slaveL is pause!!");
  }
  
 int reR = punch_BT_R.Master_mode_receive();
 if(reR >0 && reR <10000){
     Serial.print("receive val_right: ");
     Serial.println(reR); 
     if(reR == 5) {
       if(!already_resetR) {   
            Serial.println("master is reset send reset signal to slaveR");
            punch_BT_R.Master_mode_transmit_reset();
       }
        already_resetR = true;
     } else if(reR == 1) {
        already_resetR = false;
     }
  } else if (reR == 10000){
      Serial.println("slaveR is pause!!");
  }
 #endif
}


