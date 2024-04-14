
#ifndef SO2_PROJEKT_BALL_H
#define SO2_PROJEKT_BALL_H

#include <atomic>
#include <thread>
#include <random>


//TODO: modify this comment to describe the finished version of the class, add english version
// konstruktor przyjmuje znak z którego będzie składał się obiekt
// funkcja, która rysuje obiekt na ekranie / generuje jego kształt
// zmienne przechowująca położenie, prędkość, kierunek

class Ball {

    const int MAX_BOUNCES = 5;
    const int MAX_SPEED = 5;



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

    bool finished = false; // flag that indicates that the ball finished bouncing


    Ball(int windowWidth, int windowHeight, std::atomic_bool *stopFlag);
    ~Ball();









};


#endif //SO2_PROJEKT_BALL_H
