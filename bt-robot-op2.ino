#include <AFMotor.h>
#include <Servo.h> 
#include <NewPing.h>
#define FR_Motor 2
#define FL_Motor 1
#define BR_Motor 3
#define BL_Motor 0
uint8_t current_state = FORWARD;
char INBYTE;
int mSpeed = 255;
int *last_motor_state;
int last_total_motors;

AF_DCMotor motors[4] = { { 1, MOTOR12_64KHZ }, { 2, MOTOR12_64KHZ }, { 3, MOTOR34_64KHZ }, { 4, MOTOR34_64KHZ } };

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (!Serial.available()) return;
  INBYTE = Serial.read();
  handleRequests(INBYTE);
}


void handleRequests(char INBYTE) {
   int all_motors[4] = {FR_Motor, FL_Motor, BR_Motor, BL_Motor};      
   int right_motors[2] = {FR_Motor, BR_Motor};
   int left_motors[2] = {FL_Motor, BL_Motor};
    Serial.println(current_state);

   switch (INBYTE) {
    case '8':
      motorRun(all_motors, 4, FORWARD);
      break;
    case '2':
      motorRun(all_motors, 4, BACKWARD);
      break;
    case '.':
      motorRun(all_motors, 4, RELEASE);
      resetCurrentState();
      break;
    case '6':
      motorRun(left_motors, 2, current_state);
      break;
    case '4':
      motorRun(right_motors, 2, current_state);
      break;
   case '+':
      speedUp();
      break;
   case '-':
      speedDown();
      break;
   case 'r':
      speedReset();
      break;

    default:
      break;
  }
  
  }

void motorRun(int *motor, int totalMotors, uint8_t motor_direction) {
    stop_motors();
    for ( int i = 0; i < totalMotors; i++ ) {
      motors[motor[i]].setSpeed(mSpeed);
      motors[motor[i]].run(motor_direction);
      }
  last_motor_state = motor;
  last_total_motors = totalMotors;
  current_state = motor_direction;
}

void resetCurrentState() {
  current_state = FORWARD;
  }

void stop_motors() {
    for ( int i = 0; i < 4; i++ )
      motors[i].run(RELEASE);      
  }

void speedUp() {
     if (mSpeed >= 255) return;
      mSpeed += 15;
      motorRun(last_motor_state, last_total_motors, current_state);
  }

void speedDown() {
    if (mSpeed <= 0) return;
    mSpeed -= 15;
    motorRun(last_motor_state, last_total_motors, current_state);

}

  void speedReset() {
    mSpeed = 255;
    motorRun(last_motor_state, last_total_motors, current_state);
  }

