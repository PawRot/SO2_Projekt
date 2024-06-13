#include "Ball.h"

Ball::Ball(int windowWidth, int windowHeight, std::atomic_bool* stopFlag, std::vector<bool>* colors, Rectangle* rectanglePtr, std::queue<Ball *>* waitingBalls) {
    this->stopFlag = stopFlag;
    this->colors = colors;
    this->rectanglePtr = rectanglePtr;
    this->waitingBalls = waitingBalls;

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
    if (waitingBalls->front() == this) {
        waitingBalls->pop();
    }
    colors->at(color) = false;
    ballThread->join();
    delete ballThread;
}

void Ball::runBall() {
    while (*stopFlag != true && bounces < MAX_BOUNCES) {

        if (bouncedFromRectangle && waitingInQueue) {
            if (waitingBalls->front() == this) {
                waitingInQueue = false;
            } else {
                auto stopWaiting = false;
                while (!stopWaiting) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    if (waitingBalls->front() == this) {
                        waitingInQueue = false;
                        stopWaiting = true;
                    }
                }
            }
        }

        std::unique_lock lock(rectanglePtr->mtx);

        const int rectX = rectanglePtr->getX();
        const int rectWidth = rectanglePtr->getWidth();

        // Check if the ball's x-coordinate is within the range
        if (x >= (rectX - 10) && x <= (rectX + rectWidth + 10)) {
            // The ball's x-coordinate is within the range

            // Predict the ball's next position
            const int nextX = x + horizontalDirection;
            const int nextY = y + verticalDirection;

            // Get the rectangle's position and size
            const int rectY = rectanglePtr->getY();
            const int rectHeight = rectanglePtr->getHeight();

            // Check if the next position is within the rectangle's boundaries
            if (nextX >= rectX && nextX <= rectX + rectWidth && nextY >= rectY && nextY <= rectY + rectHeight) {
                // The ball's next position will intersect with the rectangle
                insideCounter++;
                if (insideCounter > 2 && !bouncedFromRectangle) {
                    lock.unlock();
                    bounces = MAX_BOUNCES;
                    finished = true;
                    break;
                }

                if (!justBounced) {
                    if (!bouncedFromRectangle) {
                        bouncedFromRectangle = true;
                        waitingInQueue = true;
                        waitingBalls->push(this);
                    }


                    // Reverse the ball's direction
                    horizontalDirection = -horizontalDirection;
                    verticalDirection = -verticalDirection;


                    const auto rectVertDirection = rectanglePtr->getVerticalDirection();
                    if (verticalDirection == 0 && x >= rectX && x <= rectX + rectWidth) {
                        verticalDirection = rectVertDirection;
                        y = y + 1 * verticalDirection;
                    }

                    const auto rectSpeed = rectanglePtr->getSpeed();
                    const auto rectSleep = rectanglePtr->getBaseSleep();
                    if (verticalDirection == rectVertDirection && (rectSleep / rectSpeed) < (base_sleep / speed)) {
                        speed = (rectSpeed > 1) ? rectSpeed - 1 : rectSpeed;
                        base_sleep = (rectSpeed == 1) ? rectSleep - 15 : rectSleep;
                    }
                    justBounced = true;
                }
            } else {
                insideCounter = 0;
                justBounced = false;
            }

        } else lock.unlock();

        y = y + 1 * verticalDirection;
        x = x + 1 * horizontalDirection;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> chance(0, 100);

        if (y >= max_y || y <= min_y) {
            if (bouncedFromRectangle) {
                bouncedFromRectangle = false;
                waitingBalls->pop();
                // notify here
            }
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
            if (bouncedFromRectangle) {
                bouncedFromRectangle = false;
                waitingBalls->pop();
                // notify here
            }

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
            if (waitingBalls->front() == this) {
                waitingBalls->pop();
            }
            finished = true;
        }


        if (lock.owns_lock()) lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(base_sleep / speed));

    }
}

int Ball::generateSpeed() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_SPEED);
    return dis(gen);
}
