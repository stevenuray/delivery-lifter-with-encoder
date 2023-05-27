const unsigned int INNER_CIRCLE_DIAMETER_MM = 30;
const unsigned int OUTER_CIRCLE_DIAMETER_MM = 45;
const unsigned int INNER_CIRCLE_CIRCUMFERENCE_MM = getCircumferenceFromDiameter(INNER_CIRCLE_DIAMETER_MM);
const unsigned int OUTER_CIRCLE_CIRCUMFERENCE_MM = getCircumferenceFromDiameter(OUTER_CIRCLE_DIAMETER_MM);
const unsigned int MAX_ROPE_MM = 4;
const unsigned int ESTIMATED_ENCODER_CM_PER_ENCODER_UNITS = 1262;
const unsigned int countableCyclesPerRevolutionMotor = 12;
const unsigned int countableCyclesPerRevolutionOutput = 1669;
const unsigned int countableEventsPerRevolutionMotor = 48;
const unsigned int countableEventsPerRevolutionOutput = 6678;

const float valueOfPiEstimate = 3.14159;

const int MAX_MOTOR_SPEED_UP = 400;
const int MAX_MOTOR_SPEED_DOWN = -400;

long getCircumferenceFromDiameter(int diameter) {
  return round(2*valueOfPiEstimate*diameter);
}

long getEncoderDownDistanceInCentimeters(long encoderValue) {
  // Hacky method that worked fast but is probably not accurate at long distances or extensible
  return round(encoderValue / ESTIMATED_ENCODER_CM_PER_ENCODER_UNITS);
  
  /* Well intentioned attempt to do math for accurate and legit estimates that somehow did not work
  float estimatedRotations = encoderValue / countableEventsPerRevolutionOutput;
  float estimatedRotationsMM = estimatedRotations * OUTER_CIRCLE_CIRCUMFERENCE_MM;
  float estimatedRotationsCM = estimatedRotationsMM / 10;
  Serial.print("Estimated Rotations: ");
  Serial.print(estimatedRotations, 6);
  Serial.print(" Estimated RotationsMM: ");
  Serial.print(estimatedRotationsMM);
  Serial.print(" Estimated Rotations CM: ");
  Serial.print(estimatedRotationsCM);
  Serial.println();
  return estimatedRotationsCM;
  */
}

//*****************************************************  
long getEncoderValue(int encoder)
//*****************************************************
{
    unsigned int count1Value, count2Value, count3Value, count4Value;
    long result;
    
    selectEncoder(encoder);
    
    SPI.transfer(0x60); // Request count
    count1Value = SPI.transfer(0x00); // Read highest order byte
    count2Value = SPI.transfer(0x00);
    count3Value = SPI.transfer(0x00);
    count4Value = SPI.transfer(0x00); // Read lowest order byte
    
    deselectEncoder(encoder);
   
    result= ((long)count1Value<<24) + ((long)count2Value<<16) + ((long)count3Value<<8) + (long)count4Value;
    
    return result;
}//end func

//*************************************************
void selectEncoder(int encoder)
//*************************************************
{
  switch(encoder)
  {
     case 1:
       digitalWrite(chipSelectPin1,LOW);
       break;
     case 2:
       digitalWrite(chipSelectPin2,LOW);
       break;
     case 3:
       digitalWrite(chipSelectPin3,LOW);
       break;
  }//end switch
  
}//end func

//*************************************************
void deselectEncoder(int encoder)
//*************************************************
{
  switch(encoder)
  {
     case 1:
        digitalWrite(chipSelectPin1,HIGH);
        break;
     case 2:
       digitalWrite(chipSelectPin2,HIGH);
       break;
     case 3:
       digitalWrite(chipSelectPin3,HIGH);
       break;    
  }//end switch
  
}//end func



// LS7366 Initialization and configuration
//*************************************************
void LS7366_Init(void)
//*************************************************
{
   
    
    // SPI initialization
    SPI.begin();
    //SPI.setClockDivider(SPI_CLOCK_DIV16);      // SPI at 1Mhz (on 16Mhz clock)
    delay(10);
   
   digitalWrite(chipSelectPin1,LOW);
   SPI.transfer(0x88); 
   SPI.transfer(0x03);
   digitalWrite(chipSelectPin1,HIGH); 
   
   
   digitalWrite(chipSelectPin2,LOW);
   SPI.transfer(0x88); 
   SPI.transfer(0x03);
   digitalWrite(chipSelectPin2,HIGH); 
   
   
   digitalWrite(chipSelectPin3,LOW);
   SPI.transfer(0x88); 
   SPI.transfer(0x03);
   digitalWrite(chipSelectPin3,HIGH);
}

void rotateClockwise() {
  digitalWrite(inputAPin, HIGH);
  digitalWrite(inputBPin, LOW);
  rotationDirection = CLOCKWISE;
}

void rotateCounterClockwise() {
  digitalWrite(inputAPin, LOW);
  digitalWrite(inputBPin, HIGH);
  rotationDirection = COUNTER_CLOCKWISE;
}

void setupMotorDriver() {
  pinMode(enablePin, OUTPUT);
  pinMode(inputAPin, OUTPUT);
  pinMode(inputBPin, OUTPUT);
}

void startRotation() {
  digitalWrite(enablePin, HIGH);
}

void stopRotation(){
  digitalWrite(enablePin, LOW);
  digitalWrite(inputAPin, LOW);
  digitalWrite(inputBPin, LOW);
}
