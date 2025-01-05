#ifndef TETRISMENU
#define TETRISMENU

#include <Windows.h>
#include <vector>
#include <set>
#include <string>
#include "../headers/Macros.hpp"
#include "../headers/Config.hpp"

Tetris::Config CFG;

namespace Tetris
{
class RenderObject
{
    public:
    RECT RenderArea;
    POINT ParentOrigin;
};

class Button : public RenderObject
{
    private:
    void (*_OnClick)(Button *) = [](Button * Btn){};
    void (*_OnKeyPress)(Button *, int) = NULL;

    public:
    static Button * ActiveBtn;
    std::wstring Title;
    std::wstring Value;

    void Render(HDC hdc)
    {

    }

    void OnClick()
    {
        return _OnClick(this);
    }
    void OnKeyPress(int Key)
    {
        if(_OnKeyPress)
        {
            return _OnKeyPress(this, Key);
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
    void (*OnClick)(Button *))
    : Button()
    {
        this->Title = Title;
        RenderArea = {x, y, x+w, y+h};
        ParentOrigin = {ParentX, ParentY};
        this->_OnClick = OnClick;
    }

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(Button *), void (*OnKeyPress)(Button *, int))
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->_OnKeyPress = OnKeyPress;
    }
};

enum MenuType
{
    Main,
    Pause,
    Controls
};

class Menu : public RenderObject
{
    private:
    static Menu * CreateMainMenu();
    static Menu * CreatePauseMenu(){return NULL;};
    static Menu * CreateControlsMenu(){return NULL;};
    
    std::wstring Title;

    public:
    std::vector<Button *> Buttons;

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
        for(int i = 0; i < Buttons.size(); ++i)
        {
            Buttons[i]->Render(MenuDC);
        }
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
                POINT Mouse = {LOWORD(lParam) * 5 / CFG.WindowSize, HIWORD(lParam) * 5 / CFG.WindowSize};
                for(int i = 0; i < Buttons.size(); ++i)
                {
                    if(PtInRect(&Buttons[i]->RenderArea, Mouse))
                    {
                        Buttons[i]->OnClick();
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
            case Main:
            return CreateMainMenu();
            case Pause:
            return CreatePauseMenu();
            case Controls:
            return CreateControlsMenu();
        }
    }

    static void DestroyMenu(Menu * M)
    {
        for(int i = 0; i < M->Buttons.size(); ++i)
        {
            delete M->Buttons[i];
        }
        delete M;
    }
};

namespace BtnFunc
{
    void NumberIncrement(Button * Btn)
    {
        static int Level = 1;
        ++Level;
        Btn->Value.assign(std::to_wstring(Level));
    }
}

Menu * Menu::CreateMainMenu()
{
    Menu * M = new Menu(300, 150, 200, 300);
    M->Buttons.push_back(new Button
    (
        L"Level: ",
        M->RenderArea.left,
        M->RenderArea.top,
        10, 120,
        180, 60,
        BtnFunc::NumberIncrement
    ));
    return M;
}

}
#endif