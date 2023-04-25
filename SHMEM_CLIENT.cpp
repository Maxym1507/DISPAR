#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main()
{
    // Відкриття регіону спільної пам'яті
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MySharedMemory");
    if (hMapFile == NULL) {
        cerr << "Could not open file mapping object (" << GetLastError() << ")" << endl;
        return 1;
    }

    // Підключення до регіону спільної пам'яті
    LPCTSTR pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
    if (pBuf == NULL) {
        cerr << "Could not map view of file (" << GetLastError() << ")" << endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // Запис даних у спільну пам'ять
    _tcscpy((LPTSTR)pBuf, TEXT("Hello, Server!"));

    // Чекаємо, доки сервер не підтвердить
    while (_tcslen((LPTSTR)pBuf) > 0) {
        Sleep(1000);
    }

    // Від'єднання від регіону спільної пам'яті
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
