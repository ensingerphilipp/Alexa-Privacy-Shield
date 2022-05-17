#include <Arduino.h>
#include <driver/i2s.h>
#include "Application.h"
#include "state_machine/DetectWakeWordState.h"
#include "state_machine/WakeWordRecognizedState.h"
#include "IndicatorLight.h"

Application::Application(I2SSampler *sample_provider, i2s_pin_config_t *i2s_edac_pins, bool use_internal_dac, IndicatorLight *indicator_light)
{
    // detect wake word state - waits for the wake word to be detected
    m_detect_wake_word_state = new DetectWakeWordState(sample_provider);
    // command recongiser - streams audio to the server for recognition
    m_wakeword_recognized_state = new WakeWordRecognizedState(sample_provider, i2s_edac_pins, use_internal_dac, indicator_light);
    // start off in the detecting wakeword state
    //m_current_state = m_wakeword_recognized_state;
    m_current_state = m_detect_wake_word_state;
    m_current_state->enterState();
}

// process the next batch of samples
void Application::run()
{
    bool state_done = m_current_state->run();
    if (state_done)
    {
        m_current_state->exitState();
        // switch to the next state - very simple state machine so we just go to the other state...
        if (m_current_state == m_detect_wake_word_state)
        {
            m_current_state = m_wakeword_recognized_state;
        }
        else
        {
            m_current_state = m_detect_wake_word_state;
        }
        m_current_state->enterState();
    }
    vTaskDelay(10);
}
