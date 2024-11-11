#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <string>
#include <mutex>
#include "./VirtualKeyCodes.h"

#define RF_PIECESPAWN 1
#define RF_PIECE 2
#define RF_HOLD 4
#define RF_NEXT 8
#define RF_LINES 16
#define RF_LEVEL 32
#define RF_POINTS 64
#define RF_MATRIX 128

using namespace std;
using namespace std::chrono;

typedef signed char int8;
typedef struct SBlock
{
    int8 Pos[4][2];
} SBlock;

std::minstd_rand RNG (system_clock::now().time_since_epoch().count());
HWND Ghwnd; //Global handle to the window
std::mutex RenderMutex;

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
    int8 StartLevel;
    int8 MaxLevel;
    CMenu()
    {
        StartLevel = 1;
        MaxLevel = 25;
    }
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
    static void Pause();
    static void Resume();

    static void RenderMenu();
};

class CBoard
{
    public:
    int8 Matrix[40][10];
    int8 NextPieces[14];
    int8 NextPointer;
    int Lines;
    int8 Level;
    int8 HeldPiece;
    bool CanHold;
    CPiece Piece;
    CMenu Menu;
    static int8 Mode; //0-Menu, 1-Game, 2-Pause, 3-End
    struct Phys
    {
        time_point<system_clock, milliseconds> DASDelay, DropDelay;
        int DropSpeed[2];//Interval between drops in microseconds (0-normal, 1-soft drop)
        int DropMult;
        int DASLag, DropLag;
        bool HDrop, RCW, RCCW, Drop;
        bool LDAS, RDAS, LeftHeld, RightHeld;
        bool CanLeft, CanRight, Left, Right;
    } Phys;
    time_point<system_clock, milliseconds> PauseTime;

    void Pause()
    {
        PauseTime = time_point_cast<milliseconds>(system_clock::now());
    }
    void Resume()
    {
        milliseconds Difference = (time_point_cast<milliseconds>(system_clock::now()) - PauseTime);
        Phys.DASDelay += Difference;
        Phys.DropDelay += Difference;
        Timer += Difference;
    }

    int8 SpawnPiece();
    int8 Hold();
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
            || (YPos+Blk.Pos[i][1]) < 0)
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
            || (YPos+Blk.Pos[i][1]) < 0)
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
            || (XPos+Blk.Pos[i][0]) < 0)
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
    bool MoveDown();
    void MoveLeft();
    void MoveRight();
    void HardDrop();
    void LockPiece();
    void ClearLines();
    void GetSpeed()
    {
        //Formula: pow(0.8 - ((Level-1)*0.007), Level-1);
        static const int SpeedTable[25] =
        {
            1000000,
            793000,
            617796,
            472729,
            355197,
            262004,
            189677,
            134735,
            93882,
            64152,
            42976,
            28218,
            18153,
            11439,
            7059,
            4264,
            2520,
            1457,
            824,
            455,
            246,
            130,
            67,
            34,
            16
        };
        Phys.DropSpeed[0] = SpeedTable[Level - 1];
        Phys.DropSpeed[1] = SpeedTable[Level - 1] / 20 + 1;
    }

    int8 X, Y, R, MoveCount, TimerSet;
    time_point<system_clock, milliseconds> Timer;
    void AutoLock()
    {
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
    void RenderLevel();
    void RenderBkgd();
    short RenderFlags;//Bkgd,Matrix,Points,Level,Lines,Next,Hold,Piece,PieceSpawn
    SBlock RenderBlock;
    int8 RenderX, RenderY, RenderR, ShadowY;
    void RenderPiece(bool Spawn);
    void FlashPiece();
    void RenderNext();
    void FlashLine(int8 Line);
    void RenderHold();

    void Render();

    void InitMatrix()
    {
        for(int8 i = 39; i >= 0; i--)
        {
            for(int8 j = 0; j < 10; ++j)
            {
                this->Matrix[i][j] = 0;
            }
        }
    }
    CBoard()
    {
        
    };
};

CBoard Player1;


void CMenu::Pause()
{
    CBoard::Mode = 2;
    Player1.Pause();
}
void CMenu::Resume()
{
    CBoard::Mode = 1;
    Player1.Resume();
    std::thread (CBoard::Input, &Player1).detach();
}