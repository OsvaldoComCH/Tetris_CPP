#include "render.cpp"

void CBoard::Input()
{
    Mode = 1;
    time_point<system_clock, milliseconds> DASDelay, DropDelay, CurrentTickTime;
    HANDLE Timer = CreateWaitableTimer(NULL, false, NULL);
    LARGE_INTEGER DueTime;
    DueTime.QuadPart = -160000;
    SetWaitableTimerEx(Timer, &DueTime, 10, NULL, NULL, NULL, 0);

    while(Mode == 1)
    {
        CurrentTickTime = time_point_cast<milliseconds>(system_clock::now());
        if(GetAsyncKeyState(VK_LEFT))
        {
            if(Phys.CanLeft)
            {
                if(!Phys.LeftHeld)
                {
                    Phys.LeftHeld = true;
                    if(Phys.RightHeld)
                    {
                        Phys.CanRight = false;
                    }
                    Phys.DASLag = 0;
                }
                if(Phys.Left)
                {
                    MoveLeft();
                    Phys.Left = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!Phys.LDAS)
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 150 + Phys.DASLag;
                            Phys.LDAS = true;
                            Phys.Left = true;
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 30 + Phys.DASLag;
                            Phys.Left = true;
                        }
                    }
                }
            }
        }else
        {
            Phys.Left = true;
            Phys.LDAS = false;
            Phys.LeftHeld = false;
            Phys.CanRight = true;
            Phys.CanLeft = true;
        }
        if(GetAsyncKeyState(VK_RIGHT))
        {
            if(Phys.CanRight)
            {
                if(!Phys.RightHeld)
                {
                    Phys.RightHeld = true;
                    if(Phys.LeftHeld)
                    {
                        Phys.CanLeft = false;
                    }
                    Phys.DASLag = 0;
                }
                if(Phys.Right)
                {
                    MoveRight();
                    Phys.Right = false;
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(!Phys.RDAS)
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 150 + Phys.DASLag;
                            Phys.RDAS = true;
                            Phys.Right = true;
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 30 + Phys.DASLag;
                            Phys.Right = true;
                        }
                    }
                }
            }
        }else
        {
            Phys.Right = true;
            Phys.RDAS = false;
            Phys.RightHeld = false;
            Phys.CanLeft = true;
            Phys.CanRight = true;
        }
        if(Phys.Drop)
        {
            MoveDown();
            Phys.Drop = false;
            DropDelay = CurrentTickTime;
        }else
        {
            Phys.DropMult = Phys.DropSpeed / (19 * (bool) GetAsyncKeyState(VK_DOWN) + 1);
            if((CurrentTickTime - DropDelay).count() >= Phys.DropMult - Phys.DropLag)
            {
                Phys.DropLag = (CurrentTickTime - DropDelay).count() - Phys.DropMult + Phys.DropLag;
                if(Phys.DropLag > Phys.DropMult)
                {
                    Phys.DropLag = Phys.DropMult;
                }
                if(Phys.DropLag < 0){Phys.DropLag = 0;}
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
