#include "Render.h"

Render::Render(std::atomic_bool* threadsStoppedPtr) {
    initscr();
    nodelay(stdscr, TRUE);
    curs_set(0);
    noecho();
    start_color();

    for (int i = 0; i < COLORS; i++) {
        init_pair(i + 1, i, COLOR_BLACK);
    }

    colors = std::vector(256, false);

    this->stopFlag = false;
    this->width = COLS - 2; // -2 to avoid border
    this->height = LINES - 2;
    this->threadsStoppedPtr = threadsStoppedPtr;


    // this->rectangleThread = new std::thread(&Rectangle::runRectangle, rectangle);
    rectangle = new Rectangle(width, height, &stopFlag);


    this->ballSpawnThread = new std::thread(&Render::spawnBall, this);
    // this->ballSpawnThread = new std::thread([this] {spawnBall();}); // lambda syntax
}

Render::~Render() {
    clear();
    addstr("Waiting for threads to finish\n");
    refresh();


    ballSpawnThread->join();
    delete rectangle;

    for (const auto ball: balls) {
        delete ball;
    }

    balls.clear();


    endwin();
    delete ballSpawnThread;

    *threadsStoppedPtr = true;
}

void Render::stop() {
    stopFlag = true;
    delete this;
}

void Render::runRender() {
    while (true) {
        erase();
        std::this_thread::sleep_for(std::chrono::microseconds(RENDER_SLEEP_TIME));
        draw();
        refresh();

        if (checkKey()) {
            break;
        }
    }
    stop();
}

void Render::spawnBall() {
    while (stopFlag != true) {
        {
            if (balls.size() < MAX_BALLS) {
                std::unique_lock spawnLock(mtx);
                auto ball = new Ball(width, height, &stopFlag, &colors, rectangle, &waitingBalls);
                balls.push_back(ball);
            }
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(300, 600);

        std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));
    }
}

void Render::drawBorder() {
    border(0, 0, 0, 0, 0, 0, 0, 0);
}


void Render::drawRectangle() {
    const auto&x = rectangle->getX();
    const auto&y = rectangle->getY();
    const auto&height = rectangle->getHeight();
    const auto&width = rectangle->getWidth();

    // Draw top and bottom
    mvhline(y, x, 0, width);
    mvhline(y + height, x, 0, width);

    // Draw left and right
    mvvline(y, x, 0, height);
    mvvline(y, x + width, 0, height);

    mvaddch(y, x, ACS_ULCORNER); // top-left corner
    mvaddch(y, x + width, ACS_URCORNER); // top-right corner
    mvaddch(y + height, x, ACS_LLCORNER); // bottom-left corner
    mvaddch(y + height, x + width, ACS_LRCORNER); // bottom-right corner
}

void Render::drawBalls() {
    std::unique_lock drawLock(mtx);
    for (const auto ball: balls) {
        if (ball->finished) {
            std::erase(balls, ball);
            delete ball;
        }
        else if (!ball->getBouncedFromRectangle()) {
            const auto color = ball->color;
            attron(COLOR_PAIR(color));
            mvaddch(ball->y, ball->x, 'o');
            attroff(COLOR_PAIR(color));
        }
        else if (ball->getBouncedFromRectangle()) {
            const auto color = ball->color;
            attron(COLOR_PAIR(color));
            mvaddch(ball->y, ball->x, 'X');
            attroff(COLOR_PAIR(color));
        }
    }

}

void Render::draw() {
    drawBorder();
    std::unique_lock posLock(rectangle->mtx);
    drawRectangle();
    drawBalls();
}


bool Render::checkKey() {
    if (const auto ch = getch(); ch == ' ') {
        return true;
    }

    return false;
}
