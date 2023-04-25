#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    // Створення об'єкту типу event з іменем "L3_Event"
    HANDLE event = CreateEvent(NULL, FALSE, FALSE, L"L3_Event");
    // Перевірка, чи створення об'єкту відбулося успішно
    if (event == NULL) {
        cout << "Failed to create event." << endl;
        return 1;
    }

    cout << "Press enter to signal the event." << endl;
    cin.get();
    // Сигналізування про подію
    SetEvent(event);
    cout << "Event signalled." << endl;

    cout << "Press enter to wait for the event." << endl;
    cin.get();
    // Очікування на подію
    DWORD result = WaitForSingleObject(event, INFINITE);

    if (result == WAIT_OBJECT_0) {
        cout << "Event received." << endl;
    }
    else {
        cout << "Wait failed." << endl;
    }

    CloseHandle(event);

    return 0;
}
