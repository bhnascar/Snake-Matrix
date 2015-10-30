#pragma once

#include "ofMain.h"

/* Sound modes. */
typedef enum {
    SIN_MODE = 0,
    TRIANGLE_MODE,
    SQUARE_MODE,
    SAW_MODE,
} SMSoundMode;

/* Struct to wrap properties of a sound device. */
struct SMSoundProperties {
    float volume;
    float freq;
};

class ofSoundMixer {
public:
    ofSoundMixer(ofBaseApp* app, int numSources);
    ~ofSoundMixer();
    
    /* Ping/play/pause. Ping means the sound will be played and then
     * faded out, like a bell. */
    void Ping(int source, float duration);
    void Play(int source);
    void Stop(int source);
    
    /* Sets the timbre. */
    void SetMode(SMSoundMode mode);
    
    /* RtAudio callback. */
    void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
    
private:
    float SampleSignal(int sourceID, int tick);
    
    ofMutex mutex;
    SMSoundMode mode = SIN_MODE;
    ofSoundStream stream;
    std::vector<SMSoundProperties> sourceProperties;
};

