#pragma once

char ScreenBuffer[65536];

void setScreenSize(int width, int height);
void setCursorIcon(int isActive);
void setCursorPos(int x, int y);
void setCursorVisibility(int isVisible);
void SetColor(unsigned short backgroundColor, unsigned short textColor);

void ClearBuffer();
int WriteToBuffer(int x, int y, const char* str);

#ifndef SCREEN_H
#define SCREEN_H

extern int screenWidth; // 화면의 가로 길이 변수
extern int screenHeight; // 화면의 세로 길이 변수

// 기타 함수들...

#endif // SCREEN_H