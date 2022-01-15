#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "WakeWordRecognizedState.h"
#include "IndicatorLight.h"
#include "../config.h"
#include <string.h>

WakeWordRecognizedState::WakeWordRecognizedState(I2SSampler *sample_provider, IndicatorLight *indicator_light )
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_indicator_light = indicator_light;

}
void WakeWordRecognizedState::enterState()
{
    return;
}

bool WakeWordRecognizedState::run()
{
    Serial.println("Marvin detected!");
    digitalWrite(23, HIGH);
    delay(200);
    digitalWrite(23, LOW);
    delay(100);
    
    //TODO
    //Arduino Kommunikation - Status abfragen

    return true;
}

void WakeWordRecognizedState::exitState()
{
    uint32_t free_ram = esp_get_free_heap_size();
    //Serial.printf("Free ram after request %d\n", free_ram);
}