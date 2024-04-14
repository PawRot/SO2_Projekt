#include "Render.h"

Render::Render(const int x, const int y) {
    this->x = x;
    this->y = y;

    // this->ballSpawnThread = new std::thread(&Render::spawnBall, this);
    // this->ballSpawnThread = new std::thread([this] {spawnBall();}); // lambda syntax

    // auto rectangle = new Rectangle();
    // this->rectangleThread = new std::thread(&Rectangle::runRectangle, rectangle);
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
}

void Render::stop() {

    stopFlag = true;
    delete this;
}

void Render::runRender() {
    erase();
    std::this_thread::sleep_for(std::chrono::microseconds(16666));
    draw();
    refresh();
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
