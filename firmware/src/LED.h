#include "Arduino.h"

enum state{
  normal,
  boot,
  notification,
  listening,
  mute,
  setup_mode,
  recieving_call,
  active_call,
  do_not_disturb,  //also used for WiFi issues during setup mode
  guard
};

enum led_color{
  off,
  orange,
  yellow,
  purple,
  blue,
  cyan,
  red,
  green,
  white,
  none
};

class LED {
  private:
    char colorValue1=0,colorValue2=0,colorValue3 = 0; //RGB Values
  public:
    LED() {
    }
    
    void setValue1(char value) {
      this->colorValue1 = value;
    }
    
    void setValue2(char value) {
      this->colorValue2 = value;
    }
    
    void setValue3(char value) {
      this->colorValue3 = value;
    }

    char getValue1() {
      return colorValue1;
    }
    
    char getValue2() {
      return colorValue2;
    }
    
    char getValue3() {
      return colorValue3;
    } 

    void printVals(){
      Serial.print(colorValue1, HEX);
      Serial.print(" : ");
      Serial.print(colorValue2, HEX);
      Serial.print(" : ");
      Serial.println(colorValue3, HEX);
    }

    //get enum led_color from RGB values
    led_color getColor(){
      //-----------------------------//
      //---- Orange Color Values ----//
      //-----------------------------//
      if(colorValue1==0x21 && colorValue2==0x0B && colorValue3==0){
        return orange;
      }
      //-----------------------------//
      //---- Purple Color Values ----//
      //-----------------------------//
      else if(colorValue1==0x01 && colorValue2==0 && colorValue3==0x01){
        return purple;
      }
      else if(colorValue1==0x02 && colorValue2==0 && colorValue3==0x04){
        return purple;
      }
      else if(colorValue1==0x03 && colorValue2==0 && colorValue3==0x06){
        return purple;
      }
      else if(colorValue1==0x05 && colorValue2==0 && colorValue3==0x09){
        return purple;
      }
      else if(colorValue1==0x06 && colorValue2==0 && colorValue3==0x0B){
        return purple;
      }
      else if(colorValue1==0x07 && colorValue2==0 && colorValue3==0x0D){
        return purple;
      }
      else if(colorValue1==0x08 && colorValue2==0 && colorValue3==0x10){
        return purple;
      }
      else if(colorValue1==0x0A && colorValue2==0 && colorValue3==0x12){
        return purple;
      }
      else if(colorValue1==0x0B && colorValue2==0 && colorValue3==0x15){
        return purple;
      }
      else if(colorValue1==0x0D && colorValue2==0 && colorValue3==0x17){
        return purple;
      }
      //-----------------------------//
      //----- Blue Color Values -----//
      //-----------------------------//
      else if(colorValue1==0 && colorValue2==0 && colorValue3==0x1E){
        return blue;
      }
      //-----------------------------//
      //----- Cyan Color Values -----//
      //-----------------------------//
      else if(colorValue1==0 && colorValue2==0x1E && colorValue3==0x1E){
        return cyan;
      }
      else if(colorValue1==0 && colorValue2==0x15 && colorValue3==0x1E){
        return cyan;
      }
      else if(colorValue1==0 && colorValue2==0x24 && colorValue3==0x1E){
        return cyan;
      }
      else if(colorValue1==0 && colorValue2==0x17 && colorValue3==0x1E){
        return cyan;
      }
      else if(colorValue1==0 && colorValue2==0x1C && colorValue3==0x1C){
        return cyan;
      }
      //----------------------------//
      //----- Red Color Values -----//
      //----------------------------//
      else if(colorValue1>0 && colorValue2==0 && colorValue3==0){
        return red;
      }
      //----------------------------//
      //----- Off Color Values -----//
      //----------------------------//
      else if(colorValue1==0 && colorValue2==0 && colorValue3==0){
        return off;
      }
      else{
        return none;
      }
    }
};