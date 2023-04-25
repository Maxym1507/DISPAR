#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main()
{
    // ��������� ������ ������ ���'��
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, _T("MySharedMemory"));
    if (hMapFile == NULL) {
        cerr << "Could not create file mapping object (" << GetLastError() << ")" << endl;
        return 1;
    }

    // ϳ��������� �� ������ ������ ���'��
    LPTSTR pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
    if (pBuf == NULL) {
        cerr << "Could not map view of file (" << GetLastError() << ")" << endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // ������ �� ��� �� �볺���
    cout << "Waiting for client data..." << endl;
    while (_tcslen(pBuf) == 0) {
        Sleep(100);
    }

    // ��������� ����� �� �볺���
    _tprintf(TEXT("Received data: %s\n"), pBuf);

    // ϳ����������� �볺���, �� ��� ��������
    _tcscpy(pBuf, TEXT("Data received"));

    // ��������� �������
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
