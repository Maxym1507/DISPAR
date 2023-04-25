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
	
	// Вимірювання часу передачі даних
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	// Чекаємо, доки сервер не підтвердить
	while (_tcslen((LPTSTR)pBuf) <= 0) {
		Sleep(1000);
	}

	QueryPerformanceCounter(&end);

	// Обрахування швидкості передачі даних
	double transferTime = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	double transferRate = 1024 / transferTime;

	// Виведення результатів
	cout << "Transfer rate: " << transferRate/1024/1024 << " mb/sec" << endl;

	// Від'єднання від регіону спільної пам'яті
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	return 0;
}
