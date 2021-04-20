#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

FPS_GT511C3 fps(10, 11);
//FPS_GT511C3 fps(D6, D5);  // For Nodemcu RX-TX NMCU D5 <-> FPS P3
uint8_t irRead = A0;
int irADCVal = 0;
uint8_t usrID = 201;
uint8_t i = 0;
uint8_t flagRead = 0, readCount = 0;

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW); // Lock Control Signal - Close / Pull Down
  //wdt_enable(WDTO_4S);
  //Timer_Compare_Setup();
  fps.Open();
  fps.SetLED(false);
  fps.UseSerialDebug = true; // Enables debugging for HEX cmd
}

int validetUsr()
{
  uint8_t id = 201;
  //send serial command to initialize fps
  fps.SetLED(true);
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    delay(20);
    id = fps.Identify1_N();
    if (id > 0 && id < 200) //<- change id value depending model you are using
    { //if the fingerprint matches, provide the matching template ID
      Serial.print("Verified-ID:");
      Serial.println(id);
      readCount = 0;
      flagRead = 0;
      digitalWrite(12, HIGH);
      delay(500);
      digitalWrite(12, LOW);
      delay(10);
      fps.SetLED(false);
      return id;
    }
  }
}

void loop() {
  irADCVal = analogRead(A0);
  delay(10);
  /***
    Serial.print("flagRead:");
    Serial.print(flagRead);
    Serial.print(" readCount:");
    Serial.print(readCount);
    Serial.print("  ADC:");
    Serial.print(irADCVal);
    Serial.print("  User ID:");
    Serial.println(usrID);
  */
  if (irADCVal < 200) {
    //  initialize all parameter which are going to be use during scanning
    Serial.println("******  Button Press Detectrd   ******");
    flagRead = 1;
    readCount = 0;
  }
  if (flagRead == 1) {
    Serial.print(" User ID:");
    Serial.println(usrID);
    readCount++;
    if (readCount < 10) {
      usrID = validetUsr();
      Serial.print(" Matched ID:");
      Serial.println(usrID);
    }
  }
  if (readCount > 10) {
    readCount = 0;
    flagRead = 0;
    usrID = 201;
    fps.SetLED(false);
  }

  delay(980);
}
