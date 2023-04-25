#include <iostream>
#include <windows.h>

using namespace std;

HANDLE mutex;
int sharedVariable = 0;

void increment() {
    // ���������� �'������
    WaitForSingleObject(mutex, INFINITE);

    // ��������� ������ ����� �� 1
    sharedVariable++;

    // ��������� �'������
    ReleaseMutex(mutex);
}

int main() {
    mutex = CreateMutex(NULL, FALSE, L"L5_Mutex");
    
    if (mutex == NULL) {
        cout << "Failed to create mutex." << endl;
        return 1;
    }

    // ��������� 3 ������, ����� � ���� ���� ���������� ������ ����� �� 1
    HANDLE threads[3];
    for (int i = 0; i < 3; i++) {
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)increment, NULL, 0, NULL);
    }

    // ���������� ���������� ��� ������
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    // ��������� �������� ������ �����
    cout << "Shared variable: " << sharedVariable << endl;

    // ��������� �������
    CloseHandle(mutex);
    for (int i = 0; i < 3; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
