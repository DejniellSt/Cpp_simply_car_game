// MAC version 

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = {
    {' ', '#', '#', ' '},
    {'#', '#', '#', '#'},
    {' ', '#', '#', ' '},
    {'#', '#', '#', '#'}
};

int carPos = WIN_WIDTH / 2;
int score = 0;


// Sets the console cursor to position (x,y)
void gotoxy(int x, int y) {
    move(y, x);
}

// Sets the visibility and size of the console cursor.
void setcursor(bool visible) {
    curs_set(visible ? 1 : 0);
}

// Draws the board border (frame) of the game in the console
void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 17; j++) {
            mvaddch(i, j, '|');
            mvaddch(i, WIN_WIDTH - j, '|');
        }
        mvaddch(i, SCREEN_WIDTH, '|');
    }
    refresh();
}

// Generates a random position X for the opponent with index ind
void genEnemy(int ind) {
    enemyX[ind] = 17 + rand() % 33;
}

// Draws the opponent at index ind on the screen.
void drawEnemy(int ind) {
    if (enemyFlag[ind]) {
        mvprintw(enemyY[ind], enemyX[ind], "****");
        mvprintw(enemyY[ind] + 1, enemyX[ind], " ** ");
        mvprintw(enemyY[ind] + 2, enemyX[ind], "****");
        mvprintw(enemyY[ind] + 3, enemyX[ind], " ** ");
        refresh();
    }
}

// Removes the graphic image of the opponent from the screen
void eraseEnemy(int ind) {
    if (enemyFlag[ind]) {
        for (int i = 0; i < 4; ++i)
            mvprintw(enemyY[ind] + i, enemyX[ind], "    ");
    }
}

// Resets the position and status of the opponent with index ind.
void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

// Draws the player (car) on the screen at the current position carPos
void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mvaddch(i + 22, j + carPos, car[i][j]);
        }
    }
    refresh();
}

// Removes the graphic image of the car from the screen
void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mvaddch(i + 22, j + carPos, ' ');
        }
    }
}

// Check collison
int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
            return 1;
        }
    }
    return 0;
}

// Display end cridential
void gameover() {
    clear();
    mvprintw(10, 30, "--------------------------");
    mvprintw(11, 30, "-------- Game Over -------");
    mvprintw(12, 30, "--------------------------");
    mvprintw(14, 25, "Press any key to go back to menu.");
    refresh();
    getch();
}

// Update score points 
void updateScore() {
    mvprintw(5, WIN_WIDTH + 7, "Score: %d", score);
    refresh();
}

// AI trail 
/*void autoMoveCar() {
    // Najpierw koryguj pozycję, jeśli już poza granicami:
    if(carPos < 18) carPos = 18;
    if(carPos > 50) carPos = 50;

    bool moveLeft = false;
    bool moveRight = false;

    for(int i = 0; i < 2; i++) {
        if(enemyFlag[i] && enemyY[i] > 10 && enemyY[i] < 23) {  // wyczul na wroga bliżej
            if(enemyX[i] >= carPos && carPos > 18) {
                moveLeft = true;  // wróg po prawej → jedź w lewo
            } else if(enemyX[i] + 4 <= carPos && carPos < 50) {
                moveRight = true; // wróg po lewej → jedź w prawo
            }
        }
    }

    if(moveLeft && !moveRight) {
        carPos -= 4;
    } else if(moveRight && !moveLeft) {
        carPos += 4;
    }
    
    // Zapewnij granice ruchu
    if(carPos < 18) carPos = 18;
    if(carPos > 50) carPos = 50;
}*/

// Supports car movement logic
void play() {
    carPos = WIN_WIDTH / 2 - 1;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    clear();
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    mvprintw(2, WIN_WIDTH + 7, "Car Game");
    mvprintw(4, WIN_WIDTH + 6, "----------");
    mvprintw(6, WIN_WIDTH + 6, "----------");
    mvprintw(12, WIN_WIDTH + 7, "Control");
    mvprintw(13, WIN_WIDTH + 7, "--------");
    mvprintw(14, WIN_WIDTH + 2, "A Key - Left");
    mvprintw(15, WIN_WIDTH + 2, "D Key - Right");
    mvprintw(5, 18, "Press any key to start");
    refresh();
    getch();
    mvprintw(5, 18, "                      ");

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    while (1) {
        int ch = getch();
        if (ch != ERR) {
            if (ch == 'a' || ch == 'A') {
                if (carPos > 18)
                    carPos -= 4;
            } else if (ch == 'd' || ch == 'D') {
                if (carPos < 50)
                    carPos += 4;
            } else if (ch == 27) {
                break;
            }
        }
        //autoMoveCar();

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if (collision() == 1) {
            gameover();
            return;
        }
        usleep(50000);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if (enemyY[0] == 10 && enemyFlag[1] == 0)
            enemyFlag[1] = 1;

        if (enemyFlag[0] == 1)
            enemyY[0] += 1;

        if (enemyFlag[1] == 1)
            enemyY[1] += 1;

        if (enemyY[0] > SCREEN_HEIGHT - 4) {
            resetEnemy(0);
            score++;
            updateScore();
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4) {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
    nodelay(stdscr, FALSE);
}

int main() {
    initscr();
    noecho();
    setcursor(0);
    srand(time(0));

    while (1) {
        clear();
        
        mvprintw(5, 10, " -------------------------- ");
        mvprintw(6, 10, " |        Car Game        | ");
        mvprintw(7, 10, " -------------------------- ");
        mvprintw(8, 10, "Select option: ");
        mvprintw(9, 10, "1. Start Game");
        mvprintw(10, 10, "2. Quit");
        
        refresh();
        int op = getch();

        if (op == '1') play();
        else if (op == '2') break;
    }

    endwin();
    return 0;
}