#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <MsTimer2.h>

Servo myservo;                                           // create servo object to control the servo
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // create LCD object to control the screen

        // Permanent constant:
#define SEV 10        // the time to be delayed by the servo
#define Relax 0       // the relaxed position
#define Shrink 180    // the shrinked position 

        // Changeable variables:
unsigned char Hand_Angle = 0;    
unsigned char Num = 0;

// Method to Relax(open) the hand
void Hand_Relax() {
  if (Hand_Angle != Relax) {
    while (Hand_Angle != Relax) {
      Hand_Angle--;
      myservo.write(Hand_Angle);
      delay(SEV);
    }
  }
}

// Method to Shrink(Close) the hand
void Hand_Shrink() {
  if (Hand_Angle != Shrink) {
    while (Hand_Angle != Shrink) {
      Hand_Angle++;
      myservo.write(Hand_Angle);
      delay(SEV);
    }
  }
}

//Method to Change the hand's position (Will be runned every 1000ms)
void action() {
  if (Num > 0)
    Hand_Shrink();
  else
    Hand_Relax();

  Num = 0;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myservo.attach(2);
  MsTimer2::set(1000, action);
  MsTimer2::start();
}

void loop() {
  int sensorValue = analogRead(A0);

    lcd.setCursor(4, 0); 
    lcd.print("Status: ");

    // The muscle is closing the fist:
  if (sensorValue > 50) {
    Num++;
    Serial.println("Num: " + String(Num));
    Serial.println("Sensor: " + String(sensorValue));
    delay(1000);
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("Perfect muscle");
  }
  // The muscle is moving, but very weak 
  else if (sensorValue > 30) {
    Num =0 ;
    Serial.println("Sensor: " + String(sensorValue));
    delay(1000);
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Weak muscle");
  } 
  // No, or negligible muscle movement
  else {
    Num =0 ;
    Serial.println("Sensor: " + String(sensorValue));
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Inactive muscle");
  }
}
