#include <SoftwareSerial.h>   
#include <Servo.h>            

SoftwareSerial Serial1(2, 3); 

int dist;                    
int strength;                
int check;                   
int i;
int uart[9];                 
const int HEADER = 0x59;      

const int RED_LED = 10;       
const int YELLOW_LED = 5;     
const int GREEN_LED = 6;      
const int SERVO_PIN = 9;      
const int BUZZER_PIN = 8;     // <--- Buzzer connected to Pin 8

Servo myservo;                

void setup() {
  Serial1.begin(115200);      
  pinMode(RED_LED, OUTPUT);   
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT); 
  pinMode(BUZZER_PIN, OUTPUT); // <--- set buzzer pin as output
  myservo.attach(SERVO_PIN);  
  myservo.write(0);           
}

void setLEDs(bool red, bool yellow, bool green) {
  digitalWrite(RED_LED, red);
  digitalWrite(YELLOW_LED, yellow);
  digitalWrite(GREEN_LED, green);
}

void activateBuzzer(bool state) {
  digitalWrite(BUZZER_PIN, state ? HIGH : LOW);
}

void loop() {
  if (Serial1.available()) {                
    if (Serial1.read() == HEADER) {         
      uart[0] = HEADER;
      if (Serial1.read() == HEADER) {       
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) {           
          uart[i] = Serial1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) {    
          dist = uart[2] + uart[3] * 256;   

          if (dist > 30 && dist < 300) {    // Danger zone
            setLEDs(true, false, false);
            myservo.write(90);              
            activateBuzzer(true);           // Turn on buzzer
          } 
          else if (dist >= 300 && dist < 600) { // Warning zone
            setLEDs(false, true, false);
            myservo.write(0);               
            activateBuzzer(false);          // Turn off buzzer
          } 
          else if (dist >= 600) {            // Safe zone
            setLEDs(false, false, true);
            myservo.write(0);               
            activateBuzzer(false);          // Turn off buzzer
          }
          delay(50); // Small delay to stabilize output
        }
      }
    }
  }
}
