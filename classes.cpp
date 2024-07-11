#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <string>
#include "./VirtualKeyCodes.h"
using namespace std;
using namespace std::chrono;

typedef signed char int8;
typedef struct SBlock
{
    int8 Pos[4][2];
} SBlock;

std::minstd_rand RNG (system_clock::now().time_since_epoch().count());
HWND Ghwnd; //Global handle to the window

class CPiece
{
    public:
    int8 Type;//1=I, 2=T, 3=O, 4=L, 5=J, 6=Z, 7=S
    unsigned Rotation :2;//0-up, 1-right, 2-down, 3-left
    int8 Position[2];//[X, Y]
    SBlock Block;
    struct Kick
    {
        int8 Data[4][2][4][2];//Rotation - Direction(CW, CCW) - Kicks - Offset(X, Y)
    } Kick;
    CPiece(){};
    int8 LastType = 0;
    void GetKickData()
    {
        if(Type == 1)
        {
            if(LastType == 1)
            {
                return;
            }
            Kick =
            {{
                {{{-2,0},{1,0},{-2,-1},{1,2}},//0 >> 1
                {{-1,0},{2,0},{-1,2},{2,-1}}},//0 >> 3
                {{{-1,0},{2,0},{-1,2},{2,-1}},//1 >> 2
                {{2,0},{-1,0},{2,1},{-1,-2}}},//1 >> 0
                {{{2,0},{-1,0},{2,1},{-1,-2}},//2 >> 3
                {{1,0},{-2,0},{1,-2},{-2,1}}},//2 >> 1
                {{{1,0},{-2,0},{1,-2},{-2,1}},//3 >> 0
                {{-2,0},{1,0},{-2,-1},{1,2}}} //3 >> 2
            }};
        }else
        {
            if(LastType > 1)
            {
                return;
            }
            Kick =
            {{
                {{{-1,0},{-1,1},{0,-2},{-1,-2}},//0 >> 1
                {{1,0},{1,1},{0,-2},{1,-2}}},//0 >> 3
                {{{1,0},{1,-1},{0,2},{1,2}},//1 >> 2
                {{1,0},{1,-1},{0,2},{1,2}}},//1 >> 0
                {{{1,0},{1,1},{0,-2},{1,-2}},//2 >> 3
                {{-1,0},{-1,1},{0,-2},{-1,-2}}},//2 >> 1
                {{{-1,0},{-1,-1},{0,2},{-1,2}},//3 >> 0
                {{-1,0},{-1,-1},{0,2},{-1,2}}} //3 >> 2
            }};
        }
        LastType = Type;
    }
    void SpawnBlocks()
    {
        switch (Type)
        {
            case 1:
                Block = {{{0,2},{1,2},{2,2},{3,2}}};
            break;
            case 2:
                Block = {{{0,2},{1,2},{2,2},{1,3}}};
            break;
            case 3:
                Block = {{{1,3},{2,3},{2,2},{1,2}}};
            break;
            case 4:
                Block = {{{0,2},{1,2},{2,2},{2,3}}};
            break;
            case 5:
                Block = {{{0,3},{0,2},{1,2},{2,2}}};
            break;
            case 6:
                Block = {{{0,3},{1,3},{1,2},{2,2}}};
            break;
            case 7:
                Block = {{{0,2},{1,2},{1,3},{2,3}}};
            break;
            default:
                Block = {{{0,2},{1,2},{2,2},{1,3}}};
            break;
        }
    }
    CPiece(int8 PieceType)
    {
        this->Type = PieceType;
        this->Rotation = 0;
        this->Position[0] = 3;
        this->Position[1] = 18;
        GetKickData();
        SpawnBlocks();
    }
};

class CMenu
{
    public:
    int8 StartLevel = 1;
    int8 MaxLevel = 25;
    inline void ChangeStartLevel()
    {
        if(StartLevel == 25)
        {
            StartLevel = 1;
        }else
        {
            if(StartLevel == 1)
            {
                --StartLevel;
            }
            StartLevel += 5;
        }
        if(MaxLevel < StartLevel)
        {
            MaxLevel = StartLevel;
        }
    }
    inline void ChangeMaxLevel()
    {
        if(MaxLevel == 25)
        {
            MaxLevel = StartLevel;
        }else
        {
            if(MaxLevel == 1)
            {
                --MaxLevel;
            }
            MaxLevel += 5;
        }
    }
};

class CBoard
{
    public:
    POINT Pos;
    int8 Matrix[40][10];
    int8 NextPieces[14];
    int8 NextPointer = 0;
    int Lines = 0;
    int8 HeldPiece = 0;
    bool CanHold;
    CPiece Piece;
    int8 Mode = 1; //0-Menu, 1-Game, 2-Pause
    struct Phys
    {
        time_point<system_clock, milliseconds> DASDelay, DropDelay;
        int DropSpeed;
        int DropMult = 1;
        int DASLag = 0, DropLag = 0;
        bool HDrop, RCW, RCCW, Drop;
        bool LDAS = false, RDAS = false, LeftHeld = false, RightHeld = false;
        bool CanLeft = true, CanRight = true, Left = true, Right = true;
    } Phys;
    time_point<system_clock, milliseconds> PauseTime;

    void Pause()
    {
        Mode = 2;
        PauseTime = time_point_cast<milliseconds>(system_clock::now());
    }
    void Resume()
    {
        Mode = 1;
        milliseconds Difference = (time_point_cast<milliseconds>(system_clock::now()) - PauseTime);
        Phys.DASDelay += Difference;
        Phys.DropDelay += Difference;
    }

    int8 SpawnPiece();
    void Hold();
    void GetMatrixPos()
    {
        RECT r;
        GetWindowRect(Ghwnd, &r);
        Pos.x = (r.right - r.left)/2;
        Pos.y = (r.bottom - r.top)/2;
    }
    void GenBag(bool Bag)
    {
        int8 ptypes = 0, next;
        for(int8 x = 0; x < 7; ++x)
        {
            while(true)
            {
                next = (unsigned) (int8) RNG() % 7;
                if(!((ptypes >> next) & 1))
                {
                    ptypes += 1 << next;
                    NextPieces[(Bag * 7) + x] = next + 1;
                    break;
                }
            } 
        }
    }
    bool CollisionFull(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[YPos+Blk.Pos[i][1]][XPos+Blk.Pos[i][0]]
            || (unsigned) (XPos+Blk.Pos[i][0]) > 9
            || (YPos+Blk.Pos[i][1]) & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    bool CollisionDown(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[YPos+Blk.Pos[i][1]][XPos+Blk.Pos[i][0]]
            || (YPos+Blk.Pos[i][1]) & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    bool CollisionLeft(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[YPos+Blk.Pos[i][1]][XPos+Blk.Pos[i][0]]
            || (XPos+Blk.Pos[i][0]) & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    bool CollisionRight(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[YPos+Blk.Pos[i][1]][XPos+Blk.Pos[i][0]]
            || XPos+Blk.Pos[i][0] > 9)
            {
                return 1;
            }
        }
        return 0;
    }
    int8 RotatePiece(bool Dir);//Direction: 0=CW, 1=CCW
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void HardDrop();
    void LockPiece();
    void ClearLines();

    int8 X, Y, R, MoveCount, TimerSet;
    time_point<system_clock, milliseconds> Timer;
    void AutoLock(bool Spawn)
    {
        if(Spawn)
        {
            X = Piece.Position[0];
            Y = Piece.Position[1];
            R = Piece.Rotation;
            MoveCount = 15;
            TimerSet = 0;
            return;
        }
        if(Y > Piece.Position[1])
        {
            MoveCount = 15;
            Y = Piece.Position[1];
            TimerSet = 0;
        }
        if(CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-1))
        {
            if(!TimerSet)
            {
                Timer = time_point_cast<milliseconds>(system_clock::now());
                TimerSet = 1;
            }
            if(X != Piece.Position[0] || R != Piece.Rotation)
            {
                X = Piece.Position[0];
                R = Piece.Rotation;
                if(MoveCount)
                {
                    --MoveCount;
                    Timer = time_point_cast<milliseconds>(system_clock::now());
                }else
                {
                    LockPiece();
                }
            }else
            if(Timer + (milliseconds) 500 <= time_point_cast<milliseconds>(system_clock::now()))
            {
                LockPiece();
            }
        }else
        {
            if(X != Piece.Position[0] || R != Piece.Rotation)
            {
                X = Piece.Position[0];
                R = Piece.Rotation;
            }
            if(TimerSet){TimerSet = 0;}
        }
    }

    void Input();
    void StartGame();
    HDC Ghdc;
    HBITMAP DCBitmap;
    inline void DrawBlock(int x, int y)
    {
        Rectangle(Ghdc, 166+(x*30), 636-(y*30), 195+(x*30), 665-(y*30));
    };
    void RenderMatrix();
    void RenderLines();
    void RenderBkgd();
    SBlock RenderBlock;
    int8 RenderX, RenderY, RenderR, ShadowY;
    void RenderPiece(bool Spawn);
    void FlashPiece();
    void RenderNext();
    void FlashLine(int8 Line);
    void RenderHold();

    CBoard()
    {
        GetMatrixPos();
        for(int8 i = 39; i >= 0; i--)
        {
            for(int8 j = 0; j < 10; ++j)
            {
                this->Matrix[i][j] = 0;
            }
        }
    };
};

CBoard Player1;
