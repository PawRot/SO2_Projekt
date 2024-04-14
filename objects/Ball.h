
#ifndef SO2_PROJEKT_BALL_H
#define SO2_PROJEKT_BALL_H

#include <atomic>
#include <iostream>

//TODO: modify this comment to describe the finished version of the class, add english version
// konstruktor przyjmuje znak z którego będzie składał się obiekt
// funkcja, która rysuje obiekt na ekranie / generuje jego kształt
// zmienne przechowująca położenie, prędkość, kierunek

class Ball {

    int bounces = 0;
    int x,y;
    int speed;

public:
    Ball();
    ~Ball();

    Ball(const Ball &ball);

    void runBall();

    int get_x() const { return x; }
    int get_y() const { return y; }








};


#endif //SO2_PROJEKT_BALL_H
