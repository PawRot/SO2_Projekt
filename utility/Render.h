
#ifndef SO2_PROJEKT_RENDER_H
#define SO2_PROJEKT_RENDER_H
#include <vector>
#include <thread>
#include <ncurses.h>
#include "../objects/Ball.h"
#include "../objects/Rectangle.h"


// TODO: rewrite this comment so it describes the finished version of the program, add english version
// funkcja, która rysuje obiekty na ekranie
//Ncurses nie jest wielowejściowe - wszystkie funkcje Ncurses w jednym wątku (prawdopodobnie w tym)
//
//Nie używać standardowego wejścia wyjścia z C++ w Ncurses
//
//Kulki wątkami + wątki techniczne + prawdopodobnie wątek do generowania nowych wątków

class Render {

    std::vector<Ball *> balls{}; // stores pointers to ball objects
    Rectangle *rectangle{}; // stores pointer to rectangle object

    std::thread *ballSpawnThread{};
    std::thread *rectangleThread{};


    int x, y; // x - number of columns, y - number of rows

    void spawnBall(); // function that spawns a ball, runs in a separate thread
    static void drawBorder(); // function that draws the border


    bool checkKey(); // function that checks if key was pressed

    int number = 0;



public:
    std::atomic_bool stopFlag = false; // flag that indicates that threads should stop
    std::atomic_bool keyPressed = false; // flag that indicates that key was pressed
    std::atomic_bool *threadsStoppedPtr; // pointer to a flag that indicates that threads stopped

    Render(int x, int y, std::atomic_bool *threadsStoppedPtr);
    ~Render();

    void stop();
    void draw();
    void runRender();

};


#endif //SO2_PROJEKT_RENDER_H
