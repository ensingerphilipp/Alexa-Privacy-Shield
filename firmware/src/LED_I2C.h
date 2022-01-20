#include "Wire.h"
#include <Adafruit_NeoPixel.h>
#include "LED.h"


LED leds[12];
boolean value_changed = false;
boolean show_actual_led = true;
state assistant_state=normal;
Adafruit_NeoPixel strip(12, 16, NEO_GRB + NEO_KHZ800);

void receiveEvent(int howMany)
{
  char led_address = 0;
  while(1 < Wire.available())
  {
    led_address = Wire.read();
    //Serial.print("c: ");
    //Serial.print(led_address, HEX); 
  }
  char led_value = Wire.read();
  //Serial.print(" x: "); 
  //Serial.println(led_value, HEX);
  switch(led_address){
    case 0x01:
      if(leds[0].getValue1()!=led_value){
        leds[0].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x02:
      if(leds[0].getValue2()!=led_value){
        leds[0].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x03:
      if(leds[0].getValue3()!=led_value){
        leds[0].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x04:
      if(leds[1].getValue1()!=led_value){
        leds[1].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x05:
      if(leds[1].getValue2()!=led_value){
        leds[1].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x06:
      if(leds[1].getValue3()!=led_value){
        leds[1].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x07:
      if(leds[2].getValue1()!=led_value){
        leds[2].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x08:
      if(leds[2].getValue2()!=led_value){
        leds[2].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x09:
      if(leds[2].getValue3()!=led_value){
        leds[2].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x0A:
      if(leds[3].getValue1()!=led_value){
        leds[3].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x0B:
      if(leds[3].getValue2()!=led_value){
        leds[3].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x0C:
      if(leds[3].getValue3()!=led_value){
        leds[3].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x0D:
      if(leds[4].getValue1()!=led_value){
        leds[4].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x0E:
      if(leds[4].getValue2()!=led_value){
        leds[4].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x0F:
      if(leds[4].getValue3()!=led_value){
        leds[4].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x10:
      if(leds[5].getValue1()!=led_value){
        leds[5].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x11:
      if(leds[5].getValue2()!=led_value){
        leds[5].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x12:
      if(leds[5].getValue3()!=led_value){
        leds[5].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x13:
      if(leds[6].getValue1()!=led_value){
        leds[6].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x14:
      if(leds[6].getValue2()!=led_value){
        leds[6].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x15:
      if(leds[6].getValue3()!=led_value){
        leds[6].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x16:
      if(leds[7].getValue1()!=led_value){
        leds[7].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x17:
      if(leds[7].getValue2()!=led_value){
        leds[7].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x18:
      if(leds[7].getValue3()!=led_value){
        leds[7].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x19:
      if(leds[8].getValue1()!=led_value){
        leds[8].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x1A:
      if(leds[8].getValue2()!=led_value){
        leds[8].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x1B:
      if(leds[8].getValue3()!=led_value){
        leds[8].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x1C:
      if(leds[9].getValue1()!=led_value){
        leds[9].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x1D:
      if(leds[9].getValue2()!=led_value){
        leds[9].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x1E:
      if(leds[9].getValue3()!=led_value){
        leds[9].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x1F:
      if(leds[10].getValue1()!=led_value){
        leds[10].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x20:
      if(leds[10].getValue2()!=led_value){
        leds[10].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x21:
      if(leds[10].getValue3()!=led_value){
        leds[10].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x22:
      if(leds[11].getValue1()!=led_value){
        leds[11].setValue1(led_value);
        value_changed = true;
      }
      break;
    case 0x23:
      if(leds[11].getValue2()!=led_value){
        leds[11].setValue2(led_value);
        value_changed = true;
      }
      break;
    case 0x24:
      if(leds[11].getValue3()!=led_value){
        leds[11].setValue3(led_value);
        value_changed = true;
      }
      break;
    case 0x25:
      if(led_value==0&&value_changed){
        value_changed = false;
        char off_count =0;
        
        for(int led = 0; led<12;led++){
          switch(leds[led].getColor()){
            case orange: assistant_state=setup_mode;
              break;
            case purple: assistant_state=do_not_disturb;
              break;
            case blue:
            case cyan: assistant_state=listening;
              break;
            case red: assistant_state=mute;
              break;
            case off: off_count++;
              break;
          }
          //y=kx+d
          //k = y/x
          float k = 1;
          char value1 = leds[led].getValue1();
          char value2 = leds[led].getValue2();
          char value3 = leds[led].getValue3();
          if(!(value1==0&&value2==0&&value3==0)&&value1<255&&value2<255&&value3<255){
            if(value1>=value2&&value1>=value3){
              k = (float)255/value1;
            }
            else if(value2>=value1&&value2>=value3){
              k = (float)255/value2;
            }
            else if(value3>=value1&&value3>=value2){
              k = (float)255/value3;
            }
            value1 = value1*k;
            value2 = value2*k;
            value3 = value3*k;
          }
          /*Serial.print(value1, HEX);
          Serial.print(" : ");
          Serial.print(value2, HEX);
          Serial.print(" : ");
          Serial.println(value3, HEX);
          Serial.print("k: ");
          Serial.println(k, DEC);
          leds[led].printVals();*/
          strip.setPixelColor(led+1, strip.Color(value1,value2,value3));
        }
        if(off_count==12){
          assistant_state=normal;
        }
        if(show_actual_led){
          strip.show();
        }
        //leds[0].printVals();
        
      }
      break;
  }
}

class LED_I2C
{
  public:
      LED_I2C(){
          for(int i = 0; i<12; i++){
              leds[i]=LED();
          }
          strip.begin();
          strip.fill(strip.Color(0,0,200), 1, 12);
          strip.show();
          delay(1000);
          strip.fill(strip.Color(0,0,0), 1, 12);
          strip.show();
          
          // Für release einkommentieren und 2.00 version mit backmerged wire verwenden
          //Wire.onReceive(receiveEvent);
          //Wire.begin(0x3f,21,22,100000);
      }
    
};