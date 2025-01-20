#ifndef TETRISMENU
#define TETRISMENU

#include <Windows.h>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include "../headers/Constants.hpp"

namespace Tetris
{

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
            Position.y - 12,
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
                RenderArea.top + ((RenderArea.bottom - RenderArea.top) >> 1) - 12,
                Title.c_str(),
                Title.size()
            );
        }else
        {
            TextOut
            (
                hdc,
                RenderArea.left + ((RenderArea.right - RenderArea.left) >> 1),
                RenderArea.top + 6,
                Title.c_str(),
                Title.size()
            );
            TextOut
            (
                hdc,
                RenderArea.left + ((RenderArea.right - RenderArea.left) >> 1),
                RenderArea.top + 30,
                Value.c_str(),
                Value.size()
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
        SetTextAlign(hdc, TA_CENTER);
        SelectObject(hdc, Render::DefFont);
        SelectObject(hdc, GetStockObject(DC_BRUSH));

        Rectangle(hdc, 0, 0, RenderArea.right - RenderArea.left, RenderArea.bottom - RenderArea.top);

        for(int i = 0; i < Labels.size(); ++i)
        {
            Labels[i]->Render(hdc);
        }

        for(int i = 0; i < Buttons.size(); ++i)
        {
            Buttons[i]->Render(hdc);
        }
    }

    void EventLoop(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        switch(Msg)
        {
            case WM_KEYDOWN:
            {
                
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
                        Button::ActiveBtn = Buttons[i];
                        return;
                    }
                }
            }
            break;
            case WM_LBUTTONUP:
            {
                POINT Mouse = {
                    LOWORD(lParam) * 5 / CFG.WindowSize - RenderArea.left,
                    HIWORD(lParam) * 5 / CFG.WindowSize - RenderArea.top
                };
                if(PtInRect(&Button::ActiveBtn->RenderArea, Mouse))
                {
                    Button::ActiveBtn->OnClick();
                    Button::ActiveBtn = NULL;
                    PostMessage(hwnd, WM_PRINT, 0, 0);
                    return;
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