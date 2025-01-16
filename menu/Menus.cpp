#include "Classes.hpp"
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
    }

    Menu * Menu::CreateMainMenu()
    {
        Menu * M = new Menu(280, 100, 240, 390);
        Label * Label1 = new Label(L"0", 120, 10);

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
            BtnFunc::LabelIncrement,
            Label1
        ));
        M->Buttons.push_back(new Button
        (
            L"Options",
            M->RenderArea.left,
            M->RenderArea.top,
            30, 210,
            180, 60,
            BtnFunc::NumberIncrement
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
};