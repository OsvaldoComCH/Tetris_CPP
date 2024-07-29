/* Compile:
g++ main.cpp -o "Tetris_CPP.exe" -static -lpthread -std=c++11 -O2 -l gdi32 -static-libgcc -static-libstdc++
*/
#include "game.cpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CREATE:
        {
            HDC hdc = GetDC(hwnd);
            Player1.Ghdc = CreateCompatibleDC(hdc);
            Player1.DCBitmap = CreateCompatibleBitmap(hdc, 700, 700);
            SelectObject(Player1.Ghdc, Player1.DCBitmap);
            ReleaseDC(hwnd, hdc);
            Player1.RenderBkgd();
        }
        break;
        case WM_KILLFOCUS:
        {
            if(CBoard::Mode == 1)
            {
                Pause();
            }
        }
        break;
        case WM_SETFOCUS:
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT PS;
            HDC hdc = BeginPaint(hwnd, &PS);
            HBRUSH hb = CreateSolidBrush(RGB(128,128,128));
            FillRect(hdc, &PS.rcPaint, hb);
            DeleteObject(hb);
            EndPaint(hwnd, &PS);
            Player1.RenderFlags = 255;
            Player1.Render();
            RenderScreen();
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
        }
        break;
        case WM_KEYDOWN:
        {
            if(wParam == VK_ESCAPE)
            {
                if(CBoard::Mode == 1)
                {
                    Pause();
                }else
                if(CBoard::Mode == 2)
                {
                    Resume();
                }
            }
            if(wParam == VK_RETURN && CBoard::Mode == 3)
            {
                CBoard::Mode = 1;
                std::thread (CBoard::StartGame, &Player1).detach();
            }
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
        200, 0, 960, 735,
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
    
    CBoard::Mode = 1;
    std::thread (CBoard::StartGame, &Player1).detach();
    std::thread (RenderThread).detach();

    FreeConsole();
    while(GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
