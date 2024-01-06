#include "render.cpp"

void Delay(int Time, bool *Key)
{
    *Key = false;
    Sleep(Time);
    *Key = true;
}
void Input()
{
    bool Down = true, Left = true, Right = true, Drop = true, RCW = true, RCCW = true, AutoDrop = true; DAS = true;
    while(1)
    {
        if(GetAsyncKeyState(VK_DOWN) && Down)
        {
            Player.MoveDown();
            std::thread (Delay, 50, &Down).detach();
        }else
        if(AutoDrop)
        {
            Player.MoveDown();
            std::thread (Delay, 1000, &AutoDrop).detach();
        }
        /*if(GetAsyncKeyState(VK_LEFT) && Left && Right)
        {
            Player.MoveLeft();
            std::thread (Delay, 200, &Left).detach();
        }
        if(GetAsyncKeyState(VK_RIGHT) && Right && Left)
        {
            Player.MoveRight();
            std::thread (Delay, 200, &Right).detach();
        }*/
        if(GetAsyncKeyState(VK_SPACE))
        {
            if(Drop)
            {
                Player.HardDrop();
                Drop = false;
            }
        }
        else
        {
            Drop = true;
        }
        if(GetAsyncKeyState(VK_UP))
        {
            if(RCW)
            {
                Player.RotatePiece(0);
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
                Player.RotatePiece(1);
                RCCW = false;
            }    
        }
        else
        {
            RCCW = true;
        }
        Sleep(20);
    }
}