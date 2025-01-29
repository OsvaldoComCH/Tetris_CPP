#ifndef TETRISGAME
#define TETRISGAME

#include <Windows.h>
#include <chrono>
#include <random>
#include <vector>
#include "../headers/Constants.hpp"

#define RF_PIECESPAWN 1
#define RF_PIECE 2
#define RF_HOLD 4
#define RF_NEXT 8
#define RF_LINES 16
#define RF_LEVEL 32
#define RF_POINTS 64
#define RF_MATRIX 128

namespace Tetris::Game
{
    std::mt19937 RNG (system_clock::now().time_since_epoch().count());

    //Formula: pow(0.8 - ((Level-1)*0.007), Level-1);
    static constexpr int SpeedTable[25] =
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

    int8 StartLevel = 1;
    int8 MaxLevel = 25;

    typedef struct Point
    {
        int8 x, y;

        int8& operator[] (int index)
        {
            if(index){return y;}
            return x;
        }
        int8& operator[] (int8 index)
        {
            if(index){return y;}
            return x;
        }
    } Point;

    typedef struct Block
    {
        Point Positions[4];

        Point& operator[] (int index)
        {
            return Positions[index];
        }
        Point& operator[] (int8 index)
        {
            return Positions[index];
        }
    } Block;

    typedef struct KickData
    {
        Point Data[4][2][4];

        Point& operator() (int8 Rotation, int8 Direction, int8 Kick)
        {
            return Data[Rotation][Direction][Kick];
        }
    } KickData;

    typedef struct RenderData
    {
        short Flags;//Bkgd,Matrix,Points,Level,Lines,Next,Hold,Piece,PieceSpawn
        Block Block;
        int8 X, Y, R, Shadow;
    } RenderData;
    
    class PhysFlags 
    {
        short Flags;

        inline short Left(){return Flags & 0x0001;}
        inline void Left(bool Value){if(Value){Flags |= 0x0001;}else{Flags &= ~0x0001;}}

        inline short Right(){return Flags & 0x0002;}
        inline void Right(bool Value){if(Value){Flags |= 0x0002;}else{Flags &= ~0x0002;}}

        inline short CanLeft(){return Flags & 0x0004;}
        inline void CanLeft(bool Value){if(Value){Flags |= 0x0004;}else{Flags &= ~0x0004;}}

        inline short CanRight(){return Flags & 0x0008;}
        inline void CanRight(bool Value){if(Value){Flags |= 0x0008;}else{Flags &= ~0x0008;}}
        
        inline short LDAS(){return Flags & 0x0010;}
        inline void LDAS(bool Value){if(Value){Flags |= 0x0010;}else{Flags &= ~0x0010;}}

        inline short RDAS(){return Flags & 0x0020;}
        inline void RDAS(bool Value){if(Value){Flags |= 0x0020;}else{Flags &= ~0x0020;}}

        inline short LeftHeld(){return Flags & 0x0040;}
        inline void LeftHeld(bool Value){if(Value){Flags |= 0x0040;}else{Flags &= ~0x0040;}}

        inline short RightHeld(){return Flags & 0x0080;}
        inline void RightHeld(bool Value){if(Value){Flags |= 0x0080;}else{Flags &= ~0x0080;}}
        
        inline short PrevDown(){return Flags & 0x0100;}
        inline void PrevDown(bool Value){if(Value){Flags |= 0x0100;}else{Flags &= ~0x0100;}}

        inline short DownHeld(){return Flags & 0x0200;}
        inline void DownHeld(bool Value){if(Value){Flags |= 0x0200;}else{Flags &= ~0x0200;}}

        inline short Drop(){return Flags & 0x0400;}
        inline void Drop(bool Value){if(Value){Flags |= 0x0400;}else{Flags &= ~0x0400;}}

        inline short HardDrop(){return Flags & 0x0800;}
        inline void HardDrop(bool Value){if(Value){Flags |= 0x0800;}else{Flags &= ~0x0800;}}

        inline short RCW(){return Flags & 0x1000;}
        inline void RCW(bool Value){if(Value){Flags |= 0x1000;}else{Flags &= ~0x1000;}}

        inline short RCCW(){return Flags & 0x2000;}
        inline void RCCW(bool Value){if(Value){Flags |= 0x2000;}else{Flags &= ~0x2000;}}
    };

    using namespace std::chrono;
    typedef struct Phys
    {
        time_point<system_clock, milliseconds> DASDelay, DropDelay;
        int DropSpeed[2];//Interval between drops in microseconds (0-normal, 1-soft drop)
        int DASLag, DropLag;
        bool HDrop, RCW, RCCW, Drop;
        bool LDAS, RDAS, LeftHeld, RightHeld;
        bool CanLeft, CanRight, Left, Right;
        bool DownHeld, PrevDown;
    } Phys;

    typedef struct AutolockPhys
    {
        time_point<system_clock, milliseconds> Timer;
        int8 X, Y, R, MoveCount, TimerSet;
    } AutolockPhys;

    class Piece
    {
        public:
        KickData Kicks;
        Block Blocks;
        Point Position;
        int8 Type;//1=I, 2=T, 3=O, 4=L, 5=J, 6=Z, 7=S
        int8 Rotation;//0-up, 1-right, 2-down, 3-left

        void GetKickData(int8 LastType)
        {
            if(Type == 1)
            {
                if(LastType == 1)
                {
                    return;
                }
                Kicks =
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
                Kicks =
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
                    Blocks = {{{0,2},{1,2},{2,2},{3,2}}};
                break;
                case 2:
                    Blocks = {{{0,2},{1,2},{2,2},{1,3}}};
                break;
                case 3:
                    Blocks = {{{1,3},{2,3},{2,2},{1,2}}};
                break;
                case 4:
                    Blocks = {{{0,2},{1,2},{2,2},{2,3}}};
                break;
                case 5:
                    Blocks = {{{0,3},{0,2},{1,2},{2,2}}};
                break;
                case 6:
                    Blocks = {{{0,3},{1,3},{1,2},{2,2}}};
                break;
                case 7:
                    Blocks = {{{0,2},{1,2},{1,3},{2,3}}};
                break;
                default:
                    Blocks = {{{0,2},{1,2},{2,2},{1,3}}};
                break;
            }
        }
        void Spawn(int8 PieceType)
        {
            int8 LastType = Type;
            Type = PieceType;
            Rotation = 0;
            Position[0] = 3;
            Position[1] = 18;
            GetKickData(LastType);
            SpawnBlocks();
        }
    };

    class Board
    {
        public:
        static std::vector<Board *> AllBoards;

        Phys Phys;// 46 !Aligns on 8-byte intervals
        Piece Piece;// 76
        int Lines;// 4
        int8 Matrix[40][10];// 400
        RenderData RenderData;// 14 !Aligns on 2-byte intervals
        int8 NextPieces[14];// 14
        int8 NextPointer;
        int8 Level;
        int8 HeldPiece;
        bool CanHold;
        AutolockPhys LockPhys;// 13 !Aligns on 8-byte intervals

        Board()
        {
            AllBoards.push_back(this);
            Init();
        }

        ~Board()
        {
            for(int i = 0; i < AllBoards.size(); ++i)
            {
                if(AllBoards[i] == this)
                {
                    AllBoards.erase(AllBoards.begin() + i);
                }
            }
        }

        void Init()
        {
            for(int8 i = 39; i >= 0; i--)
            {
                for(int8 j = 0; j < 10; ++j)
                {
                    this->Matrix[i][j] = 0;
                }
            }
        }
        
        void GenerateBags(int8 Bag)
        {
            int8 ptypes = 0, Next;
            for(int8 x = 0; x < 7; ++x)
            {
                while(true)
                {
                    Next = (unsigned) (int8) RNG() % 7;
                    if(!((ptypes >> Next) & 1))
                    {
                        ptypes += 1 << Next;
                        NextPieces[(Bag * 7) + x] = Next + 1;
                        break;
                    }
                } 
            }
        }

        int8 Collision(Block& Blk, int8 XPos, int8 YPos)
        {
            for(int8 i = 0; i < 4; ++i)
            {
                if(this->Matrix[YPos + Blk[i][1]][XPos + Blk[i][0]]
                || (unsigned) (XPos + Blk[i][0]) > 9
                || (YPos + Blk[i][1]) < 0)
                {
                    return 1;
                }
            }
        }

        int8 CollisionDown(Block& Blk, int8 XPos, int8 YPos)
        {
            for(int8 i = 0; i < 4; ++i)
            {
                if(this->Matrix[YPos + Blk[i][1]][XPos + Blk[i][0]]
                || (YPos + Blk[i][1]) < 0)
                {
                    return 1;
                }
            }
            return 0;
        }

        int8 CollisionSides(Block& Blk, int8 XPos, int8 YPos)
        {
            for(int8 i = 0; i < 4; ++i)
            {
                if(this->Matrix[YPos + Blk[i][1]][XPos + Blk[i][0]]
                || (unsigned) (XPos + Blk[i][0]) > 9)
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
        int8 SpawnPiece();
        int8 Hold();

        void Input();
        void StartGame();

        void RenderMatrix();
        void RenderLines();
        void RenderLevel();
        void RenderBkgd();
        void RenderPiece(bool Spawn);
        void RenderNext();
        void RenderHold();
        //void FlashPiece();
        //void FlashLine(int8 Line);


        void GetSpeed()
        {
            Phys.DropSpeed[0] = SpeedTable[Level - 1];
            Phys.DropSpeed[1] = SpeedTable[Level - 1] / 20 + 1;
        }

        void AutoLock()
        {
            if(LockPhys.Y > Piece.Position[1])
            {
                LockPhys.MoveCount = 15;
                LockPhys.Y = Piece.Position[1];
                LockPhys.TimerSet = 0;
            }
            if(CollisionDown(Piece.Blocks, Piece.Position[0], Piece.Position[1]-1))
            {
                if(!LockPhys.TimerSet)
                {
                    LockPhys.Timer = time_point_cast<milliseconds>(system_clock::now());
                    LockPhys.TimerSet = 1;
                }
                if(LockPhys.X != Piece.Position[0] || LockPhys.R != Piece.Rotation)
                {
                    LockPhys.X = Piece.Position[0];
                    LockPhys.R = Piece.Rotation;
                    if(LockPhys.MoveCount)
                    {
                        --LockPhys.MoveCount;
                        LockPhys.Timer = time_point_cast<milliseconds>(system_clock::now());
                    }else
                    {
                        LockPiece();
                    }
                }else
                if(LockPhys.Timer + (milliseconds) 500 <= time_point_cast<milliseconds>(system_clock::now()))
                {
                    LockPiece();
                }
            }else
            {
                if(LockPhys.X != Piece.Position[0] || LockPhys.R != Piece.Rotation)
                {
                    LockPhys.X = Piece.Position[0];
                    LockPhys.R = Piece.Rotation;
                }
                if(LockPhys.TimerSet){LockPhys.TimerSet = 0;}
            }
        };
    };
    
    time_point<system_clock, milliseconds> PauseTime;

    void Pause()
    {
        PauseTime = time_point_cast<milliseconds>(system_clock::now());
    }

    void Resume()
    {
        milliseconds Difference = (time_point_cast<milliseconds>(system_clock::now()) - PauseTime);
        for(int i = 0; i < Board::AllBoards.size(); ++i)
        {
            Board::AllBoards[i]->Phys.DASDelay += Difference;
            Board::AllBoards[i]->Phys.DropDelay += Difference;
        }
    }
}

#endif