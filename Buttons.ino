void setStateFromButton() {
    if (digitalRead(downButtonPin) == LOW) {    
      liftState = DOWN;
      Serial.println("Down button pressed");
      startRotation();
      rotateClockwise();
    }

    if (digitalRead(pauseButtonPin) == LOW) {    
      liftState = PAUSE;
      Serial.println("Pause button pressed");
      stopRotation();
    }

    if (digitalRead(upButtonPin) == LOW) {    
      liftState = UP;
      Serial.println("Up button pressed");
      startRotation();
      rotateCounterClockwise();
    }
    
    if (digitalRead(upToDeliveryButtonPin) == LOW) {    
      liftState = UP_TO_DELIVERY_POSITION;
      Serial.println("Up to delivery position button pressed");
      startRotation();
      rotateCounterClockwise();
    }

    if (digitalRead(downToWaitButtonPin) == LOW) {    
      liftState = DOWN_TO_WAIT_POSITION;
      Serial.println("Down to wait position button pressed");
      startRotation();
      rotateClockwise();
    }
}
