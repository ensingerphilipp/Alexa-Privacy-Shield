#include <Arduino.h>
#include <driver/i2s.h>
#include <esp_task_wdt.h>
#include "I2SMicSampler.h"
#include "ADCSampler.h"
#include "I2SOutput.h"
#include "config.h"
#include "Application.h"
#include "SPIFFS.h"
#include "IndicatorLight.h"
#include "Wire.h"
#include <Adafruit_NeoPixel.h>

// i2s config for using the internal ADC
i2s_config_t adcI2SConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_LSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s config for reading from both channels of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

// i2s speaker pins
i2s_pin_config_t i2s_speaker_pins = {
    .bck_io_num = I2S_SPEAKER_SERIAL_CLOCK,
    .ws_io_num = I2S_SPEAKER_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_SPEAKER_SERIAL_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE};

// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
  Application *application = static_cast<Application *>(param);

  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  while (true)
  {
    // wait for some audio samples to arrive
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0)
    {
      application->run();
    }
  }
}

//-------------------//
//---- LED Zeugs ----//
//-------------------//

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

class Led {
  private:
    char colorValue1=0,colorValue2=0,colorValue3 = 0; //RGB Values
  public:
    Led() {
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

Led leds[12];
boolean value_changed = false;
boolean show_actual_led = true;
state assistant_state=normal;
Adafruit_NeoPixel strip(12, 16, NEO_GRB + NEO_KHZ800);

/*void receiveEvent(int howMany)
{
  char led_address = 0;
  while(1 < Wire.available())
  {
    led_address = Wire.read();
    //Serial.print("c: ");
    //Serial.print(led_address, HEX); 
  }
  char led_value = Wire.read();
  //Serial.print(" v: ");
  //Serial.println(led_value, HEX); 
}*/
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

void setup()
{

  for(int i = 0; i<12; i++){
    leds[i]=Led();
  }
  strip.begin();
  strip.fill(strip.Color(0,0,200), 1, 12);
  strip.show();
  delay(1000);
  strip.fill(strip.Color(0,0,0), 1, 12);
  strip.show();
  Serial.begin(115200);
  Wire.onReceive(receiveEvent);
  Wire.begin(0x3f,21,22,100000);


  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);

  // startup SPIFFS for the wav files
  // SPIFFS.begin();
  // make sure we don't get killed for our long running tasks
  esp_task_wdt_init(10, false);

  // start up the I2S input (from either an I2S microphone or Analogue microphone via the ADC)
#ifdef USE_I2S_MIC_INPUT
  // Direct i2s input from INMP441 or the SPH0645
  I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);
#else
  // Use the internal ADC
  I2SSampler *i2s_sampler = new ADCSampler(ADC_UNIT_1, ADC_MIC_CHANNEL);
#endif

  // indicator light to show when we are listening
  IndicatorLight *indicator_light = new IndicatorLight();

  // create our application
  Application *application = new Application(i2s_sampler, indicator_light);

  // set up the i2s sample writer task
  TaskHandle_t applicationTaskHandle;
  xTaskCreate(applicationTask, "Application Task", 8192, application, 1, &applicationTaskHandle);

  // start sampling from i2s device - use I2S_NUM_0 as that's the one that supports the internal ADC
#ifdef USE_I2S_MIC_INPUT
  i2s_sampler->start(I2S_NUM_0, i2sMemsConfigBothChannels, applicationTaskHandle);
#else
  i2s_sampler->start(I2S_NUM_0, adcI2SConfig, applicationTaskHandle);
#endif

}

void loop()
{
  vTaskDelay(1);
}