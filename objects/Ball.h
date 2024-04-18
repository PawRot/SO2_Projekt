
#ifndef SO2_PROJEKT_BALL_H
#define SO2_PROJEKT_BALL_H

#include <atomic>
#include <thread>
#include <random>




class Ball {

    const int MAX_BOUNCES = 5;
    const int MAX_SPEED = 5;

    std::vector<bool> *colors; // pointer to vector that stores which colors are in use



    int max_y, min_y; // maximum and minimum y coordinate of the ball
    int max_x, min_x; // maximum and minimum x coordinate of the ball
    int bounces = 0;
    int speed;

    int horizontalDirection; // 1 - right, -1 - left, 0 - no movement
    int verticalDirection = -1; // 1 - down, -1 - up, 0 - no movement

    std::thread *ballThread; // thread that runs the ball

    std::atomic_bool *stopFlag; // flag that indicates that the ball should stop

    int generateSpeed(); // function that generates speed of the ball
    void runBall(); // function that runs the ball




public:

    int x,y; // x and y coordinates of the ball
    int color; // color of the ball

    bool finished = false; // flag that indicates that the ball finished bouncing


    Ball(int windowWidth, int windowHeight, std::atomic_bool *stopFlag, std::vector<bool> *colors);
    ~Ball();









};


#endif //SO2_PROJEKT_BALL_H
