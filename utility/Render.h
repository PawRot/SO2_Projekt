#ifndef SO2_PROJEKT_RENDER_H
#define SO2_PROJEKT_RENDER_H
#include <vector>
#include <thread>
#include <ncurses.h>
#include "../objects/Ball.h"
#include "../objects/Rectangle.h"


class Render {
    std::vector<Ball *> balls{}; // stores pointers to ball objects
    Rectangle* rectangle{}; // stores pointer to rectangle object


    std::thread* ballSpawnThread{};

    const int RENDER_SLEEP_TIME = 16666; // time in microseconds between each render


    int width, height; // width - number of columns, height - number of rows

    void spawnBall(); // function that spawns a ball, runs in a separate thread

    void drawBorder(); // function that draws the border
    void drawRectangle(); // function that draws the rectangle
    void drawBalls(); // function that draws the balls


    void stop(); // function that stops the render and deletes the object
    void draw(); // function that draws the objects


    bool checkKey(); // function that checks if key was pressed

    int number = 0;

public:
    std::vector<bool> colors; // vector that stores which colors are in use
    std::mutex mtx;

    std::atomic_bool stopFlag; // flag that indicates that threads should stop
    std::atomic_bool keyPressed = false; // flag that indicates that key was pressed
    std::atomic_bool* threadsStoppedPtr; // pointer to a flag that indicates that threads stopped

    Render(std::atomic_bool* threadsStoppedPtr);

    ~Render();

    void runRender(); // function that runs the render
};


#endif //SO2_PROJEKT_RENDER_H
