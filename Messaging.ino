boolean sendMessageIfTime(int messageCount, String message) {
  if (floor((millis() / messageFrequencyInMillis)) > messageCount) {
    Serial.println(message);
    return true;
  } else {
    return false;
  }
}

String buildMessageString(int rotationCount) {
  char data[100];
  sprintf(data, "Rotation count is: %d", rotationCount);
  return data;
}