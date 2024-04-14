#include <iostream>
#include <thread>
#include <ncurses.h>
#include "objects/Ball.h"
#include "objects/Rectangle.h"
#include "utility/Render.h"


bool checkKey();

int main() {

    initscr();
    nodelay(stdscr, TRUE);
    curs_set(0);
    noecho();

    auto render = new Render(COLS, LINES);


    while (true) {

        // render->runRender();
        erase();
        std::this_thread::sleep_for(std::chrono::microseconds(16666));
        render->draw();
        refresh();

        if (checkKey()) {
            render->stop();
            break;
        }
    }


    endwin();
    return 0;
}

bool checkKey() {
    if (const auto ch = getch(); ch == ' ') {
        return true;
    }

    return  false;
}






// int main() {
//
//     // Ball ball;
//     auto ball = new Ball();
//
//
//
//     // std::thread ball_thread(&Ball::run_ball, &ball);
//     // std::thread ball_thread([&](){ return ball.run_ball(); });
//
//
//     // std::thread ball_thread(&Ball::run_ball, ball);
//     std::thread ball_thread([&](){ return ball->run_ball(); });
//
//     ball_thread.join(); // wait for the thread to finish
//
//     // std::cout << ball.get_x() << std::endl << ball.get_y() << std::endl;
//     std::cout << ball->get_x() << std::endl << ball->get_y() << std::endl;
//     // delete ball;
//
//     // std::cout << ball->get_x();
//     // ball_thread.join();
//
//
//
//     return 0;
// }

//void print_triangle(int startRow,int startCol,int height);      //function declaration to print triangle
//void erase_triangle(int startRow,int startCol,int height);     //function declaration to erase triangle
//
//int main()
//
//{
//    int sr=10;                                        //variable which holds the starting row value
//    initscr();
//
//    for(int m=1;m<=15;m++)                //this loop decides how many times we need to move the triangle
//    {
//        print_triangle(sr,45,4);             //print triangle
//        refresh();                                 //refresh the screen
//
//        for(int s=1;s<=500000000;s++);   //loop which controls the speed of the moving triangle
//
//        erase_triangle(sr,45,4);           //erase the triangle which is drawn before
//        refresh();                                //refresh the screen
//
//        sr++;                                             //increment the starting row by one
//
//    }
//    getch();
//    endwin();
//    return 0;
//
//}
//
////----------------------------------------------------------------------------------
//void print_triangle(int startRow,int startCol,int height)
//{
//    int x=startCol;
//    for(int r=startRow;r>=startRow-height;r--)
//    {
//        for(int c=startCol;c<=x;c++)
//        {
//            move(r,c);
//            printw("*");
//        }
//        x++;
//        startCol--;
//    }
//
//}
//
////----------------------------------------------------------------------------------
//void erase_triangle(int startRow,int startCol,int height)
//{
//    int x=startCol;
//    for(int r=startRow;r>=startRow-height;r--)
//    {
//        for(int c=startCol;c<=x;c++)
//        {
//            move(r,c);
//            printw(" ");
//        }
//        x++;
//        startCol--;
//    }
//
//}
