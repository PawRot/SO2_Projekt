
#include "Ball.h"

Ball::Ball() {
    std::cout << "I am being constructed" << std::endl;
    x = 0;
    y = 0;
    speed = 5;
}

Ball::~Ball() {

}

Ball::Ball(const Ball& ball) {
    std::cout << "I am being copied" << std::endl;
    x = 0;
    y = 0;
    speed = 5;
}

void Ball::runBall() {
    x = 2;
    y = x + 1;







    if (bounces == 5 ) {
        delete this;
    }
}
