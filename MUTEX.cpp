#include <iostream>
#include <windows.h>

using namespace std;

HANDLE mutex;
int sharedVariable = 0;

void increment() {
    // Захоплення м'ютексу
    WaitForSingleObject(mutex, INFINITE);

    // Збільшення спільної змінної на 1
    sharedVariable++;

    // Звільнення м'ютексу
    ReleaseMutex(mutex);
}

int main() {
    mutex = CreateMutex(NULL, FALSE, L"L5_Mutex");
    
    if (mutex == NULL) {
        cout << "Failed to create mutex." << endl;
        return 1;
    }

    // Створення 3 потоків, кожен з яких буде збільшувати спільну змінну на 1
    HANDLE threads[3];
    for (int i = 0; i < 3; i++) {
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)increment, NULL, 0, NULL);
    }

    // Очікування завершення всіх потоків
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    // Виведення значення спільної змінної
    cout << "Shared variable: " << sharedVariable << endl;

    // Звільнення ресурсів
    CloseHandle(mutex);
    for (int i = 0; i < 3; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
