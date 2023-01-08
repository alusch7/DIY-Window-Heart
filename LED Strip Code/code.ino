#include <Adafruit_NeoPixel.h>
#include "pitches.h"
#define LED_PIN 6
#define BUZZER 12
#define LED_COUNT 96
#define COLBUT 0 // Color Button Pin
#define MODEBUT 3 // Mode Button Pin
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

////////////////////////// TIME VARIABLES /////////////////////////////
unsigned long lastPressTimeColor = 0;
unsigned long lastPressTimeMode = 0;

///////////////////////// GLOBAL VARIABLES ///////////////////////////
int top_left = 49;
int top_right = 48;
bool colorFlag = 0;
bool modeFlag = 0;
bool firstLoop = true;
int mode = 0;

/////////////////////////////// MODES & COLORS ////////////////////////////////////       
enum {startup, solid, pulse, breathe, rainbowWave, theaterChase, brightnessChange};
int currentMode = startup;
// Red, orange, yellow, blue, green, purple, pink, teal
int colors[9][3] = {{145,145,125}, {255, 0, 0}, {255, 30, 0}, {240, 100, 0}, {0, 255, 0}, {0, 55, 255}, {105, 0, 90}, {255, 0, 75}, {0, 170, 60}};
int currColor = 0;

void setup() {
  pinMode(COLBUT, INPUT);
  pinMode(MODEBUT, INPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  strip.begin();           
  strip.show();
  strip.setBrightness(210); // Set BRIGHTNESS to max
}

void loop() {
  switch(currentMode) {
    case startup:
      {
        int melody[] = {NOTE_G4, NOTE_A4, NOTE_D4, NOTE_A4, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_D4, NOTE_A4, NOTE_G4};
        int noteDurations[] = {2, 2, 3, 2, 2, 12, 12, 5, 2, 2, 3, 2, 2.5};
        for (int thisNote = 0; thisNote < 13; thisNote++) {
          // to calculate the note duration, take one second divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(BUZZER, melody[thisNote], noteDuration);
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing:
          noTone(BUZZER);
        }
        currentMode = solid;
      }
    case solid:
      {
        if (firstLoop == true) {
          setStrip(colors[currColor][0], colors[currColor][1], colors[currColor][2], 255);
          firstLoop = false;
        }
        if (digitalRead(MODEBUT) == HIGH && ((millis() - lastPressTimeMode) >= 300)) {
          Serial.println("modeflag");
          lastPressTimeMode = millis();
          modeFlag = false;
          firstLoop = true;
          currentMode = pulse;
        } else if (digitalRead(COLBUT) == HIGH && ((millis() - lastPressTimeColor) >= 300)) {
          //Serial.println(millis() - lastPressTimeColor);
          Serial.println("colorflag");
          lastPressTimeColor = millis();
          if (currColor >= 8) {
            firstLoop = true;
            currColor = 0;
          } else {
            firstLoop = true;
            currColor++;
          }
          colorFlag = false;
        }
        break;
      }
    case pulse:
      {
        pulseEffect(colors[currColor][0], colors[currColor][1], colors[currColor][2]);
        if (modeFlag == true) {
          modeFlag = false;
          currentMode = breathe;
        } else if (colorFlag == true) {
          if (currColor >= 8) {
            currColor = 0;
          } else {
            currColor++;
          }
          colorFlag = false;
        }
        break;
      }
    case breathe:
      {
        breatheEffect(colors[currColor][0], colors[currColor][1], colors[currColor][2]);
        if (modeFlag == true) {
          modeFlag = false;
          currentMode = rainbowWave;
          break;
        } else if (colorFlag == true) {
          if (currColor >= 8) {
            currColor = 0;
          } else {
            currColor++;
          }
          colorFlag = false;
        }
        break;
      }
    case rainbowWave:
      {
        rainbow(10);
        if (modeFlag == true) {
          modeFlag = false;
          currentMode = theaterChase;
        }
        break;
      }
    case theaterChase:
      {
        rainbowTheaterChase(150);
        if (modeFlag == true) {
          modeFlag = false;
          currentMode = solid;
        }
        break;
      }    
  }
}

void brighten(int pixel, int R, int G, int B, int factor, int basebr = 255, int fadedur = 60) {
  int j;
  for (j = basebr; j < factor; j+=fadedur) {
    setPixel(pixel, R, G, B, j);
    strip.show();
  }  
}

void dim(int pixel, int R, int G, int B, int factor, int basebr = 255, int fadedur = 70) {
  int j;
  for (j = factor; j > basebr; j-=fadedur) {
    setPixel(pixel, R, G, B, j);
    strip.show();
  }
  if ((digitalRead(COLBUT) == HIGH) && ((millis() - lastPressTimeColor) >= 300)) {
    lastPressTimeColor = millis();
    colorFlag = true;
  } 
  if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
    lastPressTimeMode = millis();
    modeFlag = true;
  } 
}

void setPixel(int pixel, int r, int g, int b, int brightness) {
  strip.setPixelColor(pixel, (brightness*r/255) , (brightness*g/255), (brightness*b/255));
}

void setStrip(int r, int g, int b, int brightness) {
  for(int i=0; i<LED_COUNT; i++) {
    setPixel(i, r, g, b, brightness); 
  }
  strip.show();
}

void pulseEffect(int R, int G,int B) {
  int baselineBrightness = 12;
  setStrip(R, G, B, baselineBrightness);
  int br8 = 255;
  int br7 = 245;
  int br6 = 235;
  int br5 = 225;
  int br4 = 215;
  int br3 = 205;
  int br2 = 195;
  int br1 = 185;
  int offset = 0;
  while (true) {
    int curr_right = top_right-offset;
    int curr_left = top_left+offset;
    if ((digitalRead(COLBUT) == HIGH) && ((millis() - lastPressTimeColor) >= 100)) {
      lastPressTimeColor = millis();
      colorFlag = true;
      break;
    }
    if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 100)) {
      lastPressTimeMode = millis();
      modeFlag = true;
      break;
    }
    // In this particular instance, I actually found nested if statements to be a clearer implementation for the LED dimming
    // of trailing LEDs rather than a switch case.
    if (offset >= 1) {
      dim(curr_right+1, R, G, B, br8, br7);
      if (modeFlag || colorFlag) {break;}
      dim(curr_left-1, R, G, B, br8, br7);
      if (offset >= 2) {
        dim(curr_right+2, R, G, B, br7, br6);
        if (modeFlag || colorFlag) {break;}
        dim(curr_left-2, R, G, B, br7, br6);
        if (offset >= 3) {
          dim(curr_right+3, R, G, B, br6, br5);
          if (modeFlag || colorFlag) {break;}
          dim(curr_left-3, R, G, B, br6, br5);
          if (offset >= 4) {
            dim(curr_right+4, R, G, B, br5, br4);
            if (modeFlag || colorFlag) {break;}
            dim(curr_left-4, R, G, B, br5, br4);
            if (offset >= 5) {
              dim(curr_right+5, R, G, B, br4, br3);
              if (modeFlag || colorFlag) {break;}
              dim(curr_left-5, R, G, B, br4, br3);
              if (offset >= 6) {
                dim(curr_right+6, R, G, B, br3, br2);
                if (modeFlag || colorFlag) {break;}
                dim(curr_left-6, R, G, B, br3, br2);
                if (offset >= 7) {
                  dim(curr_right+7, R, G, B, br2, br1);
                  if (modeFlag || colorFlag) {break;}
                  dim(curr_left-7, R, G, B, br2, br1);
                  if (offset >= 8) {
                    dim(curr_right+8, R, G, B, br1, baselineBrightness);
                    if (modeFlag || colorFlag) {break;}
                    dim(curr_left-8, R, G, B, br1, baselineBrightness);
                  }
                }
              }
            }
          }
        }
      }
    }
    brighten(curr_right, R, G, B, br8, baselineBrightness, 110);
    brighten(curr_left, R, G, B, br8, baselineBrightness, 110);
    if ((digitalRead(COLBUT) == HIGH) && ((millis() - lastPressTimeColor) >= 300)) {
      lastPressTimeColor = millis();
      colorFlag = true;
      break;
    }
    if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
      lastPressTimeMode = millis();
      modeFlag = true;
      break;
    }
    offset += 1;
    if (offset >= 55) { 
      offset = 0;
    }
  }
}

void breatheEffect(int R, int G,int B) {
  int brightnessVal = 255;
  bool countingDown = true;
  while(true) {
    if (brightnessVal <= 255) {
      setStrip(R,G,B,brightnessVal);
    }
    if (countingDown && (brightnessVal >= 5)) {
      brightnessVal--;
    } else if (!countingDown && (brightnessVal <= 255)) {
      brightnessVal += 2;
      if (brightnessVal >= 255) {
        countingDown = true;
      }
    } else {
      countingDown = false;
    }
    if ((digitalRead(COLBUT) == HIGH) && ((millis() - lastPressTimeColor) >= 300)) {
      lastPressTimeColor = millis();
      colorFlag = true;
      break;
    }
    if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
      lastPressTimeMode = millis();
      modeFlag = true;
      break;
    } 
  }
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; ;firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show();
    delay(wait);
    if (firstPixelHue >= 65536) {
      firstPixelHue = 0;
    }
    if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
      lastPressTimeMode = millis();
      modeFlag = true;
      break;
    }
  }
}

void rainbowTheaterChase(int wait) {
  modeFlag = false;
  int firstPixelHue = 0;    
  for(int a=0; a<30; a++) { 
    for(int b=0; b<3; b++) { 
      strip.clear();         
      for(int c=b; c<strip.numPixels(); c += 3) {
        int hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();
      unsigned long startT = millis();
      unsigned long endT = startT;
      while ((endT-startT) <= 125) { //Delay while reading button
        if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
          lastPressTimeMode = millis();
          modeFlag = true;
          break;
        }
        endT = millis();
      }
      if (modeFlag == true) {
        break;
      }
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
    if (modeFlag == true) {
        break;
    }
    if ((digitalRead(MODEBUT) == HIGH) && ((millis() - lastPressTimeMode) >= 300)) {
      lastPressTimeMode = millis();
      modeFlag = true;
      break;
    }
    if (a >= 29) {
      a = 0;
    }
  }
}
