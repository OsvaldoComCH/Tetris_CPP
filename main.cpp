/* Compile:
g++ main.cpp -o "Tetris CPP.exe" -static -lpthread -std=c++11 -O2 -l gdi32 -static-libgcc -static-libstdc++
*/
#include "game.cpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_KILLFOCUS:
            Player1.Pause();
        break;
        case WM_SETFOCUS:
            if(Player1.Mode == 2)
            {
                Player1.Resume();
                std::thread (CBoard::Input, &Player1).detach();
            }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT PS;
            HDC hdc = BeginPaint(hwnd, &PS);
            Player1.Ghdc = CreateCompatibleDC(hdc);
            Player1.DCBitmap = CreateCompatibleBitmap(hdc, 700, 700);
            SelectObject(Player1.Ghdc, Player1.DCBitmap);
            DeleteObject()
            Player1.RenderBkgd(hdc);
            EndPaint(hwnd, &PS);
        }
        break;
        case WM_CLOSE:
        {
            DeleteDC(Player1.Ghdc);
            DeleteObject(Player1.DCBitmap);

            DeleteObject(Font);
            DeleteObject(Font2);
            for(int i = 0; i < 9; ++i)
            {
                DeleteObject(Pens[i]);
            }
            DestroyWindow(hwnd);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_SIZE:
        {
            Ghwnd = hwnd;
            InvalidateRect(hwnd, NULL, 1);
            Player1.GetMatrixPos();
        }
        break;
        default:
            return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t WClassName[]  = L"WindowClass";
    MSG Msg;
    WNDCLASS Window = {};
    Window.lpfnWndProc = WndProc;
    Window.hInstance = hInstance;
    Window.lpszClassName = WClassName;

    if(!RegisterClass(&Window))
    {
        MessageBox(NULL, L"Window Class Registration Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowEx
    (
        0,
        WClassName,
        L"Tetris",
        WS_OVERLAPPEDWINDOW,
        200, 10, 960, 720,
        NULL, NULL, hInstance, NULL
    );
    Ghwnd = hwnd;
    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    std::thread TetrisGame(CBoard::StartGame, &Player1);

    FreeConsole();
    while(GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
