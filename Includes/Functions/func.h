#pragma once
#include <iostream>
#include <windows.h>

// 콘솔 제어 관련
void SetConsoleFontSize(int size);
void ResizeConsole(int width, int height);
void PrintFileContent(FILE* file, const char* filename);