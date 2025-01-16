#ifndef TETRISMENU
#define TETRISMENU

#include <Windows.h>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include "../headers/Config.hpp"

Tetris::Config CFG;

namespace Tetris
{

namespace Render
{
    HFONT DefFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

    void TetrisBlt(HDC DestDC, HDC SrcDC, RECT TransferArea)
    {
        SetStretchBltMode(DestDC, HALFTONE);

        float Scale = ((float)CFG.WindowSize) / 5;
        StretchBlt
        (
            DestDC,
            TransferArea.left * Scale,
            TransferArea.top * Scale,
            (TransferArea.right - TransferArea.left) * Scale,
            (TransferArea.bottom - TransferArea.top) * Scale,
            SrcDC,
            0, 0,
            TransferArea.right - TransferArea.left,
            TransferArea.bottom - TransferArea.top,
            SRCCOPY
        );
    }
};

class RenderObject
{
    public:
    RECT RenderArea;
    POINT ParentOrigin;
};

class Label
{
    public:
    POINT Position;
    std::wstring Text;

    Label(){}
    Label(const wchar_t * Text, int x, int y)
    {
        Position = {x, y};
        this->Text = Text;
    }

    void Render(HDC hdc)
    {
        TextOut
        (
            hdc,
            Position.x,
            Position.y - 14,
            Text.c_str(),
            Text.size()
        );
    }
};

class Button : public RenderObject
{
    private:
    void (*_OnClick)(void *) = [](void * Target){};
    void (*_OnKeyPress)(void *, int) = NULL;

    public:
    static Button * ActiveBtn;
    std::wstring Title;
    std::wstring Value;
    void * Target = this;

    void Render(HDC hdc)
    {
        Rectangle
        (
            hdc,
            RenderArea.left,
            RenderArea.top,
            RenderArea.right,
            RenderArea.bottom
        );
        if(Value.empty())
        {
            TextOut
            (
                hdc,
                RenderArea.left + ((RenderArea.right - RenderArea.left) >> 1),
                RenderArea.top + ((RenderArea.bottom - RenderArea.top) >> 1) - 14,
                Title.c_str(),
                Title.size()
            );
        }else
        {
            TextOut
            (
                hdc,
                RenderArea.left + ((RenderArea.right - RenderArea.left) >> 1),
                RenderArea.top + ((RenderArea.bottom - RenderArea.top) >> 1) - 14,
                (Title + L" " + Value).c_str(),
                Title.size() + Value.size() + 1
            );
        }
    }

    void OnClick()
    {
        return _OnClick(Target);
    }
    void OnKeyPress(int Key)
    {
        if(_OnKeyPress)
        {
            return _OnKeyPress(Target, Key);
        }
    }

    void Activate()
    {
        ActiveBtn = this;
    }
    static void Deactivate()
    {
        ActiveBtn = NULL;
    }

    Button(){}

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(void *))
    : Button()
    {
        this->Title = Title;
        RenderArea = {x, y, x+w, y+h};
        ParentOrigin = {ParentX, ParentY};
        this->_OnClick = OnClick;
    }

    Button(const wchar_t * Title, const wchar_t * Value, int ParentX, int ParentY,
    int x, int y, int w, int h, void (*OnClick)(void *))
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->Value = Value;
    }

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(void *), void (*OnKeyPress)(void *, int))
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->_OnKeyPress = OnKeyPress;
    }

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(void *), void * Target)
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->Target = Target;
    }

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(void *), void (*OnKeyPress)(void *, int), void * Target)
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick, OnKeyPress)
    {
        this->Target = Target;
    }
};

enum MenuType
{
    MainMenu,
    PauseMenu,
    OptionsMenu
};

class Menu : public RenderObject
{
    private:
    static Menu * CreateMainMenu();
    static Menu * CreatePauseMenu(){return NULL;}
    static Menu * CreateOptionsMenu();
    
    std::wstring Title;

    public:
    std::vector<Button *> Buttons;
    std::vector<Label *> Labels;

    Menu(int x, int y, int w, int h)
    {
        RenderArea = {x, y, x+w, y+h};
        ParentOrigin = {0, 0};
    }

    void Render(HDC hdc)
    {
        HDC MenuDC = CreateCompatibleDC(hdc);
        HBITMAP Bmp = CreateCompatibleBitmap
        (
            hdc,
            RenderArea.right - RenderArea.left,
            RenderArea.bottom - RenderArea.top
        );
        SelectObject(MenuDC, Bmp);
        SetTextAlign(MenuDC, TA_CENTER);
        SelectObject(MenuDC, Render::DefFont);
        SelectObject(MenuDC, GetStockObject(DC_BRUSH));

        Rectangle(MenuDC, 0, 0, RenderArea.right - RenderArea.left, RenderArea.bottom - RenderArea.top);

        for(int i = 0; i < Labels.size(); ++i)
        {
            Labels[i]->Render(MenuDC);
        }

        for(int i = 0; i < Buttons.size(); ++i)
        {
            Buttons[i]->Render(MenuDC);
        }

        Render::TetrisBlt(hdc, MenuDC, RenderArea);

        DeleteObject(Bmp);
        DeleteDC(MenuDC);
    }

    void EventLoop(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        switch(Msg)
        {
            case WM_KEYDOWN:
            {
                Button::ActiveBtn->OnKeyPress(wParam);
            }
            break;
            case WM_LBUTTONDOWN:
            {
                POINT Mouse = {
                    LOWORD(lParam) * 5 / CFG.WindowSize - RenderArea.left,
                    HIWORD(lParam) * 5 / CFG.WindowSize - RenderArea.top
                };
                for(int i = 0; i < Buttons.size(); ++i)
                {
                    if(PtInRect(&Buttons[i]->RenderArea, Mouse))
                    {
                        Buttons[i]->OnClick();
                        PostMessage(hwnd, WM_PRINT, 0, 0);
                        return;
                    }
                }
            }
            break;
        }
    }

    static Menu * CreateMenu(MenuType Type)
    {
        switch(Type)
        {
            case MainMenu:
            return CreateMainMenu();
            case PauseMenu:
            return CreatePauseMenu();
            case OptionsMenu:
            return CreateOptionsMenu();
            default:
            return NULL;
        }
    }

    static void DestroyMenu(Menu * M)
    {
        for(int i = 0; i < M->Labels.size(); ++i)
        {
            delete M->Labels[i];
        }
        for(int i = 0; i < M->Buttons.size(); ++i)
        {
            delete M->Buttons[i];
        }
        delete M;
    }
};
}
#endif