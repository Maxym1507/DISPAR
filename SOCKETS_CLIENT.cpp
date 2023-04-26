#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <chrono>
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

    char buf[1000000];
    string userInput;

    double bytesSent = 0.0;
    double totalSeconds = 0.0;
    int i = 0;
    do {
        cout << "> ";
        //getline(cin, userInput);
        userInput = "";
        if (i < 10)
            for (int i = 0; i < 99000; i++) {
                userInput += "qwertyuiop";
            }
        else
            userInput = "";
        i++;

        if (userInput.size() > 0) {
            auto start = chrono::high_resolution_clock::now();

            int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
            auto end = chrono::high_resolution_clock::now();
            if (sendResult != SOCKET_ERROR) {
                ZeroMemory(buf, 1000000);
                int bytesReceived = recv(clientSocket, buf, 1000000, 0);
                if (bytesReceived > 0) {
                    
                    auto diff = end - start;
                    double seconds = chrono::duration_cast<chrono::microseconds>(diff).count() / 1000000.0;

                    bytesSent += userInput.size();
                    totalSeconds += seconds;

                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    } while (userInput.size() > 0);

    closesocket(clientSocket);
    WSACleanup();

    double transferRate = (bytesSent / 1024 / 1024) / totalSeconds;
    cout << "Data transfer rate: " << transferRate << " mb/sec" << endl;
    return 0;
}
