const char DOWN_HOTKEY = 'x';
const char DOWN_TO_WAIT_POSITION_HOTKEY = 'w';
const char DOWN_TO_LOAD_POSITION_HOTKEY = 'l';
const char EXIT_APARTMENT_HOTKEY = 'e';
const char RETURN_APARTMENT_HOTKEY = 'r';
const char PAUSE_HOTKEY = 'p';
const char UP_HOTKEY = 's';
const char UP_TO_DELIVERY_POSITION_HOTKEY = 'u';

void setStateFromInputKey() {
  if (Serial.available() > 0){
    byte incomingByte = Serial.read();
    char incomingChar = incomingByte;
    Serial.print("Key pressed: ");
    Serial.print(incomingChar);
    Serial.println();

    if (incomingChar == DOWN_HOTKEY) {    
      liftState = DOWN;
      Serial.println("Down hotkey pressed");
      startRotation();
      rotateClockwise();
    }
    
    if (incomingChar == DOWN_TO_WAIT_POSITION_HOTKEY) {
      Serial.println("Down to wait position hotkey pressed");
      liftState = DOWN_TO_WAIT_POSITION;
      startRotation();
      rotateClockwise();
    }
    
    if (incomingChar == DOWN_TO_LOAD_POSITION_HOTKEY) {
      Serial.println("Down to load position hotkey pressed");
      liftState = DOWN_TO_LOAD_POSITION;
      startRotation();
      rotateClockwise();
    }

    if (incomingChar == EXIT_APARTMENT_HOTKEY) {
      Serial.println("Exit apartment hotkey pressed");
      liftState = EXIT_APARTMENT;
      rotateClockwise();
    }

    if (incomingChar == PAUSE_HOTKEY) {    
      liftState = PAUSE;
      Serial.println("Pause hotkey pressed");
      stopRotation();
    }

    if (incomingChar == UP_HOTKEY) {    
      liftState = UP;
      Serial.println("Up hotkey pressed");
      startRotation();
      rotateCounterClockwise();
    }
    
    if (incomingChar == UP_TO_DELIVERY_POSITION_HOTKEY) {
      liftState = UP_TO_DELIVERY_POSITION;
      Serial.println("Up to delivery position hotkey pressed");
      startRotation();
      rotateCounterClockwise();
    }    
  }
}
