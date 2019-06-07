#include <Arduino.h>
#include <DigiKeyboard.h>
#include <A85_PCF8574.h>
// SDA - P0 - 0 // CLK - P2 - 2

#define KEY_ESC         41
#define KEY_BACKSPACE   42
#define KEY_TAB         43
#define KEY_PRT_SCR     70
#define KEY_HOME        74
#define KEY_DELETE      76
#define KEY_END         77
#define KEY_ARROW_RIGHT 0x4F
#define KEY_ARROW_DOWN  0x51
#define KEY_ARROW_UP    0x52

#define PCF8574_ADDRESS (0x20)
A85_PCF8574 pcf20;

unsigned int buttonMillis;

void fnSendKeyStroke(uint8_t data_in){
  DigiKeyboard.sendKeyStroke(0);
  switch(data_in){
    case 0:
     DigiKeyboard.sendKeyStroke(KEY_C, MOD_CONTROL_LEFT);
     break;
    case 1:
     DigiKeyboard.sendKeyStroke(KEY_HOME);
     break;
    case 2:
     DigiKeyboard.sendKeyStroke(KEY_S, MOD_GUI_LEFT | MOD_SHIFT_LEFT );
     break;
    case 3:
     DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);
     break;
    case 4:
     DigiKeyboard.sendKeyStroke(KEY_V, MOD_CONTROL_LEFT);
     break;
    case 5:
     DigiKeyboard.sendKeyStroke(KEY_END);
     break;
    case 6:
     DigiKeyboard.sendKeyStroke(KEY_L, MOD_GUI_LEFT);
     break;
    case 7:
     DigiKeyboard.sendKeyStroke(KEY_DELETE, MOD_ALT_LEFT | MOD_CONTROL_LEFT );
     break;
  }
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(200);
}

void setup() {
  pcf20.begin(PCF8574_ADDRESS);
}

void loop() {
  unsigned int currentMillis = millis();
  DigiKeyboard.update();
  
  //Limit button read to 20 times a second
  //Fast enough for most buttons 
  //but this way you don't have a dimmer output because it's blanked during button read
  //a read takes 460us t 16Mhz Arduino and normal I2C speed.
  if(currentMillis - buttonMillis >= 50){
    buttonMillis = currentMillis;
    pcf20.setByte(B11111111);
    uint8_t inputStates = pcf20.getByte();
    for(uint8_t i = 0; i < 8; i++){
      bool bitval = bitRead(inputStates, i);
      if (bitval) fnSendKeyStroke(i);
    }
  }
}