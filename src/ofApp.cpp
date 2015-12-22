#include "ofApp.h"

ofApp::ofApp(float width, float height)
: windowWidth(width), windowHeight(height) {
}

ofApp::~ofApp() {
    
}

//--------------------------------------------------------------
void ofApp::setup() {
    sm = new SnakeMatrix(8, 5);
    sm->Seed();
    precomputeDrawingVariables();
    
    mixer = new ofSoundMixer(this, 5);
    
    // Load font
    font.loadFont("Tahoma.ttf", 18, true, true);
}

void ofApp::precomputeDrawingVariables() {
    float numHorizontalSquares = sm->GetWidth();
    float numVerticalSquares = sm->GetHeight();
    float numHorizontalPads = numHorizontalSquares - 1;
    float numVerticalPads = numVerticalSquares - 1;
    
    squarePadding = 10;
    squareSize = min((windowWidth - numHorizontalPads * squarePadding) / numHorizontalSquares,
                     (windowHeight - numVerticalPads * squarePadding) / numVerticalSquares);
    float gridWidth = squareSize * numHorizontalSquares + squarePadding * numHorizontalPads;
    float gridHeight = squareSize * numVerticalSquares + squarePadding * numVerticalPads;
    startX = windowWidth / 2.f - gridWidth / 2.f;
    startY = windowHeight / 2.f - gridHeight / 2.f;
    
    sinImage.loadImage("sin.png");
    triangleImage.loadImage("triangle.png");
    squareImage.loadImage("square.png");
    
    sinImage.resize(squareSize * 0.8, squareSize * 0.8);
    triangleImage.resize(squareSize * 0.8, squareSize * 0.8);
    squareImage.resize(squareSize * 0.8, squareSize * 0.8);
}

//--------------------------------------------------------------
void ofApp::update() {
    float now = ofGetElapsedTimef();
    if (now - lastBeatTick > beatTickInterval) {
        sm->BeatTick();
        lastBeatTick = now;
    
        // Set audio mode.
        if (sm->GetFoodState() == SIN_FOOD) {
            mixer->SetMode(SIN_MODE);
        }
        else if (sm->GetFoodState() == TRIANGLE_FOOD) {
            mixer->SetMode(TRIANGLE_MODE);
        }
        else if (sm->GetFoodState() == SQUARE_FOOD) {
            mixer->SetMode(SQUARE_MODE);
        }
        else {
            mixer->SetMode(SIN_MODE);
        }
        
        // Play audio.
        int beat = sm->GetBeat();
        for (int y = 0; y < sm->GetHeight(); y++) {
            if (sm->isSnake(beat, y)) {
                mixer->Play(y);
            }
            else {
                mixer->Stop(y);
            }
        }
    }
    if (now - lastSnakeTick > snakeTickInterval) {
        sm->SnakeTick();
        lastSnakeTick = now;
    }
}

//--------------------------------------------------------------
ofColor ofApp::colorForFoodState(SMState state) {
    switch (state) {
        case SIN_FOOD:
            return ofColor(0, 200, 255, 255);
        case TRIANGLE_FOOD:
            return ofColor(230, 0, 230, 255);
        case SQUARE_FOOD:
            return ofColor(230, 0, 0, 255);
        case NONE:
        case SNAKE:
        case ON_BEAT:
        default:
            return ofColor(255, 255, 255, 255);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Hack to fix mouse disappearance bug.
    ofHideCursor();
    ofShowCursor();
    
    SMState foodState = sm->GetFoodState();
    ofClear(colorForFoodState(foodState) * 0.1);
    
    float numHorizontalSquares = sm->GetWidth();
    float numVerticalSquares = sm->GetHeight();
    for (int x = 0; x < numHorizontalSquares; x++) {
        for (int y = 0; y < numVerticalSquares; y++) {
            ofColor currentColor;
            
            // Set square color based on properties.
            if (x == sm->GetHeadPosition().x && y == sm->GetHeadPosition().y) {
                currentColor = ofColor(255, 255, 255, 255);
            }
            else if (sm->isSnake(x, y)) {
                if (foodState) {
                    currentColor = colorForFoodState(foodState);
                }
                else {
                    currentColor = ofColor(0, 230, 0, 255);
                }
            }
            else if (sm->isSinFood(x, y)) {
                currentColor = ofColor(0, 200, 255, 255);
            }
            else if (sm->isTriangleFood(x, y)) {
                currentColor = ofColor(230, 0, 230, 255);
            }
            else if (sm->isSquareFood(x, y)) {
                currentColor = ofColor(230, 0, 0, 255);
            }
            else {
                currentColor = colorForFoodState(foodState) * 0.2;
            }
            
            // Highlight active squares.
            if (sm->isOnBeat(x, y)) {
                currentColor *= 1.3f;
            }
            
            // Draw squares.
            ofSetColor(currentColor);
            ofRectRounded(startX + (squareSize + squarePadding) * x,
                          startY + (squareSize + squarePadding) * y,
                          squareSize, squareSize, 5);
            
            // Draw food labels
            ofSetColor(255, 255, 255, 255);
            float imageStartX = startX + (squareSize + squarePadding) * x + squareSize * 0.1;
            float imageStartY = startY + (squareSize + squarePadding) * y + squareSize * 0.1;
            if (sm->isSinFood(x, y)) {
                sinImage.draw(imageStartX, imageStartY);
            }
            else if (sm->isTriangleFood(x, y)) {
                triangleImage.draw(imageStartX, imageStartY);
            }
            else if (sm->isSquareFood(x, y)) {
                squareImage.draw(imageStartX, imageStartY);
            }
        }
    }
    
    // Print framerate.
    std::ostringstream buff;
    buff << ofGetFrameRate();;
    font.drawString(buff.str(), 10, 30);
}

//--------------------------------------------------------------
SMLocation ofApp::mapLocationToSMLocation(int x, int y) {
    SMLocation loc;
    x -= startX;
    y -= startY;
    loc.x = x / (squareSize + squarePadding);
    loc.y = y / (squareSize + squarePadding);
    return loc;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch(key) {
        case OF_KEY_LEFT:
            sm->SetDirection(LEFT);
            break;
        case OF_KEY_RIGHT:
            sm->SetDirection(RIGHT);
            break;
        case OF_KEY_UP:
            sm->SetDirection(UP);
            break;
        case OF_KEY_DOWN:
            sm->SetDirection(DOWN);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    tapLocation = mapLocationToSMLocation(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    SMLocation releaseLocation = mapLocationToSMLocation(x, y);
    if (tapLocation == releaseLocation) {
        sm->Tap(tapLocation.x, tapLocation.y);
    }
    tapLocation.x = -1;
    tapLocation.y = -1;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    precomputeDrawingVariables();
}

//--------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    if (mixer) {
        mixer->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
    }
}
