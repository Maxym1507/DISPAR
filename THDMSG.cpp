#include <windows.h>
#include <process.h>

// ідентифікатори потоків
DWORD dwThreadId1;
DWORD dwThreadId2;

// функції обробки повідомлень для вікон
LRESULT CALLBACK WndProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProc(LPVOID lpParameter);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Реєструємо клас першого вікна
    WNDCLASSEX wc1 = { 0 };
    wc1.cbSize = sizeof(WNDCLASSEX);
    wc1.lpfnWndProc = WndProc1;
    wc1.hInstance = hInstance;
    wc1.lpszClassName = L"WindowClass1";
    RegisterClassEx(&wc1);

    // Реєструємо клас другого вікна
    WNDCLASSEX wc2 = { 0 };
    wc2.cbSize = sizeof(WNDCLASSEX);
    wc2.lpfnWndProc = WndProc2;
    wc2.hInstance = hInstance;
    wc2.lpszClassName = L"WindowClass2";
    RegisterClassEx(&wc2);

    // Створюємо перше вікно
    HWND hwnd1 = CreateWindowEx(
        0,
        L"WindowClass1",
        L"Window 1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Створюємо друге вікно
    HWND hwnd2 = CreateWindowEx(
        0,
        L"WindowClass2",
        L"Window 2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd1 || !hwnd2) {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Відображаємо обидва вікна
    ShowWindow(hwnd1, nCmdShow);
    ShowWindow(hwnd2, nCmdShow);

    // Отримуємо ідентифікатор потоку для першого вікна
    dwThreadId1 = GetWindowThreadProcessId(hwnd1, NULL);

    // Отримуємо ідентифікатор потоку для другого вікна
    dwThreadId2 = GetWindowThreadProcessId(hwnd2, NULL);

    // Запускаємо два потоки, які будуть виконувати обробку повідомлень вікон
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, hwnd1, 0, &dwThreadId1);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc, hwnd2, 0, &dwThreadId2);
    if (!hThread1 || !hThread2) {
        MessageBox(NULL, L"Failed to create thread", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    WaitForSingleObject(hThread1, 2000);
    WaitForSingleObject(hThread2, 2000);

    return 0;
}

// Функція обробки повідомлень для вікна 1
LRESULT CALLBACK WndProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        MessageBox(hwnd, L"Window 1 was created", L"Info", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Функція обробки повідомлень для вікна 2
LRESULT CALLBACK WndProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        MessageBox(hwnd, L"Window 2 was created", L"Info", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Функція, яка буде виконуватися в окремому потоці
DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    HWND hwnd = (HWND)lpParameter;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}