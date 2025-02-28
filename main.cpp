#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <fstream>
#include <iostream>
#include "headers/Constants.hpp"
#include "menu/Menus.cpp"
#include "render/Classes.hpp"
#include "game/Classes.hpp"
#include "game/Game.cpp"
#include "game/Input.cpp"
#include "game/Render.cpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    using namespace Tetris;
    if(MenuStack::CurMenu)
    {
        MenuStack::CurMenu->EventLoop(hwnd, Msg, wParam, lParam);
    }else
    {
        Game::EventLoop(hwnd, Msg, wParam, lParam);
    }
    switch(Msg)
    {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            Game::JoinInputThread();
            MenuStack::CloseMenu();
        }
        break;
        case WM_CREATE:
            Tetris::Render::InitResources();
            Tetris::Render::InitLayers();
            Tetris::Render::CreateLayer(0, 0, 800, 600); //Background Layer (0)
            MenuStack::OpenMenu(MenuType::MainMenu);
            //Game::LaunchInputThread();
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            Tetris::Render::RenderBkgd();
            if(MenuStack::CurMenu)
            {
                MenuStack::CurMenu->Render();
            }
            Tetris::Render::TransferAllLayers();
            Tetris::Render::TransferToWindow();
        }
        break;
        case WM_PRINT:
        {
            Tetris::Render::RenderBkgd();
            if(MenuStack::CurMenu)
            {
                MenuStack::CurMenu->Render();
            }
            Tetris::Render::TransferAllLayers();
            Tetris::Render::TransferToWindow();
        }
        break;
        case WM_DESTROY:
        {
            Render::FreeResources();
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
    
    using namespace Tetris;
    ReadConfigFile(&CFG);

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
    int w = 160 * CFG.WindowSize;
    int h = 120 * CFG.WindowSize;

    //Center window on screen
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) >> 1;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) >> 1;

    //Adjust window size for desired client area
    RECT Area = {x, y, x + w, y + h};
    AdjustWindowRectEx(&Area, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), false, 0);

    w = Area.right - Area.left;
    h = Area.bottom - Area.top;
    if(Area.left < 0){Area.left = 0;}
    if(Area.top < 0){Area.top = 0;}

    hwnd = CreateWindowEx
    (
        0,
        WClassName,
        L"Tetris_CPP",
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
        Area.left, Area.top, w, h,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    FreeConsole();
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
