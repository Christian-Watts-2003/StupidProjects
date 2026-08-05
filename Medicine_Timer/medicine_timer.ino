#include "TFT_eSPI.h"
#include "driver.h"

const int BUTTON_KEY0 = 2;   // KEY0 - GPIO2
const int BUTTON_KEY1 = 3;   // KEY1 - GPIO3

bool lastKey0State = HIGH;
bool lastKey1State = HIGH;

long count_48 = 0;
long count_168 = 0;

unsigned long lastHourTick = 0;
const unsigned long HOUR_MS = 3600000UL;

#ifdef EPAPER_ENABLE
EPaper epaper;
#endif

void drawCounterScreen(long count_48, long count_168) {
#ifdef EPAPER_ENABLE
  epaper.fillScreen(TFT_WHITE);
  epaper.setTextSize(3);
  epaper.drawString("Medicine", 30, 10);
  epaper.drawString("Timer", 40, 35);
  epaper.setTextSize(2);
  epaper.drawString("Hours Until:", 10, 60);
  epaper.drawString("48Hr: ", 10, 90);
  epaper.setTextSize(3);
  if (count_48 < 0) {
    epaper.setTextColor(TFT_WHITE, TFT_BLACK);
    epaper.drawString(String(count_48), 10, 110);
  } else {
    epaper.setTextColor(TFT_BLACK, TFT_WHITE);
    epaper.drawString(String(count_48), 10, 110);
  }
    epaper.setTextSize(2);
    epaper.setTextColor(TFT_BLACK, TFT_WHITE);
    epaper.drawString("168Hr: ", 10, 140);
    epaper.setTextSize(3);
  if (count_168 < 0) {
    epaper.setTextColor(TFT_WHITE, TFT_BLACK);
    epaper.drawString(String(count_168), 10, 160);
  } else {
    epaper.setTextColor(TFT_BLACK, TFT_WHITE);
    epaper.drawString(String(count_168), 10, 160);
  }
  epaper.update();
#endif
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_KEY0, INPUT_PULLUP);
  pinMode(BUTTON_KEY1, INPUT_PULLUP);

  lastKey0State = digitalRead(BUTTON_KEY0);
  lastKey1State = digitalRead(BUTTON_KEY1);

  lastHourTick = millis();

#ifdef EPAPER_ENABLE
  epaper.begin();
#endif

  drawCounterScreen(count_48, count_168);
}

void loop() {
  bool key0State = digitalRead(BUTTON_KEY0);
  bool key1State = digitalRead(BUTTON_KEY1);

  if (key0State != lastKey0State) {
    if (key0State == LOW) {
      count_48 = 48;
      drawCounterScreen(count_48, count_168);
    }
    lastKey0State = key0State;
    delay(50);
  }

  if (key1State != lastKey1State) {
    if (key1State == LOW) {
      count_168 = 168;
      drawCounterScreen(count_48, count_168);
    }
    lastKey1State = key1State;
    delay(50);
  }

  while (millis() - lastHourTick >= HOUR_MS) {
    count_48--;
    count_168--;
    lastHourTick += HOUR_MS;
    drawCounterScreen(count_48, count_168);
  }

  delay(10);
}
