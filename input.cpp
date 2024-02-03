#include "render.cpp"

void CPlayer::Input()
{
    bool Left = true, Right = true, HDrop = true, RCW = true, RCCW = true, Drop = true;
    bool LDAS = false, RDAS = false;
    bool LeftHeld = false, RightHeld = false, CanLeft = true, CanRight = true;
    int DropSpeed = 1000;//Auto-drop interval in milliseconds
    time_point<system_clock, milliseconds> DASDelay, DropDelay, CurrentTickTime;

    HANDLE Timer = CreateWaitableTimer(NULL, false, NULL);
    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -160000;
    SetWaitableTimerEx(Timer, &DueTime, 10, NULL, NULL, NULL, 0);
    while(1)
    {
        CurrentTickTime = time_point_cast<milliseconds>(system_clock::now());
        if(GetAsyncKeyState(VK_LEFT))
        {
            if(CanLeft)
            {
                if(!LeftHeld)
                {
                    LeftHeld = true;
                    if(RightHeld)
                    {
                        CanRight = false;
                    }
                }
                if(Left)
                {
                    Board.MoveLeft();
                    Left = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!LDAS)
                    {
                        if(DASDelay + (milliseconds) 150 <= CurrentTickTime)
                        {
                            LDAS = true;
                            Left = true;
                        }
                    }else
                    {
                        if(DASDelay + (milliseconds) 30 <= CurrentTickTime)
                        {
                            Left = true;
                        }
                    }
                }
            }
        }else
        {
            Left = true;
            LDAS = false;
            LeftHeld = false;
            CanRight = true;
            CanLeft = true;
        }
        if(GetAsyncKeyState(VK_RIGHT))
        {
            if(CanRight)
            {
                if(!RightHeld)
                {
                    RightHeld = true;
                    if(LeftHeld)
                    {
                        CanLeft = false;
                    }
                }
                if(Right)
                {
                    Board.MoveRight();
                    Right = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!RDAS)
                    {
                        if(DASDelay + (milliseconds) 150 <= CurrentTickTime)
                        {
                            RDAS = true;
                            Right = true;
                        }
                    }else
                    {
                        if(DASDelay + (milliseconds) 30 <= CurrentTickTime)
                        {
                            Right = true;
                        }
                    }
                }
            }
        }else
        {
            Right = true;
            RDAS = false;
            RightHeld = false;
            CanLeft = true;
            CanRight = true;
        }
        if(Drop)
        {
            Board.MoveDown();
            Drop = false;
            DropDelay = CurrentTickTime;
        }else
        {
            if(DropDelay + (milliseconds) DropSpeed / (19 * (bool) GetAsyncKeyState(VK_DOWN) + 1) 
            <= CurrentTickTime)
            {
                Drop = true;
            }
        }
        Board.AutoLock(0);
        if(GetAsyncKeyState(VK_SPACE))
        {
            if(HDrop)
            {
                Board.HardDrop();
                HDrop = false;
            }
        }
        else
        {
            HDrop = true;
        }
        if(GetAsyncKeyState(VK_C) && Board.CanHold)
        {
            Board.Hold();
        }
        if(GetAsyncKeyState(VK_UP))
        {
            if(RCW)
            {
                Board.RotatePiece(0);
                RCW = false;
            }
        }
        else
        {
            RCW = true;
        }
        if(GetAsyncKeyState(VK_X))
        {
            if(RCCW)
            {
                Board.RotatePiece(1);
                RCCW = false;
            }    
        }
        else
        {
            RCCW = true;
        }
        WaitForSingleObject(Timer, INFINITE);
    }
    CancelWaitableTimer(Timer);
    CloseHandle(Timer);
}