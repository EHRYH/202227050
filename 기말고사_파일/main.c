#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "screen.h"
#include "util.h"

int Initialize();
int Release();
int DrawBuffer(int DeltaTime);
void ProcessInput();
void InitApple();
void InitBApple();
int IsCollision(int x, int y);

// ���ӽ�����Ʈ�� 0�϶� ������ ���� ����
int GameState;
int GameStartSnakeLength;
int ActiveSnakeNum;
int SnakeDirection; // �������� ������ ��Ÿ���� ����
int Score;
int SnakeSpeed; // ���� �ӵ� ����

#define MAX_SNAKE_LENGTH 1400
struct GameObject
{
    int posX;
    int posY;
    int isActive;
};

// 0���� �Ӹ�
struct GameObject snakeSkin[MAX_SNAKE_LENGTH];
struct GameObject apple;
struct GameObject bapple;

int main()
{
    // ���� ȭ�� ���
    CHECKERROR(mainscreen());








    // P�� ���� ������ ���
    while (1)
    {
        if (_kbhit())
        {
            int key = _getch();
            if (key == 'p' || key == 'P')
            {
                ClearBuffer();
                break;
            }
            else if (key == 27) // ESC Ű�� ���� ��� ���α׷� ����
            {
                return 0;
            }
        }
    }

    CHECKERROR(Initialize());

    unsigned long time_end = GetTickCount();
    unsigned long deltaTime = GetTickCount();

    while (GameState)
    {
        deltaTime = GetTickCount() - time_end;
        time_end = GetTickCount();

        ProcessInput(); // �Է� ó��

        DrawBuffer(deltaTime);

        if (deltaTime < SnakeSpeed)
            Sleep(SnakeSpeed - deltaTime);

        if (!GameState)
        {
            printf("Game Over!\n");
            printf("|                             Score: % d\n", Score);
            printf("|       Press 'P' to restart the game or ESC to exit...");
            while (1)
            {
                if (_kbhit())
                {
                    int key = _getch();
                    if (key == 'p' || key == 'P')
                    {
                        GameState = 1; // ���� �����
                        Initialize(); // ���� �ʱ�ȭ
                        Score = 0; // Score �ʱ�ȭ
                        SnakeSpeed = 200;

                        break;
                    }
                    else
                    {
                        exit(0); // ���� ����
                    }
                }
            }
        }
    }

    CHECKERROR(Release());

    return 0;
}


void InitApple()
{
    srand(time(NULL));
    int isColliding = 1;
    while (isColliding)
    {
        apple.posX = (rand() % 32) * 2 + 2; // 1���� 68 ������ ���� ����
        apple.posY = (rand() % 7) * 2 + 2; // 1���� 18 ������ ���� ����
        isColliding = IsCollision(apple.posX, apple.posY);
    }
    apple.isActive = 1;
}

void InitBApple()
{
    srand(time(NULL));
    int isColliding = 1;
    while (isColliding)
    {
        bapple.posX = (rand() % 32) * 2 + 1; // 1���� 68 ������ ���� ����
        bapple.posY = (rand() % 7) * 2 + 1; // 1���� 18 ������ ���� ����
        isColliding = IsCollision(bapple.posX, bapple.posY);
    }
    bapple.isActive = 1;
}

int mainscreen() {
    setCursorVisibility(0);
    ClearBuffer();


    DrawBuffer(0);
    // ���� ȭ�� ���

    printf("BitEat! \n");
    printf("\n");
    setCursorVisibility(0);
    printf("|                    Press 'P' to start the game...");
    printf("\n");
    printf("\n");
    SetColor(0b1111, 0b0010);

    printf("|                             000 000 000\n");
    printf("|                           000 000 000 000");
    SetColor(0b1000, 0b1111);

    printf("\n\n\n\n\n                                                                        ");
    printf("\n                                                             TUTORIALS  ");
    printf("\n                                                                        ");
    printf("\n                 ���۹��: �����Ű�� ���� ������ ������ �� �ֽ��ϴ�. ");
    printf("\n             ����� ������ ���� �ӵ��� �������� ���� ������ ������ϴ�. ");
    printf("\n      �����(�Ķ� ���)�� �����ϰų� ���� �Ӹ��� ������ ���ӿ����Դϴ�. ");
    printf("\n                       ���� ������ �����Ѹ�ŭ ��������� ����� ������. ");
    printf("\n                                                                        ");

    return 0;
}



int Initialize()
{

    GameState = 1;
    GameStartSnakeLength = 3;
    ActiveSnakeNum = GameStartSnakeLength;
    Score = 0;
    SnakeSpeed = 200; // �ʱ� �� �ӵ�
    setCursorVisibility(0);
    SetColor(0b1000, 0b1111);
    ClearBuffer();


    InitApple();
    int i = 0;
    while (i < GameStartSnakeLength)
    {
        if (snakeSkin[i].posX == apple.posX && snakeSkin[i].posY == apple.posY)
        {
            InitApple();
            i = 0;
        }
        else
        {
            i = i + 1;
        }
    }

    InitBApple();
    int j = 0;
    while (j < GameStartSnakeLength)
    {
        if (snakeSkin[j].posX == bapple.posX && snakeSkin[j].posY == bapple.posY)
        {
            InitBApple();
            j = 0;
        }
        else
        {
            j = j + 1;
        }
    }

    // �� ���׾Ƹ� �ʱ�ȭ
    i = 0;
    while (i < MAX_SNAKE_LENGTH)
    {
        snakeSkin[i].posX = 35;
        snakeSkin[i].posY = 10;
        snakeSkin[i].isActive = 0;
        i = i + 1;
    }

    i = 0;
    while (i < GameStartSnakeLength)
    {
        snakeSkin[i].posX = 35 - i;
        snakeSkin[i].posY = 10;
        snakeSkin[i].isActive = 1;
        i = i + 1;
    }

    SnakeDirection = 0; // �ʱ� ���� ���� (������)

    return 0;
}

int Release()
{
    return 0;
}

void MoveSnake(int dir)
{
    // ���� 
    int i = GameStartSnakeLength - 1;
    while (i > 0)
    {
        snakeSkin[i].posX = snakeSkin[i - 1].posX;
        snakeSkin[i].posY = snakeSkin[i - 1].posY;
        i = i - 1;
    }
    // �Ӹ� 
    if (dir == 0) // ����
        snakeSkin[0].posX = snakeSkin[0].posX - 1;
    else if (dir == 1) // ������
        snakeSkin[0].posX = snakeSkin[0].posX + 1;
    else if (dir == 2) // ����
        snakeSkin[0].posY = snakeSkin[0].posY - 1;
    else if (dir == 3) // �Ʒ���
        snakeSkin[0].posY = snakeSkin[0].posY + 1;
}

void ProcessInput()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == 224) // ȭ��ǥ Ű�� ���
        {
            key = _getch();
            if (GameState == 0 && key == 80) // ���� ���� ȭ�鿡�� P Ű ������ ��
            {
                GameState = 1; // ���� �����
                Initialize(); // ���� �ʱ�ȭ
            }


            else if (GameState == 1) // ���� ���� ���� ���� ���� ����
            {
                if (key == 75) // ���� ȭ��ǥ
                    SnakeDirection = 0; // ����
                else if (key == 77) // ���� ȭ��ǥ
                    SnakeDirection = 1; // ������
                else if (key == 72) // ��� ȭ��ǥ
                    SnakeDirection = 2; // ����
                else if (key == 80) // �ϴ� ȭ��ǥ
                    SnakeDirection = 3; // �Ʒ���
            }
        }
        else if (GameState == 0 && key == 112) // ���� ���� ȭ�鿡�� P Ű ������ �� (ASCII �ڵ�� 112�� 'p'�� ��Ÿ��)
        {
            GameState = 1; // ���� �����
            Initialize(); // ���� �ʱ�ȭ
        }
    }
}







int DeltaTimeSum = 0;

int IsCollisionWithWall(int x, int y)
{
    if (x <= 0 || x >= 70 || y <= 0 || y >= 20)
        return 1;
    return 0;
}

int DrawBuffer(int DeltaTime)

{
    setCursorVisibility(0);
    setCursorPos(0, 0);
    SetColor(0b1111, 0b0000);
    printf("%s", ScreenBuffer);
    SetColor(0b0000, 0b1111);
    setCursorPos(0, 22);
    printf("Score: 0    \n");
    printf("Speed: 0    \n");
    SetColor(0b0000, 0b1111);
    setCursorPos(0, 24);
    if (DeltaTime != 0)
        printf("Time: %d ms\nFPS: %d\n", DeltaTime, 1000 / DeltaTime);
    DeltaTimeSum = DeltaTimeSum + DeltaTime;
    if (DeltaTimeSum >= SnakeSpeed)
    {
        MoveSnake(SnakeDirection); // ���⿡ ���� ������ �̵�
        DeltaTimeSum = 0;

        if (IsCollisionWithWall(snakeSkin[0].posX, snakeSkin[0].posY))
        {
            GameState = 0; // ���� ����
            ClearBuffer();
            setCursorPos(35, 10);
            SetColor(0b1111, 0b0000);
            setCursorPos(30, 12);
            return 0;
        }
    }
    if (snakeSkin[0].posX == apple.posX && snakeSkin[0].posY == apple.posY)
    {
        GameStartSnakeLength++;
        Score++;

        if (Score <= 10)
            SnakeSpeed -= 15; // �ӵ� ����

        InitApple();

        if (Score % 5 == 0) {
            InitBApple();
        }
    }
    if (snakeSkin[0].posX == bapple.posX && snakeSkin[0].posY == bapple.posY)
    {
        GameState = 0; // ���� ����
        ClearBuffer();
        setCursorPos(35, 10);
        SetColor(0b1111, 0b0000);
        setCursorPos(30, 12);
        return 0;
    }
    // apple
    setCursorPos(apple.posX, apple.posY);
    SetColor(0b1111, 0b0100);
    printf("��");

    // blue apple
    setCursorPos(bapple.posX, bapple.posY);
    SetColor(0b1111, 0b0001);
    printf("��");



    // snake
    int i = 0;
    while (i < GameStartSnakeLength)
    {
        setCursorPos(snakeSkin[i].posX, snakeSkin[i].posY);
        SetColor(0b1111, 0b0010);
        printf("O");
        i = i + 1;
    }

    SetColor(0b0000, 0b1111);
    setCursorPos(0, 22);
    printf("Score: %d\n", Score);
    printf("Speed: %d\n", (-(-200 - (-1) * SnakeSpeed)));

    SetColor(0b0000, 0b1111);
    setCursorPos(0, 24);
    if (DeltaTime != 0)
        printf("Time: %d ms\nFPS:   %d   \n", DeltaTime, 1000 / DeltaTime);

    return 0;
}

int IsCollision(int x, int y)
{
    int i;
    for (i = 0; i < GameStartSnakeLength; i++)
    {
        if (snakeSkin[i].posX == x && snakeSkin[i].posY == y)
            return 1;
    }
    return 0;
}