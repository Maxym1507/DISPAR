#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    HANDLE semaphore = CreateSemaphore(NULL, 0, 5, L"L4_Semaphore");

    if (semaphore == NULL) {
        cout << "Failed to create semaphore." << endl;
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        cout << "Iteration " << i + 1 << " ";
        cin.get();

        ReleaseSemaphore(semaphore, 1, NULL);
        cout << "Semaphore released." << endl << endl;
    }

    CloseHandle(semaphore);

    return 0;
}
