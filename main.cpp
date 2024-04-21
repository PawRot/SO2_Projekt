#include <iostream>
// #include <print>
#include <thread>
#include "utility/Render.h"


int main() {

    std::atomic_bool threadsStopped = false;

    auto render = new Render(&threadsStopped);

    const auto renderThread = new std::thread(&Render::runRender, render);;


    while (true) {
        if (render->stopFlag) {
            while (!threadsStopped) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            break;
        }
    }
     std::cout << "Exiting" << std::endl;
    // std::println("Exiting");


    renderThread->join();


    return 0;
}
