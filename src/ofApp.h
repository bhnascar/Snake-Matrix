#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofSoundMixer.h"
#include "SnakeMatrix.h"

class ofApp : public ofBaseApp {
public:
    ofApp(float width, float height);
    ~ofApp();
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);

private:
    ofTrueTypeFont font;
    
    /* Some helper methods... */
    SMLocation mapLocationToSMLocation(int x, int y);
    ofColor colorForFoodState(SMState state);
    void precomputeDrawingVariables();
    
    /* Main game model. */
    SnakeMatrix *sm;
    
    /* Drawing variables. */
    float squareSize;
    float squarePadding;
    float startX;
    float startY;
    
    /* Images. */
    ofImage sinImage;
    ofImage triangleImage;
    ofImage squareImage;
    
    /* Mouse tracking variables. */
    SMLocation tapLocation;
    
    /* Window variables. */
    float windowWidth;
    float windowHeight;
    
    /* Timer variables. */
    float lastSnakeTick = 0.f;
    float lastBeatTick = 0.f;
    
    /* Update intervals in seconds. */
    float snakeTickInterval = 0.3f;
    float beatTickInterval = 0.1f;
    
    /* Audio manager. */
    ofSoundMixer* mixer = NULL;
};
