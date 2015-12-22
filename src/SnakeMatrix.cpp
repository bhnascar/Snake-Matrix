#include "SnakeMatrix.h"

bool operator==(SMLocation& one, SMLocation& two) {
    return one.x == two.x && one.y == two.y;
}

bool operator!=(SMLocation& one, SMLocation& two) {
    return !(one == two);
}

SnakeMatrix::SnakeMatrix(int width, int height)
: width(width), height(height) {
    matrix = new int[width * height];
    for (int i = 0; i < width * height; i++) {
        matrix[i] = NONE;
    }
    foodState = NONE;
}

SnakeMatrix::~SnakeMatrix() {
    delete [] matrix;
}

void SnakeMatrix::Tap(int x, int y) {
    int state = GetState(x, y);
    
    // Cycle between:
    // NONE -> SIN -> TRIANGLE -> SQUARE
    if (!state) {
        AddState(SIN_FOOD, x, y);
    }
    else if (state & SIN_FOOD) {
        RemoveState(SIN_FOOD, x, y);
        AddState(TRIANGLE_FOOD, x, y);
    }
    else if (state & TRIANGLE_FOOD) {
        RemoveState(TRIANGLE_FOOD, x, y);
        AddState(SQUARE_FOOD, x, y);
    }
    else if (state & SQUARE_FOOD) {
        RemoveState(SQUARE_FOOD, x, y);
    }
}

bool SnakeMatrix::isOnBeat(int x, int y) {
    return GetState(x, y) & ON_BEAT;
}

bool SnakeMatrix::isSnake(int x, int y) {
    return GetState(x, y) & SNAKE;
}

bool SnakeMatrix::isFood(int x, int y) {
    return GetState(x, y) & (TRIANGLE_FOOD | SIN_FOOD | SQUARE_FOOD);
}

bool SnakeMatrix::isSinFood(int x, int y) {
    return GetState(x, y) & SIN_FOOD;
}

bool SnakeMatrix::isTriangleFood(int x, int y) {
    return GetState(x, y) & TRIANGLE_FOOD;
}

bool SnakeMatrix::isSquareFood(int x, int y) {
    return GetState(x, y) & SQUARE_FOOD;
}

void SnakeMatrix::AddState(SMState state, SMLocation location) {
    matrix[location.y * width + location.x] |= state;
}

void SnakeMatrix::AddState(SMState state, int x, int y) {
    matrix[y * width + x] |= state;
}

void SnakeMatrix::RemoveState(SMState state, SMLocation location) {
    matrix[location.y * width + location.x] &= ~state;
}

void SnakeMatrix::RemoveState(SMState state, int x, int y) {
    matrix[y * width + x] &= ~state;
}

SMLocation SnakeMatrix::GetHeadPosition() {
    return head;
}

SMState SnakeMatrix::GetFoodState() {
    return foodState;
}

int SnakeMatrix::GetState(int x, int y) {
    return matrix[y * width + x];
}

int SnakeMatrix::GetBeat() {
    return beat;
}

void SnakeMatrix::Seed() {
    srand(time(NULL));
    direction = (SMDirection)(((double)rand() / RAND_MAX) * 4);
    head.x = ((double)rand() / RAND_MAX) * width;
    head.y = ((double)rand() / RAND_MAX) * height;
}

void SnakeMatrix::SetDirection(SMDirection dir) {
    direction = dir;
}

void SnakeMatrix::BeatTick() {
    beat = (beat == width - 1) ? 0 : beat + 1;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x == beat) {
                AddState(ON_BEAT, x, y);
            }
            else {
                RemoveState(ON_BEAT, x, y);
            }
        }
    }
}

void SnakeMatrix::SnakeTick() {
    // Update head position.
    if (direction == UP) {
        head.y = (head.y == 0) ? height - 1 : head.y - 1;
    }
    else if (direction == RIGHT) {
        head.x = (head.x == width - 1) ? 0 : head.x + 1;
    }
    else if (direction == DOWN) {
        head.y = (head.y == height - 1) ? 0 : head.y + 1;
    }
    else if (direction == LEFT) {
        head.x = (head.x == 0) ? width - 1 : head.x - 1;
    }
    
    // Remove tail piece if snake didn't eat anything.
    // (Otherwise not removing the tail piece allows the
    // snake to grow while continuing to move forward.)
    if (!isFood(head.x, head.y) && snake.size() > 0) {
        snake.pop_front();
    }
    
    // Remove tail chunk of snake when it crosses itself.
    if (isSnake(head.x, head.y)) {
        while (snake.size() > 0 && snake.front() != head) {
            snake.pop_front();
        }
    }
    
    // Add new head.
    snake.push_back(SMLocation(head));
    
    // Update food state.
    if (isFood(head.x, head.y)) {
        foodState = (SMState)(GetState(head.x, head.y) & (SIN_FOOD | TRIANGLE_FOOD | SQUARE_FOOD));
    }
    
    // Update snake states.
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            RemoveState(SNAKE, x, y);
        }
    }
    for (std::list<SMLocation>::iterator it = snake.begin(); it != snake.end(); it++) {
        AddState(SNAKE, *it);
    }
}