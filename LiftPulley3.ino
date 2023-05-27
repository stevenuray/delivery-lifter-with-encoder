//Robogaia.com
// the sensor communicates using SPI, so include the library:
#include <SPI.h>

const unsigned short encoderToUse = 2;

const unsigned short chipSelectPin1=10;
const unsigned short chipSelectPin2=9;
const unsigned short chipSelectPin3=8;

const unsigned short enablePin = 2;
const unsigned short inputAPin = 3;
const unsigned short inputBPin = 4;

enum RotationDirection {
  CLOCKWISE,
  COUNTER_CLOCKWISE
};

enum LiftState {
  DOWN,
  DOWN_TO_WAIT_POSITION,
  DOWN_TO_LOAD_POSITION,
  PAUSE,
  EXIT_APARTMENT,
  RETURN_APARTMENT,
  UP,
  UP_TO_DELIVERY_POSITION
};

RotationDirection rotationDirection = COUNTER_CLOCKWISE;
LiftState liftState = EXIT_APARTMENT;

int rotationCount = 0;

unsigned int messageCount = 0;
const unsigned int messageFrequencyInMillis = 3000;

unsigned int loopCount = 0;
boolean printed = false;

unsigned int exitApartmentStateStartTimeMS = 0;
boolean exitApartmentStateStartTimeSet = false;

const unsigned int EXIT_APARTMENT_STATE_LOWER_DELAY_MS = 60000;
const unsigned long EXIT_APARTMENT_STATE_LOAD_DELAY_MS = 240000;

const unsigned int MAX_DOWN_CENTIMETERS = 625;
const unsigned int WAIT_POSITION_CENTIMETERS = 360;
const unsigned int DELIVERY_POSITION_CENTIMETERS = 100;

const unsigned short pauseButtonPin=5;
const unsigned short upButtonPin=6;
const unsigned short downButtonPin=7;
const unsigned short upToDeliveryButtonPin=A0;
const unsigned short downToWaitButtonPin=A1;

void setup() {
  Serial.begin(9600);
  
  setupPinModes();
  
  
  setupMotorDriver();
  LS7366_Init();

  delay(100);
}

void setupPinModes(){
  pinMode(chipSelectPin2, OUTPUT);
  digitalWrite(chipSelectPin2, HIGH);

  pinMode(pauseButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(upToDeliveryButtonPin, INPUT_PULLUP);
  pinMode(downToWaitButtonPin, INPUT_PULLUP);
}

void loop() {
  setStateFromButton();
  setStateFromInputKey();
  long encoderValue = getEncoderValue(encoderToUse);
  long encoderDownDistanceInCM = getEncoderDownDistanceInCentimeters(encoderValue);
  
  if(sendMessageIfTime(messageCount, "")) {
    Serial.print("Encoder X=");
    Serial.print(encoderValue);
    Serial.println();
    Serial.print("Encoder X DOWN Centimeters=");
    Serial.print(getEncoderDownDistanceInCentimeters(encoderValue));
    Serial.println();
    Serial.print("liftState=");
    Serial.print(liftState);
    Serial.println();
    Serial.print("millis()=");
    Serial.print(millis());
    Serial.println();
    Serial.print("Exit apartment state start time=");
    Serial.print(exitApartmentStateStartTimeMS);
    Serial.println();
    Serial.print("Time since exit apartment state start=");
    Serial.print(millis() - exitApartmentStateStartTimeMS);
    Serial.println();
    messageCount++;
  }

  switch (liftState) {
    case DOWN:
      downLoop();
      break;
    case DOWN_TO_WAIT_POSITION:
      downToWaitPositionLoop(encoderDownDistanceInCM);
      break;
    case DOWN_TO_LOAD_POSITION:
      downToLoadPositionLoop(encoderDownDistanceInCM);
      break;
    case EXIT_APARTMENT:
      exitApartmentLoop(encoderDownDistanceInCM);
      break;
    case RETURN_APARTMENT:
      returnApartmentLoop(encoderDownDistanceInCM);
      break;
    case UP:
      upLoop();
      break;
    case UP_TO_DELIVERY_POSITION:
      upToDeliveryPositionLoop(encoderDownDistanceInCM);
      break;    
    default:
      break;
  }

  delay(250);
}

void downLoop() {
  
}

void downToWaitPositionLoop(long downDistanceInCM) {
  if(downDistanceInCM >= WAIT_POSITION_CENTIMETERS) {
    stopRotation();
  }  
}

void downToLoadPositionLoop(long downDistanceInCM) {
  rotateClockwise();
  if(downDistanceInCM >= MAX_DOWN_CENTIMETERS) {
    stopRotation();
  }
}

void exitApartmentLoop(long downDistanceInCM) {
  if(exitApartmentStateStartTimeSet == false){
    exitApartmentStateStartTimeMS = millis();
    exitApartmentStateStartTimeSet = true;
    Serial.print("Exit apartment state start time is: ");
    Serial.print(exitApartmentStateStartTimeMS);
    Serial.println();
  }

  long inExitStateTimeMS = millis() - exitApartmentStateStartTimeMS;

  if(inExitStateTimeMS <= EXIT_APARTMENT_STATE_LOWER_DELAY_MS){
    Serial.println("Waiting for exit apartment start delay");
    stopRotation();
    return;
  }
  
  if(inExitStateTimeMS >= EXIT_APARTMENT_STATE_LOAD_DELAY_MS){    
    Serial.println("Going up to delivery in exit apartment state");
    startRotation();
    upToDeliveryPositionLoop(downDistanceInCM);
  } else {
    Serial.println("Going down to load in exit apartment state");
    startRotation();
    downToLoadPositionLoop(downDistanceInCM);
  }
}

void returnApartmentLoop(long downDistanceInCM){
  
}

void upLoop() {
  
}

void upToDeliveryPositionLoop(long downDistanceInCM) {
  rotateCounterClockwise();
  if(downDistanceInCM <= DELIVERY_POSITION_CENTIMETERS) {
    stopRotation();
  }
}
