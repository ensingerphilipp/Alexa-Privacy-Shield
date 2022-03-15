#include "Arduino.h"

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

enum single_color_value{
  single_red_value,
  single_green_value,
  single_blue_value
};

class LED{

  private:
  char led_red_value = 0, led_green_value = 0, led_blue_value = 0; //RGB Values
  public:
  void setValue(single_color_value color, char value) {
      switch(color){
          case single_red_value: led_red_value = value;
          break;
          case single_green_value: led_green_value = value;
          break;
          case single_blue_value: led_blue_value = value;
      }
  }

  char getValue(single_color_value color){
      switch(color){
          case single_red_value: return led_red_value;
          break;
          case single_green_value: return led_green_value;
          break;
          case single_blue_value: return led_blue_value;
      }
      return 0;
  }

  void printVals(){
      Serial.print(led_red_value, HEX);
      Serial.print(" : ");
      Serial.print(led_green_value, HEX);
      Serial.print(" : ");
      Serial.println(led_blue_value, HEX);
  }

  //get enum led_color from RGB values
  led_color getColor(){
      //-----------------------------//
      //---- Orange Color Values ----//
      //-----------------------------//
      if(led_red_value>0 && led_green_value>0 && led_blue_value==0){
          return orange;
      }
      //-----------------------------//
      //---- Purple Color Values ----//
      //-----------------------------//
      else if(led_red_value>0 && led_green_value==0 && led_blue_value>0){
          return purple;
      }
      //-----------------------------//
      //----- Blue Color Values -----//
      //-----------------------------//
      else if(led_red_value==0 && led_green_value==0 && led_blue_value>0){
          return blue;
      }
      //-----------------------------//
      //----- Cyan Color Values -----//
      //-----------------------------//
      else if(led_red_value==0 && led_green_value>0 && led_blue_value>0){
          return cyan;
      }
      //----------------------------//
      //----- Red Color Values -----//
      //----------------------------//
      else if(led_red_value>0 && led_green_value==0 && led_blue_value==0){
          return red;
      }
      //----------------------------//
      //----- Green Color Values -----//
      //----------------------------//
      else if(led_red_value==0 && led_green_value>0 && led_blue_value==0){
          return green;
      }
      //----------------------------//
      //----- White Color Values -----//
      //----------------------------//
      else if(led_red_value == led_green_value == led_blue_value >0){
          return white;
      }
      //----------------------------//
      //----- Off Color Values -----//
      //----------------------------//
      else if(led_red_value==0 && led_green_value==0 && led_blue_value==0){
          return off;
      }
      else{
          return none;
      }
  }

};
