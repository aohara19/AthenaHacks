

/*
    Hello World.ino
    2013 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author:Loovee
    2013-9-18

    Grove - Serial LCD RGB Backlight demo.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Wire.h>
#include <avr/pgmspace.h>
#include <rgb_lcd.h>
#include <Music.h>
#include "MMA7660.h"
MMA7660 accelemeter;

rgb_lcd lcd;

const int buzzer = 6; //Define pin 10, can use other PWM pins  (5,6 or 9)
//Note pins 3 and 11 can't be used when using the tone function in Arduino Uno
const int songspeed = 1.5; //Change to 2 for a slower version of the song, the bigger the number the slower the song

//*****************************************


const int buttonPin = 2;
const int buttonPin2 = 4;
int colorR = 250;
int colorG = 250;
int colorB = 250;
int buttonState = 0;
int buttonState2 = 0;
long randNumber;
int score = 0;
bool again = 0;
void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);
  accelemeter.init();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  Serial.begin(9600);
  delay(1000);
}

void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  int8_t x;
  int8_t y;
  int8_t z;
  float ax, ay, az;
  float ax_old, y_old, z_old;
  unsigned long timeOld;
  float maxChange = 0;
  bool survived = 0;
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  buttonState = digitalRead(buttonPin);
  if (buttonState) {
    if (!again) {
      lcd.print(F("Press the blue "));
      lcd.setCursor(0, 1);
      lcd.println(F("button to start "));
    }
    if (again) {
      colorR = 250;
      colorB = 250;
      colorG=255;
      lcd.setRGB(colorR, colorG, colorB);
      lcd.setCursor(0, 0);
      lcd.print(F("                "));
      lcd.setCursor(0, 0);
      lcd.print(F("Hold blue to"));
      lcd.setCursor(0, 1);
      lcd.print(F("             "));
      lcd.setCursor(0, 1);
      lcd.print(F("continue"));
      delay(3000);
      lcd.setCursor(0, 0);
      lcd.print(F("Hold red      "));
      lcd.setCursor(0, 1);
      lcd.print(F("to exit     "));
      delay(2000);
      buttonState2 = digitalRead(buttonPin2);
      /*
      lcd.setCursor(0,1);
      lcd.println(F("           "));
      lcd.setCursor(0,1);
      lcd.println(buttonState2);
      delay(1000);
      lcd.println(buttonState2);
      delay(2000);*/
   if (!buttonState2) {
      lcd.setCursor(0,0);
      lcd.println(F("Exiting...."));
      lcd.setCursor(0,1);
      lcd.println(F("               "));
      delay(2000);
      lcd.clear();
      exit(0);
      }
    }

  }
  if (!buttonState) {
    again = 1;
    lcd.setCursor(0, 1);
    lcd.println(F("                    "));
    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print(F("Start Running :}"));
    for (int i = 0; i < 39; i++) {
      //203 is the total number of music notes in the song
      int wait = pgm_read_word_near(duration + i) * songspeed;
      tone(buzzer, pgm_read_word_near(notes + i), wait);      //tone(pin,frequency,duration)

      delay(wait);
    }
    colorG = 0;
    colorB = 0;
    colorR = 255;
    lcd.setRGB(colorR, colorG, colorB);
    lcd.println(F("             "));
    lcd.setCursor(0, 0);
    lcd.println(F("Run faster!    "));
    accelemeter.getAcceleration(&ax, &ay, &az);
    timeOld = millis();
    ax_old = ax;
    while (millis() - timeOld < 5000) {
      accelemeter.getAcceleration(&ax, &ay, &az);
      if (ax - ax_old > maxChange) {
        maxChange = ax - ax_old;
      }
    }
    if (maxChange > 1.2) {
      survived = true;
    }
    if (!survived) {
      lcd.setCursor(0, 0);
      colorG = 0;
      colorR = 0;
      colorB = 250;
      lcd.setRGB(colorR, colorG, colorB);
      lcd.println(F("You Died!     "));
      delay(1000);
    }
    else {
      lcd.setCursor(0, 0);
      colorR = 10;
      colorB = 20;
      colorG = 250;
      lcd.setRGB(colorR, colorG, colorB);
      lcd.println(F("Congratulations    "));
      lcd.setCursor(0, 1);
      lcd.println(F("you lived!    "));
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.println(F("              "));
      score++;

      colorR = 255;
      colorB = 255;
      colorG = 250;
      lcd.setRGB(colorR, colorG, colorB);
      lcd.setCursor(0, 0);
      lcd.println(F("Start running     "));
      lcd.setCursor(0, 1);
      lcd.println(F("again!        "));
      for (int i = 0; i < 19; i++) {       //203 is the total number of music notes in the song

        int wait = pgm_read_word_near(duration + i) * songspeed;
        tone(buzzer, pgm_read_word_near(notes + i), wait);      //tone(pin,frequency,duration)
        delay(wait);

        //delay(10);
      }
      colorB = 10;
      colorG = 10;
      lcd.setCursor(0, 1);
      lcd.println(F("      "));
      lcd.setRGB(colorR, colorG, colorB);
      lcd.setCursor(0, 0);
      lcd.println(F("Jump!       "));
      timeOld = millis();
      accelemeter.getXYZ(&x, &y, &z);
      maxChange = 0;
      y_old = y;
      while (millis() - timeOld < 2000) {
        accelemeter.getXYZ(&x, &y, &z);
        if (y - y_old > maxChange) {
          maxChange = y - y_old;
        }
      }
      if (maxChange > 20) {
        survived = true;
      }
      else {
        survived = false;
      }
      if (!survived) {
        lcd.setCursor(0, 0);
        colorG = 0;
        colorR = 0;
        colorB = 250;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.println(F("You got caught!     "));
        delay(2000);
      }
      else {
        colorB = 255;
        colorG = 255;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.setCursor(0, 0);
        lcd.println(F("Keep going!    "));
        score++;
        for (int i = 19; i < 38; i++) {       //203 is the total number of music notes in the song
          int wait = pgm_read_word_near(duration + i) * songspeed;
          tone(buzzer, pgm_read_word_near(notes + i), wait);         //tone(pin,frequency,duration)
          delay(wait);
          //delay(10);
        }
        colorB = 10;
        colorG = 10;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.setCursor(0, 0);
        lcd.println(F("Jump!       "));
        timeOld = millis();
        accelemeter.getXYZ(&x, &y, &z);
        maxChange = 0;
        y_old = y;
        while (millis() - timeOld < 2000) {
          accelemeter.getXYZ(&x, &y, &z);
          if (y - y_old > maxChange) {
            maxChange = y - y_old;
          }
        }
        if (maxChange > 18) {
          survived = true;
        }
        else {
          survived = false;
        }
        if (!survived) {
          lcd.setCursor(0, 0);
          colorG = 0;
          colorR = 0;
          colorB = 250;
          lcd.setRGB(colorR, colorG, colorB);
          lcd.println(F("You got caught!     "));
          delay(2000);
        }
        else{
        colorB = 255;
        colorG = 255;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.setCursor(0, 0);
        lcd.println(F("Keep Going!   "));
        score++;
        for (int i = 39; i < 59; i++) {       //203 is the total number of music notes in the song
          int wait = pgm_read_word_near(duration + i) * songspeed;
          tone(buzzer, pgm_read_word_near(notes + i), wait);         //tone(pin,frequency,duration)
          delay(wait);
          //delay(10);
        }
        colorB = 10;
        colorG = 10;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.setCursor(0, 0);
        lcd.println(F("Jump!       "));
        timeOld = millis();
        accelemeter.getXYZ(&x, &y, &z);
        maxChange = 0;
        y_old = y;
        while (millis() - timeOld < 2000) {
          accelemeter.getXYZ(&x, &y, &z);
          if (y - y_old > maxChange) {
            maxChange = y - y_old;
          }
        }
        if (maxChange > 18) {
          survived = true;
        }
        else {
          survived = false;
        }
        if (!survived) {
          lcd.setCursor(0, 0);
          colorG = 0;
          colorR = 0;
          colorB = 250;
          lcd.setRGB(colorR, colorG, colorB);
          lcd.println(F("You were caught!     "));
          delay(2000);
        }
        else {
          score++;
          lcd.setCursor(0, 0);
          colorG = 255;
          colorR = 0;
          colorB = 0;
          lcd.setRGB(colorR, colorG, colorB);
          lcd.println(F("Keep going!  "));
          delay(2000);
          
          for (int i = 59; i < 59+27; i++) {       //203 is the total number of music notes in the song
            int wait = pgm_read_word_near(duration + i) * songspeed;
            tone(buzzer, pgm_read_word_near(notes + i), wait);         //tone(pin,frequency,duration)
            delay(wait);
          }
            colorB = 10;
          colorG = 10;
          lcd.setRGB(colorR, colorG, colorB);
          lcd.setCursor(0, 0);
          lcd.println(F("Sidestep!       "));
          timeOld = millis();
          accelemeter.getXYZ(&x, &y, &z);
          maxChange = 0;
          z_old = z;
          while (millis() - timeOld < 2000) {
            accelemeter.getXYZ(&x, &y, &z);
            if (z - z_old > maxChange) {
              maxChange = z - z_old;
            }
            if(z_old - z>maxChange){
              maxChange = z_old -z;
            }
          }
          if (maxChange > 20) {
            survived = true;
          }
          else {
            survived = false;
          }
          if(!survived){
            lcd.setCursor(0, 0);
            colorG = 0;
            colorR = 0;
            colorB = 250;
            lcd.setRGB(colorR, colorG, colorB);
            lcd.println(F("You were caught!     "));
            delay(2000);
          }
          else {
            score++;
            lcd.setCursor(0, 0);
            colorG = 255;
            colorR = 0;
            colorB = 0;
            lcd.setRGB(colorR, colorG, colorB);
            lcd.println(F("You got away!  "));
            delay(2000);
            lcd.setCursor(0,0);
            lcd.print(F("Your Score:"));
            lcd.println(score);
            delay(2000);
          }
        }
        }
      }
    }
  }
  delay(500);

}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
