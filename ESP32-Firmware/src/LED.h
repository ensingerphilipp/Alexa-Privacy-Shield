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
      if(colorValue1>0 && colorValue2>0 && colorValue3==0){
        return orange;
      }
      //-----------------------------//
      //---- Purple Color Values ----//
      //-----------------------------//
      else if(colorValue1>0 && colorValue2==0 && colorValue3>0){
        return purple;
      }
      //-----------------------------//
      //----- Blue Color Values -----//
      //-----------------------------//
      else if(colorValue1==0 && colorValue2==0 && colorValue3>0){
        return blue;
      }
      //-----------------------------//
      //----- Cyan Color Values -----//
      //-----------------------------//
      else if(colorValue1==0 && colorValue2>0 && colorValue3>0){
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