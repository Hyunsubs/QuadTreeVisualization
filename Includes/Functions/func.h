#pragma once
#include <iostream>
#include <windows.h>

// �ܼ� ���� ����
void SetConsoleFontSize(int size);
void ResizeConsole(int width, int height);
void PrintFileContent(FILE* file, const char* filename);