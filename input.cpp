#include "render.cpp"

void CBoard::Input()
{
    int DropMult;
    time_point<system_clock, milliseconds> DASDelay, DropDelay, CurrentTickTime;
    int DASLag = 0, DropLag = 0, CycleCount = 0;
    bool LDAS = false, RDAS = false, LeftHeld = false, RightHeld = false, CanLeft = true, CanRight = true;
    bool Left = true, Right = true;

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
                    DASLag = 0;
                }
                if(Left)
                {
                    MoveLeft();
                    Left = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!LDAS)
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - DASLag)
                        {
                            DASLag = (CurrentTickTime - DASDelay).count() - 150 + DASLag;
                            LDAS = true;
                            Left = true;
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - DASLag)
                        {
                            DASLag = (CurrentTickTime - DASDelay).count() - 30 + DASLag;
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
                    DASLag = 0;
                }
                if(Right)
                {
                    MoveRight();
                    Right = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!RDAS)
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - DASLag)
                        {
                            DASLag = (CurrentTickTime - DASDelay).count() - 150 + DASLag;
                            RDAS = true;
                            Right = true;
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - DASLag)
                        {
                            DASLag = (CurrentTickTime - DASDelay).count() - 30 + DASLag;
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
        if(Phys.Drop)
        {
            MoveDown();
            Phys.Drop = false;
            DropDelay = CurrentTickTime;
        }else
        {
            DropMult = (19 * (bool) GetAsyncKeyState(VK_DOWN) + 1);
            if((CurrentTickTime - DropDelay).count() >= (Phys.DropSpeed / DropMult) - DropLag)
            {
                DropLag = (CurrentTickTime - DropDelay).count() - (Phys.DropSpeed / DropMult) + DropLag;
                if(DropLag > (Phys.DropSpeed / DropMult)){DropLag = (Phys.DropSpeed / DropMult);}
                Phys.Drop = true;
            }
        }
        AutoLock(0);
        if(GetAsyncKeyState(VK_SPACE))
        {
            if(Phys.HDrop)
            {
                HardDrop();
                Phys.HDrop = false;
            }
        }
        else
        {
            Phys.HDrop = true;
        }
        if(GetAsyncKeyState(VK_C) && CanHold)
        {
            Hold();
        }
        if(GetAsyncKeyState(VK_UP))
        {
            if(Phys.RCW)
            {
                RotatePiece(0);
                Phys.RCW = false;
            }
        }
        else
        {
            Phys.RCW = true;
        }
        if(GetAsyncKeyState(VK_X))
        {
            if(Phys.RCCW)
            {
                RotatePiece(1);
                Phys.RCCW = false;
            }    
        }
        else
        {
            Phys.RCCW = true;
        }
        WaitForSingleObject(Timer, INFINITE);
    }
    CancelWaitableTimer(Timer);
    CloseHandle(Timer);
}