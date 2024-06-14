#ifndef SO2_PROJEKT_RECTANGLE_H
#define SO2_PROJEKT_RECTANGLE_H


#include <atomic>
#include <random>
#include <thread>
#include <mutex>

class Rectangle {
    int MAX_SPEED = 5; // maximum speed of the rectangle
    int const BASE_SLEEP = 250; // base sleep time in milliseconds



    int x, y; //x and y coordinates of the top left corner of the rectangle
    int width, height; // width and height of the rectangle
    int max_y, min_y; // maximum and minimum y coordinate of the top left of the rectangle
    int speed; // speed of the rectangle
    int direction = 1; // direction of the rectangle (1 - down, -1 - up)


    std::thread* rectangleThread{}; // thread that runs the rectangle

    std::atomic_bool* stopFlag; // flag that indicates that the rectangle should stop

    [[nodiscard]] int generateSpeed() const; // function that generates speed of the rectangle
    void runRectangle();
    // void calculateEdges(); // function that calculates the coordinates of the edges of the rectangle


public:

    std::mutex mtx; // mutex that protects the rectangle



    Rectangle(int windowWidth, int windowHeight, std::atomic_bool* stopFlag);

    ~Rectangle();

    [[nodiscard]] int getVerticalDirection() const {
        return direction;
    }

    [[nodiscard]] int getX() const {
        return x;
    }

    [[nodiscard]] int getY() const {
        return y;
    }

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    [[nodiscard]] int getSpeed() const {
        return speed;
    }

    [[nodiscard]] int getBaseSleep() const {
        return BASE_SLEEP;
    }


};


#endif //SO2_PROJEKT_RECTANGLE_H
