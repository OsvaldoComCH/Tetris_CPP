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
            if(CFG.WindowSize < 10)
            {
                ++CFG.WindowSize;
            }
            Label * Lbl = (Label *)Target;
            Lbl->Text.assign(std::to_wstring(CFG.WindowSize));
        }
        void DecreaseWndSize(void * Target)
        {
            if(CFG.WindowSize > 1)
            {
                --CFG.WindowSize;
            }
            Label * Lbl = (Label *)Target;
            Lbl->Text.assign(std::to_wstring(CFG.WindowSize));
        }
        void OpenOptionsMenu(void * Target)
        {
            MenuStack::OpenMenu(MenuType::OptionsMenu);
        }
        void CloseOptionsMenu(void * Target)
        {
            ReadConfigFile(&CFG);
            MenuStack::CloseMenu();
        }
        void SaveOptions(void * Target)
        {
            WriteConfigFile(&CFG);
            ApplyCfgChanges();
            MenuStack::CloseMenu();
        }
    }

    Menu * Menu::CreateMainMenu()
    {
        Menu * M = new Menu(280, 100, 240, 390);
        Label * Label1 = new Label(L"Tetris_CPP", 120, 15);

        M->Labels.push_back(Label1);

        M->Buttons.push_back(new Button
        (
            L"Start Level:",
            M->RenderArea.left,
            M->RenderArea.top,
            30, 30,
            180, 60,
            BtnFunc::NumberIncrement
        ));
        M->Buttons.push_back(new Button
        (
            L"Max Level:",
            M->RenderArea.left,
            M->RenderArea.top,
            30, 120,
            180, 60,
            BtnFunc::NumberIncrement
        ));
        M->Buttons.push_back(new Button
        (
            L"Options",
            M->RenderArea.left,
            M->RenderArea.top,
            30, 210,
            180, 60,
            BtnFunc::OpenOptionsMenu
        ));
        M->Buttons.push_back(new Button
        (
            L"Start Game",
            M->RenderArea.left,
            M->RenderArea.top,
            30, 300,
            180, 60,
            BtnFunc::NumberIncrement
        ));

        return M;
    }
    
    Menu * Menu::CreateOptionsMenu()
    {
        Menu * M = new Menu(90, 50, 620, 500);
        Label * WndSizeLbl = new Label(std::to_wstring(CFG.WindowSize).c_str(), 310, 385);

        M->Labels.push_back(WndSizeLbl);
        M->Labels.push_back(new Label(L"Options", 310, 30));
        M->Labels.push_back(new Label(L"Window Size:", 310, 350));

        std::wstring Key;
        GetVKNameW(CFG.Controls.Left, &Key);
        M->Buttons.push_back(new Button
        (
            L"Left:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            20, 100, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.Right, &Key);
        M->Buttons.push_back(new Button
        (
            L"Right:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            220, 100, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.SoftDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Soft Drop:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            420, 100, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.RCW, &Key);
        M->Buttons.push_back(new Button
        (
            L"Rotate CW:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            20, 180, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.RCCW, &Key);
        M->Buttons.push_back(new Button
        (
            L"Rotate CCW:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            220, 180, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.HardDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Hard Drop:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            420, 180, 180, 60, BtnFunc::CloseOptionsMenu
        ));
        GetVKNameW(CFG.Controls.Hold, &Key);
        M->Buttons.push_back(new Button
        (
            L"Hard Drop:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            220, 260, 180, 60, BtnFunc::CloseOptionsMenu
        ));

        M->Buttons.push_back(new Button
        (
            L"-", M->RenderArea.left, M->RenderArea.top,
            250, 370, 30, 30, BtnFunc::DecreaseWndSize,
            WndSizeLbl
        ));
        M->Buttons.push_back(new Button
        (
            L"+", M->RenderArea.left, M->RenderArea.top,
            340, 370, 30, 30, BtnFunc::IncreaseWndSize,
            WndSizeLbl
        ));


        M->Buttons.push_back(new Button
        (
            L"Go Back", M->RenderArea.left, M->RenderArea.top,
            120, 420, 180, 60, BtnFunc::CloseOptionsMenu
        ));

        M->Buttons.push_back(new Button
        (
            L"Save", M->RenderArea.left, M->RenderArea.top,
            320, 420, 180, 60, BtnFunc::SaveOptions
        ));

        return M;
    }
};