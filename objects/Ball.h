
#ifndef SO2_PROJEKT_BALL_H
#define SO2_PROJEKT_BALL_H

#include <atomic>
#include <iostream>

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





    // TODO - zrobić test: czy po wywołaniu run_ball() w wątku zmienia się x i y w obiekcie Ball



};


#endif //SO2_PROJEKT_BALL_H
