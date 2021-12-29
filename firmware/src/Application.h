#ifndef _application_h_
#define _applicaiton_h_

#include "state_machine/States.h"

class I2SSampler;
class State;
class IndicatorLight;

class Application
{
private:
    State *m_detect_wake_word_state;
    State *m_wakeword_recognized_state;
    State *m_current_state;

public:
    Application(I2SSampler *sample_provider, IndicatorLight *indicator_light);
    ~Application();
    void run();
};

#endif