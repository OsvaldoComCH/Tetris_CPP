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
        Phys.CurrentTickTime = time_point_cast<milliseconds>(system_clock::now());
        if(GetAsyncKeyState(VK_LEFT))
        {
            if(Phys.LRFlags & 8)//CanLeft
            {
                if(~Phys.LRFlags & 32)//!LeftHeld
                {
                    Phys.LRFlags |= 32;//LeftHeld = true
                    if(Phys.LRFlags & 16)//RightHeld
                    {
                        Phys.LRFlags &= ~4;//CanRight = true
                    }
                    Phys.DASLag = 0;
                }
                if(Phys.LRFlags & 2)//Left
                {
                    MoveLeft();
                    Phys.LRFlags &= ~2;//Left = false
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(~Phys.LRFlags & 128)//!LDAS
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 150 + Phys.DASLag;
                            Phys.LRFlags |= 128;//LDAS = true
                            Phys.LRFlags |= 2;//Left = true
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 30 + Phys.DASLag;
                            Phys.LRFlags |= 2;//Left = true
                        }
                    }
                } 
            }
        }else
        {
            Phys.LRFlags |= 2;//Left = true
            Phys.LRFlags &= ~128;//LDAS = false
            Phys.LRFlags &= ~32;//LeftHeld = false
            Phys.LRFlags |= 4;//CanRight = true
            Phys.LRFlags |= 8;//CanLeft = true
        }
        if(GetAsyncKeyState(VK_RIGHT))
        {
            if(Phys.LRFlags & 4)//CanRight
            {
                if(~Phys.LRFlags & 16)//!RightHeld
                {
                    Phys.LRFlags |= 16;//RightHeld = true
                    if(Phys.LRFlags & 32)//LeftHeld
                    {
                        Phys.LRFlags &= ~8;//CanLeft = false
                    }
                    Phys.DASLag = 0;
                }
                if(Phys.LRFlags & 1)//Right
                {
                    MoveRight();
                    Phys.LRFlags &= ~1;//Right = false
                    DASDelay = CurrentTickTime;
                }else
                {
                    if(~Phys.LRFlags & 64)//!RDAS
                    {
                        if((CurrentTickTime - DASDelay).count() >= 150 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 150 + Phys.DASLag;
                            Phys.LRFlags |= 64;//RDAS = true
                            Phys.LRFlags |= 1;//Right = true
                        }
                    }else
                    {
                        if((CurrentTickTime - DASDelay).count() >= 30 - Phys.DASLag)
                        {
                            Phys.DASLag = (CurrentTickTime - DASDelay).count() - 30 + Phys.DASLag;
                            Phys.LRFlags |= 1;//Right = true
                        }
                    }
                }
            }
        }else
        {
            Phys.LRFlags |= 1;//Right = true
            Phys.LRFlags &= ~64;//RDAS = false
            Phys.LRFlags &= 16;//RightHeld = false
            Phys.LRFlags |= 8;//CanLeft = true
            Phys.LRFlags |= 4;//CanRight = true
        }
        if(Phys.Drop)
        {
            MoveDown();
            Phys.Drop = false;
            DropDelay = CurrentTickTime;
        }else
        {
            Phys.DropMult = (19 * (bool) GetAsyncKeyState(VK_DOWN) + 1);
            if((CurrentTickTime - DropDelay).count() >= (Phys.DropSpeed / Phys.DropMult) - Phys.DropLag)
            {
                Phys.DropLag = (CurrentTickTime - DropDelay).count() - (Phys.DropSpeed / Phys.DropMult) + Phys.DropLag;
                if(Phys.DropLag > (Phys.DropSpeed / Phys.DropMult)){Phys.DropLag = (Phys.DropSpeed / Phys.DropMult);}
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
