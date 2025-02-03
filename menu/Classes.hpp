#ifndef TETRISMENU
#define TETRISMENU

#include <Windows.h>
#include <vector>
#include <set>
#include <string>
#include <stack>
#include <iostream>
#include "../headers/Constants.hpp"
#include "../render/Classes.hpp"

namespace Tetris
{

class RenderObject
{
    public:
    RECT Area;
    POINT ParentOrigin;
};

class Label
{
    public:
    POINT Position;
    std::wstring Text;
    HFONT Font;

    Label(){}
    Label(const wchar_t * Text, int x, int y)
    {
        Position = {x, y};
        this->Text = Text;
        Font = Render::DefFont;
    }

    Label(const wchar_t * Text, int x, int y, HFONT Font)
    {
        Position = {x, y};
        this->Text = Text;
        this->Font = Font;
    }

    void Render(HDC hdc)
    {
        HGDIOBJ OldFont = SelectObject(hdc, Font);
        TextOut
        (
            hdc,
            Position.x,
            Position.y - 12,
            Text.c_str(),
            Text.size()
        );
        SelectObject(hdc, OldFont);
    }
};

class Button : public RenderObject
{
    private:
    void (*_OnClick)(void *) = [](void * Target){};
    void (*_OnKeyPress)(void *, int) = [](void * Target, int Key){};

    public:
    static Button * ActiveBtn;
    std::wstring Title;
    std::wstring Value;
    void * Target = this;
    bool Toggle = false;

    void Render(HDC hdc)
    {
        using namespace Render;
        Rectangle
        (
            hdc,
            Area.left,
            Area.top,
            Area.right,
            Area.bottom
        );
        COLORREF Color, Bk;
        if(ActiveBtn == this)
        {
            Color = SetDCBrushColor(hdc, Color::LightGray);
            Bk = SetBkColor(hdc, Color::LightGray);
        }else
        {
            Color = SetDCBrushColor(hdc, RGB(160,160,160));
            Bk = SetBkColor(hdc, RGB(160,160,160));
        }
        Rectangle
        (
            hdc,
            Area.left + 5,
            Area.top + 5,
            Area.right - 5,
            Area.bottom - 5
        );
        if(Value.empty())
        {
            TextOut
            (
                hdc,
                Area.left + ((Area.right - Area.left) >> 1),
                Area.top + ((Area.bottom - Area.top) >> 1) - 12,
                Title.c_str(),
                Title.size()
            );
        }else
        {
            TextOut
            (
                hdc,
                Area.left + ((Area.right - Area.left) >> 1),
                Area.top + 6,
                Title.c_str(),
                Title.size()
            );
            TextOut
            (
                hdc,
                Area.left + ((Area.right - Area.left) >> 1),
                Area.top + 30,
                Value.c_str(),
                Value.size()
            );
        }
        SetDCBrushColor(hdc, Color);
        SetBkColor(hdc, Bk);
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
        Area = {x, y, x+w, y+h};
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
    void (*OnClick)(void *), void * Target)
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->Target = Target;
    }
    
    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnKeyPress)(void *, int))
    {
        this->Title = Title;
        Area = {x, y, x+w, y+h};
        ParentOrigin = {ParentX, ParentY};
        this->_OnKeyPress = OnKeyPress;
    }
    
    Button(const wchar_t * Title, const wchar_t * Value, int ParentX, int ParentY,
    int x, int y, int w, int h, void (*OnKeyPress)(void *, int))
    : Button(Title, ParentX, ParentY, x, y, w, h, OnKeyPress)
    {
        this->Value = Value;
    }
    Button(const wchar_t * Title, const wchar_t * Value, int ParentX, int ParentY,
    int x, int y, int w, int h, bool Toggle, void (*OnKeyPress)(void *, int))
    : Button(Title, Value, ParentX, ParentY, x, y, w, h, OnKeyPress)
    {
        this->Toggle = Toggle;
    }

    Button(const wchar_t * Title, int ParentX, int ParentY, int x, int y, int w, int h,
    void (*OnClick)(void *), void (*OnKeyPress)(void *, int))
    : Button(Title, ParentX, ParentY, x, y, w, h, OnClick)
    {
        this->_OnKeyPress = OnKeyPress;
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
    static Menu * CreatePauseMenu();
    static Menu * CreateOptionsMenu();

    public:
    std::vector<Button *> Buttons;
    std::vector<Label *> Labels;

    Menu(int x, int y, int w, int h)
    {
        Area = {x, y, x+w, y+h};
        ParentOrigin = {0, 0};
        Render::CreateLayer(x, y, w, h);
    }
    ~Menu()
    {
        Render::DestroyLayer();
    }

    void Render()
    {
        using namespace Render;
        Layer * L = GetLayer();

        SetTextAlign(L->hdc, TA_CENTER);
        SelectObject(L->hdc, DefFont);
        SelectObject(L->hdc, GetStockObject(DC_BRUSH));
        SelectObject(L->hdc, GetStockObject(NULL_PEN));

        RenderBkgd(L, Color::White);
        SetDCBrushColor(L->hdc, Color::DarkGray);
        SetBkColor(L->hdc, Color::DarkGray);
        SetTextColor(L->hdc, Color::White);
        Rectangle(L->hdc, 5, 5, Area.right - Area.left - 5, Area.bottom - Area.top - 5);
        for(int i = 0; i < Labels.size(); ++i)
        {
            Labels[i]->Render(L->hdc);
        }

        SetTextColor(L->hdc, Color::Black);
        SetDCBrushColor(L->hdc, Color::White);
        for(int i = 0; i < Buttons.size(); ++i)
        {
            Buttons[i]->Render(L->hdc);
        }
    }

    void EventLoop(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        switch(Msg)
        {
            case WM_KEYDOWN:
            {
                if(Button::ActiveBtn)
                {
                    Button::ActiveBtn->OnKeyPress(wParam);
                    PostMessage(hwnd, WM_PRINT, 0, 0);
                    if(Button::ActiveBtn->Toggle)
                    {
                        Button::ActiveBtn = NULL;
                    }
                }
            }
            break;
            case WM_LBUTTONDOWN:
            {
                POINT Mouse = {
                    LOWORD(lParam) * 5 / CFG.WindowSize - Area.left,
                    HIWORD(lParam) * 5 / CFG.WindowSize - Area.top
                };
                if(Button::ActiveBtn && Button::ActiveBtn->Toggle)
                {
                    Button::ActiveBtn = NULL;
                    PostMessage(hwnd, WM_PRINT, 0, 0);
                }else
                {
                    for(int i = 0; i < Buttons.size(); ++i)
                    {
                        if(PtInRect(&Buttons[i]->Area, Mouse))
                        {
                            Button::ActiveBtn = Buttons[i];
                            PostMessage(hwnd, WM_PRINT, 0, 0);
                            return;
                        }
                    }
                }
            }
            break;
            case WM_LBUTTONUP:
            {
                POINT Mouse = {
                    LOWORD(lParam) * 5 / CFG.WindowSize - Area.left,
                    HIWORD(lParam) * 5 / CFG.WindowSize - Area.top
                };
                if(Button::ActiveBtn)
                {
                    if(!Button::ActiveBtn->Toggle)
                    {
                        if(PtInRect(&Button::ActiveBtn->Area, Mouse))
                        {
                            Button::ActiveBtn->OnClick();
                        }
                        Button::ActiveBtn = NULL;
                        PostMessage(hwnd, WM_PRINT, 0, 0);
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

class MenuStack
{
    public:
    static std::stack<Tetris::MenuType> Menus;
    static Tetris::Menu * CurMenu;
    static void Begin(){CurMenu = NULL;}
    static void OpenMenu(Tetris::MenuType Type)
    {
        Menus.push(Type);
        if(CurMenu)
        {
            Tetris::Menu::DestroyMenu(CurMenu);
        }
        CurMenu = Tetris::Menu::CreateMenu(Type);
    }
    static void CloseMenu()
    {
        if(CurMenu)
        {
            Tetris::Menu::DestroyMenu(CurMenu);
            Menus.pop();
            if(Menus.empty())
            {
                CurMenu = NULL;
                return;
            }
            CurMenu = Tetris::Menu::CreateMenu(Menus.top());
        }
    }
    static void CloseAll()
    {
        Tetris::Menu::DestroyMenu(CurMenu);
        CurMenu = NULL;
        Menus.empty();
    }
};

}

#endif