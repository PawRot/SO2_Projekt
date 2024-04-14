
#ifndef SO2_PROJEKT_RENDER_H
#define SO2_PROJEKT_RENDER_H
#include <vector>
#include <thread>
#include <ncurses.h>
#include "../objects/Ball.h"
#include "../objects/Rectangle.h"

// funkcja, która rysuje obiekty na ekranie
//Ncurses nie jest wielowejściowe - wszystkie funkcje Ncurses w jednym wątku (prawdopodobnie w tym)
//
//Nie używać standardowego wejścia wyjścia z C++ w Ncurses
//
//Kulki wątkami + wątki techniczne + prawdopodobnie wątek do generowania nowych wątków

class Render {

    std::vector<Ball *> balls{};
    Rectangle *rectangle{};

    std::thread *ballSpawnThread{};
    std::thread *rectangleThread{};
    std::thread *renderThread{};


    int x, y; // x - number of columns, y - number of rows

    void spawnBall();
    static void drawBorder();


    bool checkKey();

    int number = 0;



public:
    std::atomic_bool stopFlag = false; // flag that indicates that threads should stop

    Render(int x, int y);
    ~Render();

    void stop();
    void draw();
    void runRender();

};


#endif //SO2_PROJEKT_RENDER_H
