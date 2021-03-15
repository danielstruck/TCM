#include "inc/Main.hpp"
#include "inc/Boundry.hpp"
#include "inc/HWIO.hpp"
#include "inc/Initializer.hpp"
#include "inc/Logger.hpp"
#include "inc/MessageSender.hpp"
#include "inc/Profile.hpp"
#include "inc/TemperatureDetection.hpp"


void setup() {
  // TODO remove delays after testing 
  setLEDs(0, 1, 0, 0, 0);
//  Serial.begin(115200);
  delay(100);
  setLEDs(0, 1, 1, 0, 0);
  setupFona();
  delay(100);
  setLEDs(0, 1, 1, 1, 0);
  setupPins();
  delay(100);
  setLEDs(0, 1, 1, 1, 1);
  setupLogger();
  delay(100);
  setLEDs(1, 0, 0, 0, 0);

  setupInitialProfile();
  
  setLEDs(0, 0, 0, 0, 0);
}

bool detectRisingEdge(bool lastBtnState, bool currentBtnState) {
  return lastBtnState == LOW && currentBtnState == HIGH;
}

bool isProfileBtnRising() {
  static bool lastBtnState = false;

  bool currentBtnState = profileBtnPressed();

  bool risingEdgeDetected = detectRisingEdge(lastBtnState, currentBtnState);
  
  lastBtnState = currentBtnState;

  return risingEdgeDetected;
}

bool isResetBtnRising() {
  static bool lastBtnState = false;

  bool currentBtnState = resetBtnPressed();

  bool risingEdgeDetected = detectRisingEdge(lastBtnState, currentBtnState);
  
  lastBtnState = currentBtnState;

  return risingEdgeDetected;
}

void loop() {
  // check termal range
  if (!isTemperatureInsideBoundries()) {
    // TODO message sender flag
  }

  logData(temperatureChamber);
  
  // read button to change current profile
  if (isProfileBtnRising()) {
    incrementProfile();
  }

  if (!isPowerOK()) {
    chooseMessage(badPower);
    sendText();
  }

  if (isResetBtnRising()) {
    // TODO reset error flags
  }
}
