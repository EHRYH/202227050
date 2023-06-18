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

// 게임스테이트가 0일때 무조건 게임 종료
int GameState;
int GameStartSnakeLength;
int ActiveSnakeNum;
int SnakeDirection; // 지렁이의 방향을 나타내는 변수
int Score;
int SnakeSpeed; // 뱀의 속도 변수

#define MAX_SNAKE_LENGTH 1400
struct GameObject
{
    int posX;
    int posY;
    int isActive;
};

// 0번이 머리
struct GameObject snakeSkin[MAX_SNAKE_LENGTH];
struct GameObject apple;
struct GameObject bapple;

int main()
{
    // 시작 화면 출력
    CHECKERROR(mainscreen());








    // P를 누를 때까지 대기
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
            else if (key == 27) // ESC 키를 누를 경우 프로그램 종료
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

        ProcessInput(); // 입력 처리

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
                        GameState = 1; // 게임 재시작
                        Initialize(); // 게임 초기화
                        Score = 0; // Score 초기화
                        SnakeSpeed = 200;

                        break;
                    }
                    else
                    {
                        exit(0); // 게임 종료
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
        apple.posX = (rand() % 32) * 2 + 2; // 1부터 68 사이의 난수 생성
        apple.posY = (rand() % 7) * 2 + 2; // 1부터 18 사이의 난수 생성
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
        bapple.posX = (rand() % 32) * 2 + 1; // 1부터 68 사이의 난수 생성
        bapple.posY = (rand() % 7) * 2 + 1; // 1부터 18 사이의 난수 생성
        isColliding = IsCollision(bapple.posX, bapple.posY);
    }
    bapple.isActive = 1;
}

int mainscreen() {
    setCursorVisibility(0);
    ClearBuffer();


    DrawBuffer(0);
    // 시작 화면 출력

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
    printf("\n                 조작방법: ↑↓←→키를 눌러 방향을 조절할 수 있습니다. ");
    printf("\n             사과에 접촉할 수록 속도가 빨라지고 뱀의 꼬리가 길어집니다. ");
    printf("\n      독사과(파란 사과)와 접촉하거나 벽에 머리를 박으면 게임오버입니다. ");
    printf("\n                       뱀의 꼬리를 가능한만큼 길어지도록 만들어 보세요. ");
    printf("\n                                                                        ");

    return 0;
}



int Initialize()
{

    GameState = 1;
    GameStartSnakeLength = 3;
    ActiveSnakeNum = GameStartSnakeLength;
    Score = 0;
    SnakeSpeed = 200; // 초기 뱀 속도
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

    // 뱀 몸뚱아리 초기화
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

    SnakeDirection = 0; // 초기 방향 설정 (오른쪽)

    return 0;
}

int Release()
{
    return 0;
}

void MoveSnake(int dir)
{
    // 몸통 
    int i = GameStartSnakeLength - 1;
    while (i > 0)
    {
        snakeSkin[i].posX = snakeSkin[i - 1].posX;
        snakeSkin[i].posY = snakeSkin[i - 1].posY;
        i = i - 1;
    }
    // 머리 
    if (dir == 0) // 왼쪽
        snakeSkin[0].posX = snakeSkin[0].posX - 1;
    else if (dir == 1) // 오른쪽
        snakeSkin[0].posX = snakeSkin[0].posX + 1;
    else if (dir == 2) // 위쪽
        snakeSkin[0].posY = snakeSkin[0].posY - 1;
    else if (dir == 3) // 아래쪽
        snakeSkin[0].posY = snakeSkin[0].posY + 1;
}

void ProcessInput()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == 224) // 화살표 키의 경우
        {
            key = _getch();
            if (GameState == 0 && key == 80) // 게임 종료 화면에서 P 키 눌렀을 때
            {
                GameState = 1; // 게임 재시작
                Initialize(); // 게임 초기화
            }


            else if (GameState == 1) // 게임 중일 때만 방향 조작 가능
            {
                if (key == 75) // 좌측 화살표
                    SnakeDirection = 0; // 왼쪽
                else if (key == 77) // 우측 화살표
                    SnakeDirection = 1; // 오른쪽
                else if (key == 72) // 상단 화살표
                    SnakeDirection = 2; // 위쪽
                else if (key == 80) // 하단 화살표
                    SnakeDirection = 3; // 아래쪽
            }
        }
        else if (GameState == 0 && key == 112) // 게임 종료 화면에서 P 키 눌렀을 때 (ASCII 코드로 112는 'p'를 나타냄)
        {
            GameState = 1; // 게임 재시작
            Initialize(); // 게임 초기화
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
        MoveSnake(SnakeDirection); // 방향에 따라 지렁이 이동
        DeltaTimeSum = 0;

        if (IsCollisionWithWall(snakeSkin[0].posX, snakeSkin[0].posY))
        {
            GameState = 0; // 게임 정지
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
            SnakeSpeed -= 15; // 속도 증가

        InitApple();

        if (Score % 5 == 0) {
            InitBApple();
        }
    }
    if (snakeSkin[0].posX == bapple.posX && snakeSkin[0].posY == bapple.posY)
    {
        GameState = 0; // 게임 정지
        ClearBuffer();
        setCursorPos(35, 10);
        SetColor(0b1111, 0b0000);
        setCursorPos(30, 12);
        return 0;
    }
    // apple
    setCursorPos(apple.posX, apple.posY);
    SetColor(0b1111, 0b0100);
    printf("●");

    // blue apple
    setCursorPos(bapple.posX, bapple.posY);
    SetColor(0b1111, 0b0001);
    printf("●");



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