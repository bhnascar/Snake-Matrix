#pragma once

#include <list>

/* Possible states of each square in the tone matrix. */
typedef enum {
    NONE = 0,
    SNAKE = (0x1 << 1),
    ON_BEAT = (0x1 << 2),
    SIN_FOOD = (0x1 << 3),
    SQUARE_FOOD = (0x1 << 4),
    TRIANGLE_FOOD = (0x1 << 5),
} SMState;

/* Possible directions of travel for the snake. */
typedef enum {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
} SMDirection;

/* A location in the tone matrix. */
struct SMLocation {
    int x;
    int y;
};

bool operator==(SMLocation& one, SMLocation& two);
bool operator!=(SMLocation& one, SMLocation& two);

/* This class combines a tone matrix with a snake game. It maintains
 * state data for both types of application. */
class SnakeMatrix
{
public:
    SnakeMatrix(int width = 8, int height = 5);
    ~SnakeMatrix();
    
    /* Returns the dimensions of this matrix in number of squares. */
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    
    /* Tap the square at location (x, y) on the board. */
    void Tap(int x, int y);
    
    /* Boolean methods for querying state of square at (x, y). */
    bool isOnBeat(int x, int y);
    bool isSnake(int x, int y);
    bool isFood(int x, int y);
    bool isSinFood(int x, int y);
    bool isSquareFood(int x, int y);
    bool isTriangleFood(int x, int y);
    
    /* Returns the food state of the snake, i.e. the last type of food
     * it consumed. */
    SMState GetFoodState();
    
    /* Returns the state of the square at location (x, y) on the board. */
    int GetState(int x, int y);
    
    /* Returns the current beat. */
    int GetBeat();
    
    /* Creates a random starting configuration. */
    void Seed();
    
    /* Sets the snake's direction of movement. */
    void SetDirection(SMDirection dir);
    
    /* Updates the snake and tone matrix states. */
    void SnakeTick();
    
    /* Updates beat information. */
    void BeatTick();
    
private:
    int width;
    int height;
    int* matrix;
    
    /* Adds the given state to the given location. */
    void AddState(SMState state, SMLocation location);
    void AddState(SMState state, int x, int y);
    
    /* Removes the given state from the given location. */
    void RemoveState(SMState state, SMLocation location);
    void RemoveState(SMState state, int x, int y);
    
    /* Snake represented as doubly-linked list. */
    std::list<SMLocation> snake;
    SMState foodState;
    SMLocation head;
    SMDirection direction;
    
    /* Beat position. */
    int beat = 0;
};

