#include "LED.h"
#include "Wire.h"
#include <Adafruit_NeoPixel.h>
#include "Enums.h"

assistant_state current_state = normal;
LED assistant_leds[12];
bool value_changed = false;
Adafruit_NeoPixel strip(12, 18, NEO_GRB + NEO_KHZ800);

void setLEDColor(char led_id, char red_value, char green_value, char blue_value){
    //y=kx+d
    //k = y/x
    float k = 1;
    char value1 = red_value;
    char value2 = green_value;
    char value3 = blue_value;
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
    strip.setPixelColor(led_id, strip.Color(value1,value2,value3));
}

void showLEDs(){
    strip.show();
}

void receiveEvent(int howMany)
{
    char led_address = 0;
    char assistant_led=0;
    
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
        case 0x02:
        case 0x03:
            assistant_led=0;
        break;
        case 0x04:
        case 0x05:
        case 0x06:
            assistant_led=1;
        break;
        case 0x07:
        case 0x08:
        case 0x09:
            assistant_led=2;
        break;
        case 0x0A:
        case 0x0B:
        case 0x0C:
            assistant_led=3;
        break;
        case 0x0D:
        case 0x0E:
        case 0x0F:
            assistant_led=4;
        break;
        case 0x10:
        case 0x11:
        case 0x12:
            assistant_led=5;
        break;
        case 0x13:
        case 0x14:
        case 0x15:
            assistant_led=6;
        break;
        case 0x16:
        case 0x17:
        case 0x18:
            assistant_led=7;
        break;
        case 0x19:
        case 0x1A:
        case 0x1B:
            assistant_led=8;
        break;
        case 0x1C:
        case 0x1D:
        case 0x1E:
            assistant_led=9;
        break;
        case 0x1F:
        case 0x20:
        case 0x21:
            assistant_led=10;
        break;
        case 0x22:
        case 0x23:
        case 0x24:
            assistant_led=11;
        break;
        case 0x25:
        if(led_value==0&&value_changed){
            value_changed = false;
            char off_count =0;
            
            for(int led = 0; led<12;led++){
                switch(assistant_leds[led].getColor()){
                    case orange: current_state=setup_mode;
                    break;
                    case purple: current_state=do_not_disturb;
                    break;
                    case blue:
                    case cyan: current_state=listening;
                    break;
                    case red: current_state=mute;
                    break;
                    case off: off_count++;
                    break;
                }
                setLEDColor(led+1,assistant_leds[led].getValue(single_red_value),
                    assistant_leds[led].getValue(single_green_value),assistant_leds[led].getValue(single_blue_value));
            }
            showLEDs();
            if(off_count==12){
                current_state=normal;
            }
        }
        break;
        default: return;
    }

    single_color_value single_color_val;
    if(led_address % 3 == 1){
        single_color_val=single_red_value;
    }else if(led_address % 3 == 2){
        single_color_val=single_green_value;
    }else if(led_address % 3 == 0){
        single_color_val=single_blue_value;
    }
    if(assistant_leds[assistant_led].getValue(single_color_val)!=led_value){
        assistant_leds[assistant_led].setValue(single_color_val, led_value);
        value_changed = true;
    }
}

void init_I2C_and_LEDs(){
    strip.begin();
    strip.fill(strip.Color(0,0,200), 1, 12);
    strip.show();
    for(int i = 0; i<12; i++){
        assistant_leds[i]=LED();
    }
    Wire.onReceive(receiveEvent);
    Wire.begin(0x3f,21,22,100000);
    delay(1000);
    strip.fill(strip.Color(0,0,0), 1, 12);
    strip.show();
}
