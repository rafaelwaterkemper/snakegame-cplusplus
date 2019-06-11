#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>

using namespace std;

bool gameOver;
const int width = 40;
const int height = 20;
int x;
int y;
int fruitX;
int fruitY;
int score;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection direction;
eDirection oldDirection;
int tailX[100];
int tailY[100];
int nTail;

int _kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void Setup()
{
    gameOver = false;
    direction = STOP;
    oldDirection = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tailX[100];
    tailY[100];
}

void Draw()
{
    system("clear");

    for (int i = 0; i < width + 2; i++)
    {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
            {
                cout << "#";
            }

            if (i == y && j == x)
            {
                cout << "O";
            }
            else if (i == fruitY && j == fruitX)
            {
                cout << "F";
            }
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                {
                    cout << " ";
                }
            }

            if (j == width - 1)
                cout << "#";
        };
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
    {
        cout << "#";
    }
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (getchar())
        {
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        };
    }
}

void Logic()
{

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    for (int i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y){
            gameOver = true;
        }
    }

    if (x > width - 1)
    {
        x = 0;
    }
    else if (x < 0)
    {
        x = width - 1;
    }

    if (y > height - 1)
    {
        y = 0;
    }
    else if (y < 0)
    {
        y = height - 1;
    }

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(gameOver){
            system("clear");
            cout << "GAME OVER" << endl << endl;
            cout << endl << endl << endl;
            cout << "Press 1 To Continue";
            cout << endl << endl;
            cout << "Press 0 To Exit";
            cout << endl << endl;
            char option = getchar();
            cout << option;

            if(option ==  '1'){
                Setup();
            }
        }
    }
    return 0;
}