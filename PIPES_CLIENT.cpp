#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
    HANDLE hPipe;
    const wchar_t* pipeName = L"\\\\.\\pipe\\Pipe";
    char buffer[1024];
    DWORD dwRead;

    hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << "Failed to connect to named pipe: " << GetLastError() << endl;
        return 1;
    }

    cout << "Connected to named pipe." << endl;

    string message;
    while (true) {
        cout << "Enter message to send (or 'exit' to quit): ";
        getline(cin, message);

        if (message == "exit") {
            break;
        }

        if (WriteFile(hPipe, message.c_str(), message.size() + 1, NULL, NULL) == FALSE) {
            cout << "Failed to write to pipe: " << GetLastError() << endl;
            break;
        }

        ZeroMemory(buffer, sizeof(buffer));
        if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) != FALSE) {
            cout << "Received response: " << buffer << endl;
        }
    }

    CloseHandle(hPipe);

    return 0;
}
