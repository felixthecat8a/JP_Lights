#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
/******************** NeoPixel Setup ********************/
#define DATA_PIN A0
#define NUM_PIXELS 90
//Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);
#define PIXEL_BRIGHTNESS_LEVEL 50
/******************** EEPROM Setup ********************/
#define EEPROM_MODE_ADDR  0
#define EEPROM_COLOR_ADDR 1
/******************** Button Timing Variables ********************/
int buttonState = 0;
const int BUTTON_PIN = 2;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int currentMode;
int maxModes = 10;
/******************** EEPROM Functions ********************/
void loadSettings() {
  EEPROM.get(EEPROM_MODE_ADDR, currentMode);
  if (currentMode < 0 || currentMode > maxModes) currentMode = 0;
}
void saveSettings() {
  EEPROM.update(EEPROM_MODE_ADDR, currentMode);
}
/******************** Button Handling Functions ********************/
void updateButton() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        handleSingleClick();
      }
    }
  }
  lastButtonState = reading;
}
void handleSingleClick() {
  currentMode++;
  if (currentMode > maxModes) currentMode = 0;
  saveSettings();
}
/*****************************************************************************/
#define NUM_COLORS 4 // number of color options
int COLOR_RGB[NUM_COLORS][3] = {
   {255, 0, 0}, {255, 75, 0}, {10, 255, 0}, {10, 0, 255}
};
const char* COLOR_NAME[NUM_COLORS] = { "Red", "Amber", "Green", "Blue" };
/*****************************************************************************/
void setArrayColor(int c) {
  //Serial.println(COLOR_NAME[c]);
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
  }
  strip.show();
}
/*****************************************************************************/
void setArrayColorAndWhite(int c) {
  //Serial.println(COLOR_NAME[c]); Serial.println(" and White");
  for(int i = 0; i < strip.numPixels(); i++) {
    if (i % 2 == 0) {
      strip.setPixelColor(i, COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
    } else {
      strip.setPixelColor(i, 191, 191, 191);
    }
  }
  strip.show();
}
/*****************************************************************************/
void setOff() {
  //Serial.println("Lights Off");
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}
/*****************************************************************************/
void setup() {
  //Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  loadSettings();

  strip.begin();
  strip.setBrightness(PIXEL_BRIGHTNESS_LEVEL);
  strip.clear();
  
  setupChristmasTree();
}

void loop() {
  updateButton();

  switch (currentMode) {
    case 0: setArrayColor(0); break;
    case 1: setArrayColor(1); break;
    case 2: setArrayColor(2); break;
    case 3: setArrayColor(3); break;
    case 4: setArrayColorAndWhite(0); break;
    case 5: setArrayColorAndWhite(1); break;
    case 6: setArrayColorAndWhite(2); break;
    case 7: setArrayColorAndWhite(3); break;
    case 8: updateMaroonChase(); break;
    case 9: updateCandyCane(); break;
    case 10: updateChristmasTree(); break;
    default: setOff(); break;
  }
}
