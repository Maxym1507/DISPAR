#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>

using namespace std;

int main()
{
    HANDLE hPipe;
    const wchar_t* pipeName = L"\\\\.\\pipe\\Pipe";
    char buffer[1000100];
    DWORD dwRead;
    double bytesSent = 0.0;
    double totalSeconds = 0.0;
    hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << "Failed to connect to named pipe: " << GetLastError() << endl;
        return 1;
    }

    cout << "Connected to named pipe." << endl;
    int i = 0;
    string message(1000000, 'x');
    while (true) {
        cout << "Enter message to send (or 'exit' to quit): ";
        //getline(cin, message);
        if (i == 10)
            message = "exit";
        i++;

        if (message == "exit") {
            break;
        }

        auto start = chrono::high_resolution_clock::now();

        if (WriteFile(hPipe, message.c_str(), message.size() + 1, NULL, NULL) == FALSE) {
            cout << "Failed to write to pipe: " << GetLastError() << endl;
            break;
        }

        auto end = chrono::high_resolution_clock::now();
        auto diff = end - start;
        double seconds = chrono::duration_cast<chrono::microseconds>(diff).count() / 1000000.0;

        bytesSent += message.size();
        totalSeconds += seconds;

        ZeroMemory(buffer, sizeof(buffer));
        if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) != FALSE) {
            cout << "Received response: " << buffer << endl;
        }
    }

    CloseHandle(hPipe);

    double transferRate = (bytesSent/1024/1024) / totalSeconds;

    cout << "Data transfer rate: " << transferRate << " mb/sec" << endl;

    return 0;
}
