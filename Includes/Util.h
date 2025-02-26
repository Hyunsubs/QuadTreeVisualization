#pragma once
#include <iostream>
#include <windows.h>


class ENGINE_API Util
{
public:
    static std::string LoadFileContent(const char* filename)
    {
        std::string result;
        FILE* file = nullptr;
        // 파일 열기
        fopen_s(&file, filename, "rt");
        if (!file) {
            std::cerr << "Could not open file: " << filename << std::endl;
            return result;
        }

        // 파일 크기 알아내기
        fseek(file, 0, SEEK_END); // 파일 끝으로 이동
        long fileSize = ftell(file); // 파일 크기 가져오기
        fseek(file, 0, SEEK_SET); // 파일 처음으로 이동

        // 파일 내용을 저장할 버퍼 할당
        char* buffer = (char*)malloc(fileSize + 1);
        if (!buffer) {
            std::cerr << "Memory allocation failed." << std::endl;
            fclose(file);
            return result;
        }

        // 파일 내용 읽기
        fread(buffer, 1, fileSize + 1, file);
        buffer[fileSize] = '\0'; // null-terminator 추가

        result += buffer;


        // 메모리 해제 및 파일 닫기
        free(buffer);
        fclose(file);

        return result;
    }
};


