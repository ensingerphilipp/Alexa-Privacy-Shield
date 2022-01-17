#ifndef _recognise_command_state_h_
#define _recognise_command_state_h_

#include "States.h"

class I2SSampler;
class IndicatorLight;

class WakeWordRecognizedState : public State
{
private:
    I2SSampler *m_sample_provider;
    unsigned long m_start_time;
    unsigned long m_elapsed_time;
    int m_last_audio_position;

    IndicatorLight *m_indicator_light;

public:
    WakeWordRecognizedState(I2SSampler *sample_provider, IndicatorLight *indicator_light);
    void enterState();
    bool run();
    void exitState();
};

#endif
