#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "WakeWordRecognizedState.h"
#include "IndicatorLight.h"
#include "DACOutput.h"
#include "../config.h"
#include <string.h>

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
    Serial.println("Ready for Pasthrough");

    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram before start of Passthrough %d\n", free_ram);
    return;
}

bool WakeWordRecognizedState::run()
{   
    DACOutput output = DACOutput();
    output.start(m_sample_provider);
    //while not interrupted by arduino
    while(true){
        Serial.printf("Listening?\n");
        sleep(1000);       
    }
        
    return true;
}

void WakeWordRecognizedState::exitState()
{
    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram after request %d\n", free_ram);
}