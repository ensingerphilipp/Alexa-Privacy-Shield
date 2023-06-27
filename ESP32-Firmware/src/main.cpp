#include "Arduino.h"
#include <driver/i2s.h>
#include <esp_task_wdt.h>
#include "I2SMicSampler.h"
#include "config.h"
#include "Application.h"
#include "IndicatorLight.h"
#include <WiFi.h>
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>
#include "AssistantStateAndLEDHandler.h"

// i2s config for reading from both channels of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 128,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

// i2s external DAC pins (Leave at default if internal dac is used)
i2s_pin_config_t i2s_edac_pins = {
    .bck_io_num = I2S_EDAC_BCK,
    .ws_io_num = I2S_EDAC_WS,
    .data_out_num = I2S_EDAC_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE};

//Set to false to use external DAC
bool use_internal_dac = true;

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

bool activation_button_pressed = false; //variable used for external activation button detection

void IRAM_ATTR isr() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  
  
  if (interrupt_time - last_interrupt_time > 50) 
  {
    Serial.println("Button Change");
    //Serial.println("Start While");
    if(digitalRead(ASSISTANT_ACTIVATION_BUTTON_PIN)==LOW){
      digitalWrite(ASSISTANT_ACTIVATION_BUTTON_PIN, HIGH);
      Serial.println("Set High");
    }
    else{
      digitalWrite(ASSISTANT_ACTIVATION_BUTTON_PIN, LOW);
      Serial.println("Set Low");
      activation_button_pressed = true;
    }
    //Serial.println("End While");
    
  }
  last_interrupt_time = interrupt_time;
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting up");
  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());

  // Disable Wifi and Bluetooth to reduce DAC Noise and Power Consumption
  WiFi.mode(WIFI_OFF);
  btStop();
  esp_wifi_stop();
  esp_bt_controller_disable();

  //Initialize I2C Communnication for LED-Controller
  init_I2C_and_LEDs();

  pinMode(ASSISTANT_ACTIVATION_BUTTON_PIN, OUTPUT);
  digitalWrite(ASSISTANT_ACTIVATION_BUTTON_PIN, LOW);

  pinMode(EXT_ACTIVATION_BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(EXT_ACTIVATION_BUTTON_PIN, isr, CHANGE);

  // make sure we don't get killed for our long running tasks
  esp_task_wdt_init(10, false);

  // start up the I2S input 
  // Direct i2s input from INMP441 or the SPH0645
  I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);

  // indicator light to show when we are listening
  IndicatorLight *indicator_light = new IndicatorLight();

  // create our application
  Application *application = new Application(i2s_sampler, &i2s_edac_pins, use_internal_dac, indicator_light);

  // set up the i2s sample writer task
  TaskHandle_t applicationTaskHandle;
  xTaskCreate(applicationTask, "Application Task", 8192, application, 1, &applicationTaskHandle);

  // start sampling from i2s device
  i2s_sampler->start(I2S_NUM_1, i2sMemsConfigBothChannels, applicationTaskHandle);
}

void loop()
{
  vTaskDelay(1000);
}