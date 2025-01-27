#include "Classes.hpp"
#include "../headers/Config.hpp"
#include <stack>

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
};

std::stack<Tetris::MenuType> MenuStack::Menus;
Tetris::Menu * MenuStack::CurMenu = NULL;
Tetris::Button * Tetris::Button::ActiveBtn = NULL;

namespace Tetris
{
    namespace BtnFunc
    {
        void NumberIncrement(void * Target)
        {
            static int Level = 0;
            ++Level;

            Button * Btn = (Button *)Target;
            Btn->Value.assign(std::to_wstring(Level));
        }
        void IncreaseWndSize(void * Target)
        {
            if(TempCFG.WindowSize < 10)
            {
                ++TempCFG.WindowSize;
            }
            Label * Lbl = (Label *)Target;
            Lbl->Text.assign(std::to_wstring(TempCFG.WindowSize));
        }
        void DecreaseWndSize(void * Target)
        {
            if(TempCFG.WindowSize > 1)
            {
                --TempCFG.WindowSize;
            }
            Label * Lbl = (Label *)Target;
            Lbl->Text.assign(std::to_wstring(TempCFG.WindowSize));
        }
        void OpenOptionsMenu(void * Target)
        {
            MenuStack::OpenMenu(MenuType::OptionsMenu);
        }
        void CloseOptionsMenu(void * Target)
        {
            MenuStack::CloseMenu();
        }
        void SaveOptions(void * Target)
        {
            ApplyCfgChanges();
        }
        int ReadKey(void * Target, int Key)
        {
            Button * Btn = (Button *) Target;
            return GetVKNameW(Key, &Btn->Value);
        }
    }

    Menu * Menu::CreateMainMenu()
    {
        Menu * M = new Menu(280, 100, 240, 390);

        M->Buttons.push_back(new Button
        (
            L"Start Level:",
            M->Area.left,
            M->Area.top,
            30, 30,
            180, 60,
            BtnFunc::NumberIncrement
        ));
        M->Buttons.push_back(new Button
        (
            L"Max Level:",
            M->Area.left,
            M->Area.top,
            30, 120,
            180, 60,
            BtnFunc::NumberIncrement
        ));
        M->Buttons.push_back(new Button
        (
            L"Options",
            M->Area.left,
            M->Area.top,
            30, 210,
            180, 60,
            BtnFunc::OpenOptionsMenu
        ));
        M->Buttons.push_back(new Button
        (
            L"Start Game",
            M->Area.left,
            M->Area.top,
            30, 300,
            180, 60,
            BtnFunc::NumberIncrement
        ));

        return M;
    }
    
    Menu * Menu::CreateOptionsMenu()
    {
        TempCFG = CFG;
        Menu * M = new Menu(90, 50, 620, 500);
        Label * WndSizeLbl = new Label(std::to_wstring(TempCFG.WindowSize).c_str(), 310, 385);

        M->Labels.push_back(WndSizeLbl);
        M->Labels.push_back(new Label(L"Options", 310, 40, Render::BigFont));
        M->Labels.push_back(new Label(L"Window Size:", 310, 350));

        std::wstring Key;
        GetVKNameW(CFG.Controls.Left, &Key);
        M->Buttons.push_back(new Button
        (
            L"Left:", Key.c_str(), M->Area.left, M->Area.top,
            20, 100, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.Left = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.Right, &Key);
        M->Buttons.push_back(new Button
        (
            L"Right:", Key.c_str(), M->Area.left, M->Area.top,
            220, 100, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.Right = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.SoftDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Soft Drop:", Key.c_str(), M->Area.left, M->Area.top,
            420, 100, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.SoftDrop = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.RCW, &Key);
        M->Buttons.push_back(new Button
        (
            L"Rotate CW:", Key.c_str(), M->Area.left, M->Area.top,
            20, 180, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.RCW = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.RCCW, &Key);
        M->Buttons.push_back(new Button
        (
            L"Rotate CCW:", Key.c_str(), M->Area.left, M->Area.top,
            220, 180, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.RCCW = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.HardDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Hard Drop:", Key.c_str(), M->Area.left, M->Area.top,
            420, 180, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.HardDrop = Key;
                }
            }
        ));
        GetVKNameW(CFG.Controls.Hold, &Key);
        M->Buttons.push_back(new Button
        (
            L"Hold:", Key.c_str(), M->Area.left, M->Area.top,
            220, 260, 180, 60, true,
            [](void * Target, int Key)
            {
                if(BtnFunc::ReadKey(Target, Key))
                {
                    TempCFG.Controls.Hold = Key;
                }
            }
        ));

        M->Buttons.push_back(new Button
        (
            L"-", M->Area.left, M->Area.top,
            250, 370, 35, 35, BtnFunc::DecreaseWndSize,
            WndSizeLbl
        ));
        M->Buttons.push_back(new Button
        (
            L"+", M->Area.left, M->Area.top,
            335, 370, 35, 35, BtnFunc::IncreaseWndSize,
            WndSizeLbl
        ));


        M->Buttons.push_back(new Button
        (
            L"Go Back", M->Area.left, M->Area.top,
            120, 420, 180, 60, BtnFunc::CloseOptionsMenu
        ));

        M->Buttons.push_back(new Button
        (
            L"Save", M->Area.left, M->Area.top,
            320, 420, 180, 60, BtnFunc::SaveOptions
        ));

        return M;
    }
};