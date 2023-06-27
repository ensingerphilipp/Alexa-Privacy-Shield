
#include <Arduino.h>
#include "driver/i2s.h"
#include <driver/dac.h>
#include <math.h>
#include <SPIFFS.h>
#include <FS.h>

#include "../audio_input/I2SSampler.h"
#include "../audio_input/I2SMicSampler.h"
#include "DACOutput.h"

// number of frames to try and send at once (a frame is a left and right sample)
// needs to be > 512 frames to avoid hiccups in audio
#define NUM_FRAMES_TO_SEND 1024

typedef struct
{
    uint16_t left;
    uint16_t right;
} Frame_t;

typedef struct
{
    int16_t left;
    int16_t right;
} FrameInt16_t;

void i2sWriterTask(void *param)
{
    DACOutput *output = (DACOutput *)param;
    int availableBytes = 0;
    int buffer_position = 0;
    Frame_t sample_buffer[NUM_FRAMES_TO_SEND];

    while (true)
    {
        // wait for some data to be requested
        i2s_event_t evt;
        if (xQueueReceive(output->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                size_t bytesWritten = 0;
                do
                {
                    if (availableBytes == 0)
                    {
                        
                        if (output->m_last_audio_position == -1)
                        {
                            // set to 1 seconds in the past the allow for state chagne latency
                            //output->m_last_audio_position = output->m_sample_provider->getCurrentWritePosition() - 16000;
                            output->m_last_audio_position = output->m_sample_provider->getCurrentWritePosition() - 16000;
                        }
                        // how many samples have been captured since we last ran
                        int audio_position = output->m_sample_provider->getCurrentWritePosition();
                        // work out how many samples there are taking into account that we can wrap around
                        int sample_count = (audio_position - output->m_last_audio_position + output->m_sample_provider->getRingBufferSize()) % output->m_sample_provider->getRingBufferSize();
                        //Serial.printf("Last sample position %d, current position %d, number samples %d\n", output->m_last_audio_position, audio_position, sample_count);
                        if (sample_count > 0)
                        {   
                            //Serial.printf("Reading Ringbuffer");
                            //read ringbuffer and send samples
                            RingBufferAccessor *reader = output->m_sample_provider->getRingBufferReader();
                            reader->setIndex(output->m_last_audio_position);
                            // send the samples up in chunks
                            
                            while (sample_count > 0)
                            {
                                for (int i = 0; i < sample_count && i < NUM_FRAMES_TO_SEND; i++)
                                {   
                                    //get sample and set left and right channel equal because only mono mic is used
                                    int16_t left = (reader->getCurrentSample() + 32768);
                                    sample_buffer[i].left = left;
                                    sample_buffer[i].right = left; 
                                    reader->moveToNextSample();
                                }
                                //Serial.printf("\n");
                                sample_count -= NUM_FRAMES_TO_SEND;
                            }
                            output->m_last_audio_position = reader->getIndex();
                            delete (reader);
                        }

                        // how many bytes do we now have to send
                        availableBytes = NUM_FRAMES_TO_SEND * sizeof(int16_t);
                        // reset the buffer position back to the start
                        buffer_position = 0;
                    }
                    // do we have something to write?
                    if (availableBytes > 0)
                    {
                        // write data to the i2s peripheral
                        //Serial.printf("I2S Writing Data to Speaker!\n");
                        
                        i2s_write(I2S_NUM_0, buffer_position + (uint8_t *) sample_buffer,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                        //Serial.printf("ab: %d bw: %d\n", availableBytes, bytesWritten);
                    }
                } while (bytesWritten > 0);
            }
        }
    }
}

void i2sEdacWriterTask(void *param)
{
    DACOutput *output = (DACOutput *)param;
    int availableBytes = 0;
    int buffer_position = 0;
    FrameInt16_t sample_buffer[NUM_FRAMES_TO_SEND];
    printf("EdacWriter");
    while (true)
    {
        // wait for some data to be requested
        i2s_event_t evt;
        if (xQueueReceive(output->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                size_t bytesWritten = 0;
                do
                {
                    if (availableBytes == 0)
                    {
                        
                        if (output->m_last_audio_position == -1)
                        {
                            // set to 1 seconds in the past the allow for state chagne latency
                            //output->m_last_audio_position = output->m_sample_provider->getCurrentWritePosition() - 16000;
                            output->m_last_audio_position = output->m_sample_provider->getCurrentWritePosition() - 16000;
                        }
                        // how many samples have been captured since we last ran
                        int audio_position = output->m_sample_provider->getCurrentWritePosition();
                        // work out how many samples there are taking into account that we can wrap around
                        int sample_count = (audio_position - output->m_last_audio_position + output->m_sample_provider->getRingBufferSize()) % output->m_sample_provider->getRingBufferSize();
                        //Serial.printf("Last sample position %d, current position %d, number samples %d\n", output->m_last_audio_position, audio_position, sample_count);
                        if (sample_count > 0)
                        {   
                            //Serial.printf("Reading Ringbuffer");
                            //read ringbuffer and send samples
                            RingBufferAccessor *reader = output->m_sample_provider->getRingBufferReader();
                            reader->setIndex(output->m_last_audio_position);
                            // send the samples up in chunks
                            
                            while (sample_count > 0)
                            {
                                for (int i = 0; i < sample_count && i < NUM_FRAMES_TO_SEND; i++)
                                {   
                                    //get sample and set left and right channel equal because only mono mic is used
                                    int16_t left = (reader->getCurrentSample());
                                    sample_buffer[i].left = left;
                                    sample_buffer[i].right = left; 
                                    //printf("%d\n", left);
                                    //printf("%d\n", sample_buffer[i].left);
                                    reader->moveToNextSample();
                                }
                                //Serial.printf("\n");
                                sample_count -= NUM_FRAMES_TO_SEND;
                            }
                            output->m_last_audio_position = reader->getIndex();
                            delete (reader);
                        }

                        // how many bytes do we now have to send
                        availableBytes = NUM_FRAMES_TO_SEND * sizeof(int16_t);
                        // reset the buffer position back to the start
                        buffer_position = 0;
                    }
                    // do we have something to write?
                    if (availableBytes > 0)
                    {
                        // write data to the i2s peripheral
                        //Serial.printf("I2S Writing Data to Speaker!\n");
                        
                        i2s_write(I2S_NUM_0, buffer_position + (int16_t *) sample_buffer,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                        //Serial.printf("ab: %d bw: %d\n", availableBytes, bytesWritten);
                    }
                } while (bytesWritten > 0);
            }
        }
    }
}

DACOutput::DACOutput(){
}

//Start Method using internal_dac
void DACOutput::start(I2SSampler *sample_provider)
{
    m_sample_provider = sample_provider;
    m_last_audio_position = -1;
    // i2s config for writing both channels of I2S internal DAC
    i2s_config_t i2sConfig_internal_DAC = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 64};

    //install and start i2s driver for internal DAC
    i2s_driver_install(I2S_NUM_0, &i2sConfig_internal_DAC, 4, &m_i2sQueue);
    // enable the DAC channels
    i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
    // clear the DMA buffers
    i2s_zero_dma_buffer(I2S_NUM_0);
    
    TaskHandle_t writerTaskHandle = NULL;
    // start a task to write samples to the i2s peripheral --> 8192 Samples? YUP with less it will create irritations in audio 
    xTaskCreate(i2sWriterTask, "i2s Writer Task", 8192, this, 1, &writerTaskHandle);
    m_i2sWriterTaskHandle = writerTaskHandle;  
}

//start method using external_dac
void DACOutput::start(I2SSampler *sample_provider, i2s_pin_config_t *i2s_edac_pins)
{
    m_sample_provider = sample_provider;
    m_last_audio_position = -1;

    // i2s config for writing both channels of I2S external DAC 
    i2s_config_t i2sConfig_external_DAC = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 512};

    //Install and Start i2s driver for external DAC
    i2s_driver_install(I2S_NUM_0, &i2sConfig_external_DAC, 4, &m_i2sQueue);
    //Need both channels for correct mixing in MAX 98537A
    //i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
    // set up the i2s pins
    i2s_set_pin(I2S_NUM_0, i2s_edac_pins);
    //clear dma buffers
    i2s_zero_dma_buffer(I2S_NUM_0);

    
    TaskHandle_t writerTaskHandle = NULL;
    // start a task to write samples to the i2s peripheral --> 8192 Samples? YUP with less it will create irritations in audio 
    xTaskCreate(i2sEdacWriterTask, "i2s Writer Task", 8192, this, 1, &writerTaskHandle);
    m_i2sWriterTaskHandle = writerTaskHandle;  
}

void DACOutput::stop()
{   
    vTaskDelete(m_i2sWriterTaskHandle);
    i2s_driver_uninstall(I2S_NUM_0);
    dac_output_disable(DAC_CHANNEL_2);
}