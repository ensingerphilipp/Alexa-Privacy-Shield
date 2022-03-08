#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "WakeWordRecognizedState.h"
#include "IndicatorLight.h"
#include "DACOutput.h"
#include "../config.h"
#include <string.h>
#include "LED.h"

#define WINDOW_SIZE 320
#define STEP_SIZE 160
#define POOLING_SIZE 6
#define AUDIO_LENGTH 16000


WakeWordRecognizedState::WakeWordRecognizedState(I2SSampler *sample_provider, IndicatorLight *indicator_light)
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_indicator_light = indicator_light;
}

void WakeWordRecognizedState::enterState()
{
    m_indicator_light->setState(ON);
    m_start_time = millis();
    m_elapsed_time = 0;
    m_last_audio_position = -1;
    m_dac_output = new DACOutput();
    //Serial.println("Ready for Passthrough");
    return;
}

void WakeWordRecognizedState::wakeup_assistant(int pin)
{
    digitalWrite(pin, HIGH);
    vTaskDelay(100);
    digitalWrite(pin, LOW);
}

bool WakeWordRecognizedState::run()
{   
    extern state assistant_state;
    extern bool activation_button_pressed;
    extern int speech_assistant_activation_button_pin;

    //Wake up Speech Assistant
    wakeup_assistant(speech_assistant_activation_button_pin);

    //start DAC Output (Mic Passthrough)
    m_dac_output->start(m_sample_provider);
    vTaskDelay(200);

    Serial.println("Started Passthrough");

    //TODO: assistant not available/responding
    /*
    if(assistant_state!=listening){

    }
    */
    //wait until speech assistant exits listening state and react to button interrupt
    while(assistant_state==listening){
        if(activation_button_pressed){
            Serial.println("Activation Button pressed");
            activation_button_pressed = false;
            wakeup_assistant(speech_assistant_activation_button_pin);
        }
        vTaskDelay(100);
    }
    m_dac_output->stop();
    Serial.println("Stopped Passthrough");
        
    return true;
}

void WakeWordRecognizedState::exitState()
{   
    delete m_dac_output;
    m_dac_output = NULL;
    uint32_t free_ram = esp_get_free_heap_size();
    //Serial.printf("Free ram after WakeWordRecognized cleanup %d\n", free_ram);
}