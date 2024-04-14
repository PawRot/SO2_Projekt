#include "Render.h"

Render::Render(const int x, const int y, std::atomic_bool *threadsStoppedPtr) {
    this->x = x;
    this->y = y;
    this->threadsStoppedPtr = threadsStoppedPtr;

    // this->ballSpawnThread = new std::thread(&Render::spawnBall, this);
    // this->ballSpawnThread = new std::thread([this] {spawnBall();}); // lambda syntax

    // auto rectangle = new Rectangle();
    // this->rectangleThread = new std::thread(&Rectangle::runRectangle, rectangle);

    // this->renderThread = new std::thread(&Render::runRender, this);

}

Render::~Render() {
    clear();
    addstr("Waiting for threads to finish\n");
    refresh();

    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // ballSpawnThread->join();
    // rectangleThread->join();

    // for (const auto ball : balls) {
    // delete ball;
    // }

    // balls.clear();

    // delete rectangle;

    // if (renderThread->joinable()) {
        // renderThread->join();
    // }


   *threadsStoppedPtr = true;
}

void Render::stop() {

    stopFlag = true;
    delete this;
}

void Render::runRender() {

    while(true) {
        erase();
        std::this_thread::sleep_for(std::chrono::microseconds(16666));
        draw();
        refresh();

        if(checkKey()) {
            break;
        }
    }
    stop();
}

void Render::spawnBall() {
}

void Render::drawBorder() {
}

void Render::draw() {
    auto str = (std::to_string(number) + "\n").c_str();
    addstr(str);
    number += 1;
}


bool Render::checkKey() {
    if (const auto ch = getch(); ch == ' ') {
        return true;
    }

    return  false;
}

