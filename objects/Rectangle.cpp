#include "Rectangle.h"

Rectangle::Rectangle(int windowWidth, int windowHeight, std::atomic_bool* stopFlag) {
    this->stopFlag = stopFlag;
    width = [=]() {
        // lambda to calculate rectangle width
        const int width = windowWidth / 10;
        if (width % 2 == 0) return width;
        return width - 1;
    }();
    height = [=] {
        const int height = windowHeight / 5;
        if (height % 2 == 0) return height;
        return height - 1;
    }();


    max_y = windowHeight - height;
    min_y = 0 + 1;

    x = windowWidth / 8;
    y = windowHeight / 2;

    speed = generateSpeed();


    this->rectangleThread = new std::thread(&Rectangle::runRectangle, this);

}

Rectangle::~Rectangle() {
    rectangleThread->join();
    delete rectangleThread;
};

void Rectangle::runRectangle() {
    while (*stopFlag != true) {
        std::unique_lock lock(mtx);
        y = y + 1 * direction;
        calculateEdges();
        lock.unlock();
        if (y >= max_y || y <= min_y) {
            if (y >= max_y) {
                y = max_y;
            }
            else {
                y = min_y;
            }

            speed = generateSpeed();
            direction = -direction;
        }

        // calculate the coordinates of the edges of the rectangle

        std::this_thread::sleep_for(std::chrono::milliseconds(500 / speed));
    }
}

void Rectangle::calculateEdges() {
    // Clear previous edge coordinates
    topEdgeCoordinates.clear();
    bottomEdgeCoordinates.clear();
    leftEdgeCoordinates.clear();
    rightEdgeCoordinates.clear();

    // Calculate top and bottom edge coordinates
    for (int i = x; i <= x + width; i++) {
        topEdgeCoordinates.emplace_back(i, y);
        bottomEdgeCoordinates.emplace_back(i, y + height);
    }

    // Calculate left and right edge coordinates
    for (int i = y; i <= y + height; i++) {
        leftEdgeCoordinates.emplace_back(x, i);
        rightEdgeCoordinates.emplace_back(x + width, i);
    }
}

int Rectangle::generateSpeed() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_SPEED);
    return dis(gen);
}
