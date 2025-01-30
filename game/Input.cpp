#include <thread>

#include "Game.cpp"
#include "../menu/Menus.cpp"

namespace Tetris::Game
{

void Board::Input(time_milli TickTime)
{
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
                PFlags.Unset(PhysFlags::Left);
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
                PFlags.Unset(PhysFlags::Right);
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
        bool DownHeld = PFlags.Get(PhysFlags::DownHeld);
        while(Phys.DropLag >= Phys.DropSpeed[DownHeld])
        {
            if(MoveDown())
            {
                Phys.DropLag %= Phys.DropSpeed[DownHeld];
                break;
            }
            Phys.DropLag -= Phys.DropSpeed[DownHeld];
        }
    }else
    {
        PFlags.SetState(PhysFlags::DownHeld, GetAsyncKeyState(CFG.Controls.SoftDrop) & 0x8000);
        
    }
}

void Input()
{
    using namespace std::chrono;
    HANDLE Timer = CreateWaitableTimer(NULL, false, NULL);

    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -160000;
    SetWaitableTimerEx(Timer, &DueTime, 15, NULL, NULL, NULL, 0);

    while(1)
    {
        if(!MenuStack::CurMenu)
        {
            time_milli CurrentTick = time_point_cast<milliseconds>(system_clock::now());
            for(int i = 0; i < Board::AllBoards.size(); ++i)
            {
                Board::AllBoards[i]->Input(CurrentTick);
            }
        }
        WaitForSingleObject(Timer, INFINITE);
    }
}

std::thread InputThread(Input);

}