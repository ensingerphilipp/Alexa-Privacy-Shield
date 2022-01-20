#include <Arduino.h>
#include "driver/i2s.h"

class SampleSource;

/**
 * Base Class for both the ADC and I2S sampler
 **/
class DACOutput
{
private:
    // I2S write task
    TaskHandle_t m_i2sWriterTaskHandle;
    // i2s writer queue
    QueueHandle_t m_i2sQueue;
    // src of samples for us to play
    I2SSampler *m_sample_provider;
    int m_last_audio_position;
    
public:
    DACOutput();
    void start(I2SSampler *sample_provider);
    void stop();

    friend void i2sWriterTask(void *param);
};