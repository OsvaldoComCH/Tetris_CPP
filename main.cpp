#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <fstream>
#include "headers/Config.hpp"
#include <iostream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
        {
            return DefWindowProc(hwnd, Msg, wParam, lParam);
        }
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    TetrisConfig Cfg;
    ReadConfigFile(&Cfg);

    const wchar_t * WClassName = L"WindowClass";
    WNDCLASS Window = {};
    Window.lpfnWndProc = WndProc;
    Window.hInstance = hInstance;
    Window.lpszClassName = WClassName;

    if(!RegisterClass(&Window))
    {
        MessageBox(NULL, L"Window Class Registration Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    //Get window client area
    int w = 160 * (Cfg.WindowSize + 1);
    int h = 120 * (Cfg.WindowSize + 1);

    //Center window on screen
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) >> 1;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) >> 1;

    //Adjust window size for desired client area
    RECT Area = {x, y, x + w, y + h};
    AdjustWindowRectEx(&Area, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), false, 0);

    HWND hwnd = CreateWindowEx
    (
        0,
        WClassName,
        L"Tetris_CPP",
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
        Area.left, Area.top, Area.right - Area.left, Area.bottom - Area.top,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return 0;
}