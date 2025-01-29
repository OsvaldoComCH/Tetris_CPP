#include <thread>

#include "Game.cpp"
#include "../menu/Menus.cpp"

namespace Tetris::Game
{

void Board::Input()
{
    
}

void Input()
{
    HANDLE Timer = CreateWaitableTimer(NULL, false, NULL);

    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -160000;
    SetWaitableTimerEx(Timer, &DueTime, 15, NULL, NULL, NULL, 0);

    while(1)
    {
        if(!MenuStack::CurMenu)
        {
            for(int i = 0; i < Board::AllBoards.size(); ++i)
            {
                Board::AllBoards[i]->Input();
            }
        }
        WaitForSingleObject(Timer, INFINITE);
    }
}

std::thread InputThread(Input);

}