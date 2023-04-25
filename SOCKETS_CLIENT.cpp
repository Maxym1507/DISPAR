#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Can't create socket! Quitting" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    int connResult = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cerr << "Can't connect to server! Quitting" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buf[4096];
    string userInput;

    do {
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0) {
            int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(clientSocket, buf, 4096, 0);
                if (bytesReceived > 0) {
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    } while (userInput.size() > 0);

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}