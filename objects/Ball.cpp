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

        int rectX = rectanglePtr->x;
        int rectWidth = rectanglePtr->width;

        // Check if the ball's x-coordinate is within the range
        // if (x >= (rectX - 10) && x <= (rectX + rectWidth + 10)) lock.unlock();
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

        // std::unique_lock lock(rectanglePtr->mtx);

        // int rectX = rectanglePtr->x;
        // int rectWidth = rectanglePtr->width;

        // Check if the ball's x-coordinate is within the range
        if (x >= (rectX - 10) && x <= (rectX + rectWidth + 10)) {
            // The ball's x-coordinate is within the range


            // possible mutex here, shared with rectangle
            auto topEdgeCoordinates = rectanglePtr->topEdgeCoordinates;
            auto bottomEdgeCoordinates = rectanglePtr->bottomEdgeCoordinates;
            auto leftEdgeCoordinates = rectanglePtr->leftEdgeCoordinates;
            auto rightEdgeCoordinates = rectanglePtr->rightEdgeCoordinates;

            // also check if the ball is in the rectangle's edge
            if (horizontalDirection == 1 && verticalDirection == 0) { // right
                for (auto& edge : leftEdgeCoordinates) {
                    if ((x + 1 == std::get<0>(edge) || x == std::get<0>(edge)) && y == std::get<1>(edge)) {
                        horizontalDirection = -horizontalDirection;
                        break;
                    }
                }

                for (auto& edge : topEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y + 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        if (rectanglePtr->getVerticalDirection() == -1) {
                            verticalDirection = -1;
                        }
                        break;
                    }
                }

                for (auto& edge : bottomEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y - 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        if (rectanglePtr->getVerticalDirection() == 1) {
                            verticalDirection = 1;
                        }
                        break;
                    }
                }
            }
            else if (horizontalDirection == -1 && verticalDirection == 0) { // left
                for (auto& edge : rightEdgeCoordinates) {
                    if ((x - 1 == std::get<0>(edge) || x == std::get<0>(edge)) && y == std::get<1>(edge)) {
                        horizontalDirection = -horizontalDirection;
                        break;
                    }
                }

                for (auto& edge : topEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y + 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        if (rectanglePtr->getVerticalDirection() == -1) {
                            verticalDirection = -1;
                        }
                        break;
                    }
                }

                for (auto& edge : bottomEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y - 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        if (rectanglePtr->getVerticalDirection() == 1) {
                            verticalDirection = 1;
                        }
                        break;
                    }
                }

            }
            else if (horizontalDirection == 0 && verticalDirection == -1) { // up
                for (auto& edge : bottomEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y - 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        verticalDirection = -verticalDirection;
                        break;
                    }
                }
            }
            else if (horizontalDirection == 0 && verticalDirection == 1) { // down
                for (auto& edge : topEdgeCoordinates) {
                    if (x == std::get<0>(edge) && (y + 1 == std::get<1>(edge) || y == std::get<1>(edge))) {
                        verticalDirection = -verticalDirection;
                        break;
                    }
                }

            } // rewrite this part, so that it check vertical, horizontal and diagonal fields
            else if (horizontalDirection == 1 && verticalDirection == -1) { // right up
                bool horizontal = false;
                bool vertical = false;
                for (auto& edge : leftEdgeCoordinates) {
                    if (x + 1 == std::get<0>(edge) && y - 1 == std::get<1>(edge)) {
                        horizontal = true;
                        break;
                    }
                }
                for (auto& edge : bottomEdgeCoordinates) {
                    if (x + 1 == std::get<0>(edge) && y - 1 == std::get<1>(edge)) {
                        vertical = true;
                        break;
                    }
                }

                if (horizontal && vertical) {
                    horizontalDirection = -horizontalDirection;
                    verticalDirection = -verticalDirection;
                }
                else if (horizontal) {
                    horizontalDirection = -horizontalDirection;
                }
                else if (vertical) {
                    verticalDirection = -verticalDirection;
                }

            }
            else if (horizontalDirection == -1 && verticalDirection == -1) { // left up
                bool horizontal = false;
                bool vertical = false;
                for (auto& edge : rightEdgeCoordinates) {
                    if (x - 1 == std::get<0>(edge) && y - 1 == std::get<1>(edge)) {
                        horizontal = true;
                        break;
                    }
                }
                for (auto& edge : bottomEdgeCoordinates) {
                    if (x - 1 == std::get<0>(edge) && y - 1 == std::get<1>(edge)) {
                        vertical = true;
                        break;
                    }
                }

                if (horizontal && vertical) {
                    horizontalDirection = -horizontalDirection;
                    verticalDirection = -verticalDirection;
                }
                else if (horizontal) {
                    horizontalDirection = -horizontalDirection;
                }
                else if (vertical) {
                    verticalDirection = -verticalDirection;
                }

            }
            else if (horizontalDirection == 1 && verticalDirection == 1) { // right down
                bool horizontal = false;
                bool vertical = false;
                for (auto& edge : leftEdgeCoordinates) {
                    if (x + 1 == std::get<0>(edge) && y + 1 == std::get<1>(edge)) {
                        horizontal = true;
                        break;
                    }
                }
                for (auto& edge : topEdgeCoordinates) {
                    if (x + 1 == std::get<0>(edge) && y + 1 == std::get<1>(edge)) {
                        vertical = true;
                        break;
                    }
                }

                if (horizontal && vertical) {
                    horizontalDirection = -horizontalDirection;
                    verticalDirection = -verticalDirection;
                }
                else if (horizontal) {
                    horizontalDirection = -horizontalDirection;
                }
                else if (vertical) {
                    verticalDirection = -verticalDirection;
                }

            }
            else if (horizontalDirection == -1 && verticalDirection == 1) { // left down
                bool horizontal = false;
                bool vertical = false;
                for (auto& edge : rightEdgeCoordinates) {
                    if (x - 1 == std::get<0>(edge) && y + 1 == std::get<1>(edge)) {
                        horizontal = true;
                        break;
                    }
                }
                for (auto& edge : topEdgeCoordinates) {
                    if (x - 1 == std::get<0>(edge) && y + 1 == std::get<1>(edge)) {
                        vertical = true;
                        break;
                    }
                }

                if (horizontal && vertical) {
                    horizontalDirection = -horizontalDirection;
                    verticalDirection = -verticalDirection;
                }
                else if (horizontal) {
                    horizontalDirection = -horizontalDirection;
                }
                else if (vertical) {
                    verticalDirection = -verticalDirection;
                }
            }
        }
        if (lock.owns_lock()) {
            lock.unlock();
        }
        // lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100 / speed));

    }
}

int Ball::generateSpeed() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_SPEED);
    return dis(gen);
}
