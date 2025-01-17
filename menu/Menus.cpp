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
Tetris::Menu * MenuStack::CurMenu;
Tetris::Button * Tetris::Button::ActiveBtn;

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
        void LabelIncrement(void * Target)
        {
            static int Level = 0;
            ++Level;

            Label * Lbl = (Label *)Target;
            Lbl->Text.assign(std::to_wstring(Level));
        }
        void OpenOptionsMenu(void * Target)
        {
            MenuStack::OpenMenu(MenuType::OptionsMenu);
        }
        void CloseCurrentMenu(void * Target)
        {
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
        Label * Label1 = new Label(L"Options", 250, 30);

        M->Labels.push_back(Label1);

        std::wstring Key;
        GetVKNameW(CFG.Controls.Left, &Key);
        M->Buttons.push_back(new Button
        (
            L"Left:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            20, 60, 180, 60, BtnFunc::CloseCurrentMenu
        ));
        GetVKNameW(CFG.Controls.Right, &Key);
        M->Buttons.push_back(new Button
        (
            L"Right:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            220, 60, 180, 60, BtnFunc::CloseCurrentMenu
        ));
        GetVKNameW(CFG.Controls.SoftDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Soft Drop:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            420, 60, 180, 60, BtnFunc::CloseCurrentMenu
        ));
        GetVKNameW(CFG.Controls.RCW, &Key);
        M->Buttons.push_back(new Button
        (
            L"Left:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            20, 140, 180, 60, BtnFunc::CloseCurrentMenu
        ));
        GetVKNameW(CFG.Controls.Right, &Key);
        M->Buttons.push_back(new Button
        (
            L"Right:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            220, 140, 180, 60, BtnFunc::CloseCurrentMenu
        ));
        GetVKNameW(CFG.Controls.SoftDrop, &Key);
        M->Buttons.push_back(new Button
        (
            L"Soft Drop:", Key.c_str(), M->RenderArea.left, M->RenderArea.top,
            420, 140, 180, 60, BtnFunc::CloseCurrentMenu
        ));


        M->Buttons.push_back(new Button
        (
            L"Go Back", M->RenderArea.left, M->RenderArea.top,
            220, 420, 180, 60, BtnFunc::CloseCurrentMenu
        ));

        return M;
    }
};