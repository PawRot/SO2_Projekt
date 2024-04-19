#include "Ball.h"

Ball::Ball(int windowWidth, int windowHeight, std::atomic_bool* stopFlag, std::vector<bool>* colors) {
    this->stopFlag = stopFlag;
    this->colors = colors;

    min_y = 0 + 1;
    max_y = windowHeight;
    min_x = 0 + 1;
    max_x = windowWidth;
    speed = generateSpeed();

    y = max_y;

    std::random_device rd;
    std::mt19937 gen(rd());

    do {
        std::uniform_int_distribution<> dist(0, 255);
        color = dist(gen);
    }
    while (colors->at(color)); // check if color is already in use

    colors->at(color) = true;

    x = [&windowWidth, &rd, &gen] {
        const int width = windowWidth / 3;
        std::uniform_int_distribution<> distr(width, 2 * width);
        return distr(gen);
    }();

    std::uniform_int_distribution<> distr(-1, 1);
    horizontalDirection = distr(gen);


    this->ballThread = new std::thread(&Ball::runBall, this);
}

Ball::~Ball() {
    colors->at(color) = false;
    ballThread->join();
    delete ballThread;
}

void Ball::runBall() {
    while (*stopFlag != true && bounces < MAX_BOUNCES) {
        y = y + 1 * verticalDirection;
        x = x + 1 * horizontalDirection;
        std::this_thread::sleep_for(std::chrono::milliseconds(100 / speed));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> chance(0, 100);

        if (y >= max_y || y <= min_y) {
            if (y >= max_y) {
                y = max_y;
            }
            else {
                y = min_y;
            }

            if (chance(gen) < NON_STANDARD_BOUNCE_CHANCE) {
                if (horizontalDirection == 0) {
                    if (chance(gen) < 50) {
                        horizontalDirection = 1;
                    }
                    else {
                        horizontalDirection = -1;
                    }
                }
                else {
                    horizontalDirection = 0;
                }
            }


            verticalDirection = -verticalDirection;
            bounces++;
        }

        if (x >= max_x || x <= min_x) {
            if (x >= max_x) {
                x = max_x;
            }
            else {
                x = min_x;
            }


            if (chance(gen) < NON_STANDARD_BOUNCE_CHANCE) {
                if (verticalDirection == 0) {
                    if (chance(gen) < 50) {
                        verticalDirection = 1;
                    }
                    else {
                        verticalDirection = -1;
                    }
                }
                else {
                    verticalDirection = 0;
                }
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
