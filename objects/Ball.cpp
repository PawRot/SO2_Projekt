#include "Ball.h"

Ball::Ball(int windowWidth, int windowHeight, std::atomic_bool* stopFlag, std::vector<bool>* colors, Rectangle* rectanglePtr) {
    this->stopFlag = stopFlag;
    this->colors = colors;
    this->rectanglePtr = rectanglePtr;

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
        std::unique_lock lock(rectanglePtr->mtx);

        const int rectX = rectanglePtr->getX();
        const int rectWidth = rectanglePtr->getWidth();



        // for collisions with the rectangle, we only check the field in the direction in which the ball is moving
        // for example if the ball is moving up, we only check the field above the ball
        // this is because the ball moves in a straight line and we can assume that the ball will not collide with the rectangle if it is not in the same row or column
        // we also need to check collisions when the ball is moving diagonally
        // in this case we need to check the fields in the direction of the ball's movement both vertically and horizontally and the diagonal field
        // this is because the ball can collide with the rectangle if it is not in the same row or column
        // but it is in the diagonal field
        // in summary, the possible options are as follows:
        // 1. the ball is moving horizontally to the right
        // 2. the ball is moving horizontally to the left
        // 3. the ball is moving vertically up
        // 4. the ball is moving vertically down
        // 5. the ball is moving diagonally up and to the right
        // 6. the ball is moving diagonally up and to the left
        // 7. the ball is moving diagonally down and to the right
        // 8. the ball is moving diagonally down and to the left


        // check if the ball x coordinate is in the range of the rectangle x coordinates + 10
        // Get the rectangle's x-coordinate and width



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
            if (nextX >= rectX && nextX <= rectX + rectWidth && nextY >= rectY && nextY <= rectY + rectHeight && !justBounced) {

                // The ball's next position will intersect with the rectangle

                // Reverse the ball's direction
                horizontalDirection = -horizontalDirection;
                verticalDirection = -verticalDirection;

                const auto rectVertDirection = rectanglePtr->getVerticalDirection();
                if (verticalDirection == 0) {
                    verticalDirection = rectVertDirection;
                }

                const auto rectSpeed = rectanglePtr->getSpeed();
                const auto rectSleep = rectanglePtr->getBaseSleep();
                if (verticalDirection == rectVertDirection && (rectSleep / rectSpeed) < (base_sleep / speed)) {
                    speed = (rectSpeed > 1) ? rectSpeed - 1 : rectSpeed;
                    base_sleep = (rectSpeed == 1) ? rectSleep - 10 : rectSleep;
                }
            } else {
                justBounced = false;
            }

        }

        y = y + 1 * verticalDirection;
        x = x + 1 * horizontalDirection;
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
