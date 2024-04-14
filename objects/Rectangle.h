
#ifndef SO2_PROJEKT_RECTANGLE_H
#define SO2_PROJEKT_RECTANGLE_H


// stała przechowująca kształt prostokąta / funkcja generująca kształt prostokąta
// zmienne przechowująca położenie, prędkość, kierunek

#include <atomic>

class Rectangle {


    int x,y;
    int width, height;
    int speed;

public:
    Rectangle();
    ~Rectangle();


    void runRectangle();


};


#endif //SO2_PROJEKT_RECTANGLE_H
