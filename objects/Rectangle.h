#ifndef SO2_PROJEKT_RECTANGLE_H
#define SO2_PROJEKT_RECTANGLE_H


#include <atomic>
#include <random>
#include <thread>

class Rectangle {
    int MAX_SPEED = 5; // maximum speed of the rectangle

    int max_y, min_y; // maximum and minimum y coordinate of the top left of the rectangle
    int speed; // speed of the rectangle
    int direction = 1; // direction of the rectangle (1 - down , -1 - up)

    std::thread* rectangleThread{}; // thread that runs the rectangle

    std::atomic_bool* stopFlag; // flag that indicates that the rectangle should stop

    int generateSpeed(); // function that generates speed of the rectangle
    void runRectangle();

public:
    int x, y; //x and y coordinates of the top left corner of the rectangle
    int width, height; // width and height of the rectangle


    Rectangle(int windowWidth, int windowHeight, std::atomic_bool* stopFlag);

    ~Rectangle();
};


#endif //SO2_PROJEKT_RECTANGLE_H
