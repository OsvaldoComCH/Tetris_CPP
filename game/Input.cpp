#include <thread>
#include <iostream>

#include "Classes.hpp"
#include "../menu/Classes.hpp"

namespace Tetris::Game
{

void Board::Input(time_milli TickTime)
{
    using namespace std::chrono;

    if(GetAsyncKeyState(CFG.Controls.HardDrop) & 0x8000)
    {
        if(PFlags.Get(PhysFlags::HardDrop))
        {
            HardDrop();
            PFlags.Unset(PhysFlags::HardDrop);
        }
    }else
    {
        PFlags.Set(PhysFlags::HardDrop);
    }

    if(GetAsyncKeyState(CFG.Controls.Left) & 0x8000)
    {
        if(PFlags.Get(PhysFlags::CanLeft))
        {
            if(!PFlags.Get(PhysFlags::LeftHeld))
            {
                PFlags.Set(PhysFlags::LeftHeld);
                if(PFlags.Get(PhysFlags::RightHeld))
                {
                    PFlags.Unset(PhysFlags::CanRight);
                }
                Phys.DASLag = 0;
            }
            if(PFlags.Get(PhysFlags::Left))
            {
                MoveLeft();
                PFlags.Unset(PhysFlags::Left | PhysFlags::TSpin | PhysFlags::TSpinMini);
                Phys.DASDelay = TickTime;
            }else
            {
                if(!PFlags.Get(PhysFlags::LDAS))
                {
                    if((TickTime - Phys.DASDelay).count() >= 150 - Phys.DASLag)
                    {
                        Phys.DASLag = (TickTime - Phys.DASDelay).count() - 150 + Phys.DASLag;
                        PFlags.Set(PhysFlags::LDAS);
                        PFlags.Set(PhysFlags::Left);
                    }
                }else
                {
                    if((TickTime - Phys.DASDelay).count() >= 30 - Phys.DASLag)
                    {
                        Phys.DASLag = (TickTime - Phys.DASDelay).count() - 30 + Phys.DASLag;
                        PFlags.Set(PhysFlags::Left);
                    }
                }
            }
        }
    }else
    {
        PFlags.Set(PhysFlags::Left);
        PFlags.Unset(PhysFlags::LDAS);
        PFlags.Unset(PhysFlags::LeftHeld);
        PFlags.Set(PhysFlags::CanRight);
        PFlags.Set(PhysFlags::CanLeft);
    }
    
    if(GetAsyncKeyState(CFG.Controls.Right) & 0x8000)
    {
        if(PFlags.Get(PhysFlags::CanRight))
        {
            if(!PFlags.Get(PhysFlags::RightHeld))
            {
                PFlags.Set(PhysFlags::RightHeld);
                if(PFlags.Get(PhysFlags::LeftHeld))
                {
                    PFlags.Unset(PhysFlags::CanLeft);
                }
                Phys.DASLag = 0;
            }
            if(PFlags.Get(PhysFlags::Right))
            {
                MoveRight();
                PFlags.Unset(PhysFlags::Right | PhysFlags::TSpin | PhysFlags::TSpinMini);
                Phys.DASDelay = TickTime;
            }else
            {
                if(!PFlags.Get(PhysFlags::RDAS))
                {
                    if((TickTime - Phys.DASDelay).count() >= 150 - Phys.DASLag)
                    {
                        Phys.DASLag = (TickTime - Phys.DASDelay).count() - 150 + Phys.DASLag;
                        PFlags.Set(PhysFlags::RDAS);
                        PFlags.Set(PhysFlags::Right);
                    }
                }else
                {
                    if((TickTime - Phys.DASDelay).count() >= 30 - Phys.DASLag)
                    {
                        Phys.DASLag = (TickTime - Phys.DASDelay).count() - 30 + Phys.DASLag;
                        PFlags.Set(PhysFlags::Right);
                    }
                }
            }
        }
    }else
    {
        PFlags.Set(PhysFlags::Right);
        PFlags.Unset(PhysFlags::RDAS);
        PFlags.Unset(PhysFlags::RightHeld);
        PFlags.Set(PhysFlags::CanLeft);
        PFlags.Set(PhysFlags::CanRight);
    }

    if(PFlags.Get(PhysFlags::Drop))
    {
        MoveDown();
        while(Phys.DropLag >= Phys.DropSpeed[Phys.DownHeld])
        {
            if(MoveDown())
            {
                Phys.DropLag %= Phys.DropSpeed[Phys.DownHeld];
                break;
            }
            Phys.DropLag -= Phys.DropSpeed[Phys.DownHeld];
        }
        PFlags.Unset(PhysFlags::Drop | PhysFlags::TSpin | PhysFlags::TSpinMini);
        Phys.DropDelay = TickTime;
    }else
    {
        Phys.DownHeld = GetAsyncKeyState(CFG.Controls.SoftDrop) & 0x8000;
        if(Phys.DownHeld ^ Phys.PrevDown)
        {
            Phys.PrevDown = Phys.DownHeld;
            Phys.DropLag = 0;
            PFlags.Set(PhysFlags::Drop);
        }else
        {
            if(duration_cast<microseconds>(TickTime - Phys.DropDelay).count()
            >= Phys.DropSpeed[Phys.DownHeld] - Phys.DropLag)
            {
                Phys.DropLag = duration_cast<microseconds>(TickTime - Phys.DropDelay).count()
                - Phys.DropSpeed[Phys.DownHeld] + Phys.DropLag;
                PFlags.Set(PhysFlags::Drop);
                if(Phys.DropLag < 0){Phys.DropLag = 0;}
            }
        }
    }

    AutoLock(TickTime);

    if(GetAsyncKeyState(CFG.Controls.Hold) & 0x8000 && CanHold)
    {
        Hold();
    }

    if(GetAsyncKeyState(CFG.Controls.RCW) & 0x8000)
    {
        if(PFlags.Get(PhysFlags::RCW))
        {
            RotatePiece(0);
            PFlags.Unset(PhysFlags::RCW);
        }
    }
    else
    {
        PFlags.Set(PhysFlags::RCW);
    }

    if(GetAsyncKeyState(CFG.Controls.RCCW) & 0x8000)
    {
        if(PFlags.Get(PhysFlags::RCCW))
        {
            RotatePiece(1);
            PFlags.Unset(PhysFlags::RCCW);
        }    
    }
    else
    {
        PFlags.Set(PhysFlags::RCCW);
    }

    Render();
}

bool RunInputThread = true;

void Input1Player()
{
    using namespace std::chrono;
    time_milli CurrentTick = time_point_cast<milliseconds>(system_clock::now());

    Board::AllBoards[0]->Input(CurrentTick);

    if(!Board::AllBoards[0]->Alive)
    {
        MenuStack::OpenMenu(GameOverMenu);
    }
    PostMessage(hwnd, WM_PRINT, 0, 0);
}

void (*InputFunc)() = Input1Player;

void Input()
{
    using namespace std::chrono;
    HANDLE Timer = CreateWaitableTimer(NULL, false, NULL);

    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -160000;
    SetWaitableTimerEx(Timer, &DueTime, 15, NULL, NULL, NULL, 0);

    while(RunInputThread)
    {
        if(!MenuStack::CurMenu)
        {
            InputFunc();
        }
        WaitForSingleObject(Timer, INFINITE);
    }
}

std::thread * InputThread = NULL;

void LaunchInputThread()
{
    if(InputThread){return;}
    RunInputThread = true;
    InputThread = new std::thread(Input);
}

void JoinInputThread()
{
    RunInputThread = false;
    InputThread->join();
    delete InputThread;
    InputThread = NULL;
}

}