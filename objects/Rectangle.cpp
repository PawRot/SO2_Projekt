
#include "Rectangle.h"

Rectangle::Rectangle(int windowWidth, int windowHeight, std::atomic_bool *stopFlag) {
    this->stopFlag = stopFlag;
    width = [=](){ // lambda to calculate rectangle width
        const int width = windowWidth / 10;
        if (width % 2 == 0 ) return width;
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

    // temp_y = y;

    this->rectangleThread = new std::thread(&Rectangle::runRectangle, this);
}

Rectangle::~Rectangle() {
    rectangleThread->join();
};

void Rectangle::runRectangle() {
    while (*stopFlag != true) {
        // temp_y = temp_y + 0.000000008 * speed * direction;
        // y = static_cast<int>(temp_y);
        y = y + 1 * direction;
        if (y >= max_y || y <= min_y) {
            if (y >= max_y) {
                y = max_y;
            } else {
                y = min_y;
            }

            speed = generateSpeed();
            direction = -direction;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100 / speed));
    }
}

int Rectangle::generateSpeed() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_SPEED);
    return dis(gen);
}
