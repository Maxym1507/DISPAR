#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
    HANDLE hPipe;
    const wchar_t* pipeName = L"\\\\.\\pipe\\Pipe";
    char buffer[1000000];
    DWORD dwRead;

    hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << "Failed to create named pipe: " << GetLastError() << endl;
        return 1;
    }

    cout << "Named pipe server started." << endl;

    if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
        while (true) {
            ZeroMemory(buffer, sizeof(buffer));
            if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) != FALSE) {
                cout << "Received message: " << buffer << endl;

                if (WriteFile(hPipe, buffer, dwRead, NULL, NULL) == FALSE) {
                    cout << "Failed to write to pipe: " << GetLastError() << endl;
                    break;
                }
            }
        }
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    return 0;
}
