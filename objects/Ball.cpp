
#include "Ball.h"

Ball::Ball(int windowWidth, int windowHeight, std::atomic_bool *stopFlag) {
    this->stopFlag = stopFlag;

    min_y = 0 + 1;
    max_y = windowHeight - 1;
    min_x = 0 + 1;
    max_x = windowWidth - 1;
    speed = generateSpeed();

    y = max_y;

    std::random_device rd;
    std::mt19937 gen(rd());

    x = [&windowWidth, &rd, &gen] {
        const int width = windowWidth / 3;
        std::uniform_int_distribution<> distr(width, 2*width);
        return distr(gen);

    }();

    std::uniform_int_distribution<> distr(-1, 1);
    horizontalDirection = distr(gen);


    this->ballThread = new std::thread(&Ball::runBall, this);

}

Ball::~Ball() {
    ballThread->join();
}

void Ball::runBall() {
    while (*stopFlag != true && bounces < MAX_BOUNCES) {
        y = y + 1 * verticalDirection;
        x = x + 1 * horizontalDirection;
        std::this_thread::sleep_for(std::chrono::milliseconds(100 / speed));
        if (y >= max_y || y <= min_y) {
            if (y >= max_y) {
                y = max_y;
            } else {
                y = min_y;
            }
            verticalDirection = -verticalDirection;
            bounces++;
        }

        if (x >= max_x || x <= min_x) {
            if (x >= max_x) {
                x = max_x;
            } else {
                x = min_x;
            }
            horizontalDirection = -horizontalDirection;
            bounces++;
        }

        if (bounces >= MAX_BOUNCES) {
            finished = true;
        }
    }
}

int Ball::generateSpeed() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_SPEED);
    return dis(gen);
}
