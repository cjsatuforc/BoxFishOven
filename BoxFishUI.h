//
// Title: BoxFish UI
// Author: Orange Cat
// Date: 2015-10-24
//
// BoxFishUI is a class to implement a consistent user interface on 2x16 line displays
// it divides the display up into 3 parts. The top line is reserved for the menu system. The bottom line is split into left (info) and right (status).
// the status area is 5 characters, the info area is 10.
//
// It performs the button reading and deboucing and the driving a MenuBackend menu system. So all the caller has to do is setup the menu hierarchy,
// create a callback and wait for menu options to be selected.
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//

#ifndef BOXFISH_UI_H
#define BOXFISH_UI_H

#include <Arduino.h>
#include <MenuBackend.h>
#include <String.h>

// define to use the Adafruit LCD.
#define BOXFISH_USE_ADAFRUIT_LCD


#ifdef BOXFISH_USE_ADAFRUIT_LCD  // BOXFISH_USE_ADAFRUIT_LCD is defined (or not) in BoxFishUI.h
  #include <Adafruit_RGBLCDShield.h>
  #define LCD_CLASS Adafruit_RGBLCDShield
#else
  #include <LiquidCrystal.h>
  #define LCD_CLASS LiquidCrystal
#endif


enum BoxFishButton {
  kBoxFishButtonNone,
  kBoxFishButtonUp = 0x01,
  kBoxFishButtonDown = 0x02,
  kBoxFishButtonLeft = 0x04,
  kBoxFishButtonRight = 0x08,
  kBoxFishButtonSelect = 0x10
};

typedef void (*BoxFishMenuCallback)(int);

static const char kBoxFishDegreeChar = '\1';     // character which prints a degree symbol on the LCD display
static const unsigned long kDebouceDelay = 40;   // key debouce delay in milliseconds
  
class BoxFishUI : public LCD_CLASS {
  public:
    BoxFishUI();

  public:
    void begin(const char program_name[], const char program_version[], BoxFishMenuCallback callback);
    MenuItemRef getRootMenu();
    MenuItemRef getCurrentMenu();
    
    void menuNavigate();        // call each time through loop()
    void menuGotoRoot();

    void writeStatus(const String& stat);
    void writeInfo(const String& info);
    void overwriteMenu(const String& menu);
    void redisplayCurrentMenu();
    
    void beep();
    BoxFishButton readButton();
    BoxFishButton lastButton();

    void softReset();   // software reset of microcontroller

  protected:
    void writeExactlyAt(unsigned int x, unsigned int y, const String& str, unsigned int width);
    void lcdSetup();
    void displaySplash();
    
  private:
    static void menuChangeEventCallback(MenuChangeEvent changed);
    static void menuUseEventCallback(MenuUseEvent used);
    void menuDisplayMenu(const MenuItem& menu);
    BoxFishButton debounce(BoxFishButton button);
    
  private:
    MenuBackend menu_;
    BoxFishButton button_state_;
    BoxFishButton last_button_reading_;
    BoxFishButton debounced_button_;
    unsigned long debounce_time_;
    BoxFishMenuCallback callback_func_;
    const char* prog_name_;
    const char* prog_version_;
};

#endif // BOXFISH_UI_H

