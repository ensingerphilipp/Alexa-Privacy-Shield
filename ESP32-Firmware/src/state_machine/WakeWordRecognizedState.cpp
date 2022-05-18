#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "WakeWordRecognizedState.h"
#include "IndicatorLight.h"
#include "DACOutput.h"
#include "config.h"
#include <string.h>
#include "Enums.h"

#define WINDOW_SIZE 320
#define STEP_SIZE 160
#define POOLING_SIZE 6
#define AUDIO_LENGTH 16000

WakeWordRecognizedState::WakeWordRecognizedState(I2SSampler *sample_provider, i2s_pin_config_t *i2s_edac_pins, bool use_internal_dac, IndicatorLight *indicator_light)
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_i2s_edac_pins = i2s_edac_pins;
    m_use_internal_dac = use_internal_dac;
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
    extern assistant_state current_state;
    extern bool activation_button_pressed;

    //Wake up Speech Assistant
    if(!activation_button_pressed){
        wakeup_assistant(ASSISTANT_ACTIVATION_BUTTON_PIN);
    } else {
        activation_button_pressed = false;
    }
    

    //start DAC Output (Mic Passthrough)
    if(m_use_internal_dac){
        Serial.println("Use internal DAC");
        m_dac_output->start(m_sample_provider);   
    } else {
        Serial.println("Use external DAC");
        m_dac_output->start(m_sample_provider, m_i2s_edac_pins);
    }
    
    vTaskDelay(200);

    Serial.println("Started Passthrough");

    //TODO: assistant not available/responding
    /*
    if(assistant_state!=listening){
    }

    */
    //wait until speech assistant exits listening state and react to button interrupt
    while(1){
        vTaskDelay(100);
    }
    while(current_state==listening){
        if(activation_button_pressed){
            Serial.println("Activation Button pressed");
            activation_button_pressed = false;
            //wakeup_assistant(ASSISTANT_ACTIVATION_BUTTON_PIN);
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