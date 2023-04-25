#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main()
{
    // Створення регіону спільної пам'яті
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, _T("MySharedMemory"));
    if (hMapFile == NULL) {
        cerr << "Could not create file mapping object (" << GetLastError() << ")" << endl;
        return 1;
    }

    // Підключення до регіону спільної пам'яті
    LPTSTR pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
    if (pBuf == NULL) {
        cerr << "Could not map view of file (" << GetLastError() << ")" << endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // Чекаємо на дані від клієнта
    cout << "Waiting for client data..." << endl;
    while (_tcslen(pBuf) == 0) {
        Sleep(100);
    }

    // Виведення даних від клієнта
    _tprintf(TEXT("Received data: %s\n"), pBuf);

    // Підтвердження клієнту, що дані отримано
    _tcscpy(pBuf, TEXT("Data received"));

    // Звільнення ресурсів
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
