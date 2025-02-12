#ifndef TETRISGAME
#define TETRISGAME

#include <Windows.h>
#include <chrono>
#include <random>
#include <vector>
#include "../headers/Constants.hpp"
#include "../render/Classes.hpp"
#include "../menu/Classes.hpp"

namespace Tetris::Game
{
    std::mt19937 RNG (std::chrono::system_clock::now().time_since_epoch().count());

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

    class CFlags
    {
        private:
        short Flags;

        public:
        inline short Get(short Flag){return Flags & Flag;}
        inline void Set(short Flag){Flags |= Flag;}
        inline void Unset(short Flag){Flags &= ~Flag;}
        inline void Clear(){Flags = 0;}
    };

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
        CFlags Flags;
        Block Block;
        int8 X, Y, R, Shadow;
    } RenderData;

    enum PhysFlags
    {
        Left = 0x0001,
        Right = 0x0002,
        CanLeft = 0x0004,
        CanRight = 0x0008,
        LDAS = 0x0010,
        RDAS = 0x0020,
        LeftHeld = 0x0040,
        RightHeld = 0x0080,
        Drop = 0x0100,
        HardDrop = 0x0200,
        RCW = 0x0400,
        RCCW = 0x0800
    };

    enum RenderFlags
    {
        PIECESPAWN = 0x0001,
        PIECE = 0x0002,
        HOLD = 0x0004,
        NEXT = 0x0008,
        LINES = 0x0010,
        LEVEL = 0x0020,
        POINTS = 0x0040,
        MATRIX = 0x0080,
        LAYOUT = 0x0100
    };

    typedef struct Phys
    {
        time_milli DASDelay, DropDelay;
        unsigned DropSpeed[2];//Interval between drops in microseconds (0-normal, 1-soft drop)
        unsigned DASLag, DropLag;
        bool DownHeld, PrevDown;
    } Phys;

    typedef struct AutolockPhys
    {
        time_milli Timer;
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

        Phys Phys;
        CFlags PFlags;
        Piece Piece;
        int8 NextPieces[14];
        int8 NextPointer;
        bool CanHold;
        int8 Matrix[40][10];
        RenderData RenderData;
        int8 HeldPiece;
        int8 Level;
        int Lines;
        unsigned Points;
        Render::Layer * Layer;
        AutolockPhys LockPhys;

        Board()
        {
            AllBoards.push_back(this);
            Init();
            Layer = Render::CreateLayer(0,0,800,600);
            Render::RenderBkgd(Layer, Render::Color::Gray);
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
            Render::DestroyLayer(Layer);
        }

        void Init()
        {
            NextPointer = 0;
            Lines = 0;
            Level = StartLevel;
            HeldPiece = 0;
            Phys.DropLag = 0;
            Phys.DASLag = 0;
            Phys.DownHeld = false;
            Phys.PrevDown = false;

            PFlags.Unset(PhysFlags::LDAS);
            PFlags.Unset(PhysFlags::RDAS);
            PFlags.Unset(PhysFlags::LeftHeld);
            PFlags.Unset(PhysFlags::RightHeld);
            PFlags.Set(PhysFlags::CanLeft);
            PFlags.Set(PhysFlags::CanRight);
            PFlags.Set(PhysFlags::Left);
            PFlags.Set(PhysFlags::Right);

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
                if(this->Matrix[YPos + Blk[i].y][XPos + Blk[i].x]
                || (unsigned) (XPos + Blk[i].x) > 9
                || (YPos + Blk[i].y) < 0)
                {
                    return 1;
                }
            }
            return 0;
        }

        int8 CollisionDown(Block& Blk, int8 XPos, int8 YPos)
        {
            for(int8 i = 0; i < 4; ++i)
            {
                if(this->Matrix[YPos + Blk[i].y][XPos + Blk[i].x]
                || (YPos + Blk[i].y) < 0)
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
                if(this->Matrix[YPos + Blk[i].y][XPos + Blk[i].x]
                || (unsigned) (XPos + Blk[i].x) > 9)
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

        void Input(time_milli TickTime);
        void StartGame();

        void RenderLayout();
        void RenderMatrix();
        void RenderLines();
        void RenderLevel();
        void RenderBkgd();
        void RenderPiece(bool Spawn);
        void RenderNext();
        void RenderHold();
        void Render();

        void GetSpeed()
        {
            Phys.DropSpeed[0] = SpeedTable[Level - 1];
            Phys.DropSpeed[1] = SpeedTable[Level - 1] / 20 + 1;
        }

        void AutoLock(time_milli TickTime)
        {
            using namespace std::chrono;
            if(LockPhys.Y > Piece.Position.y)
            {
                LockPhys.MoveCount = 15;
                LockPhys.Y = Piece.Position.y;
                LockPhys.TimerSet = 0;
            }
            if(CollisionDown(Piece.Blocks, Piece.Position.x, Piece.Position.y-1))
            {
                if(!LockPhys.TimerSet)
                {
                    LockPhys.Timer = TickTime;
                    LockPhys.TimerSet = 1;
                }
                if(LockPhys.X != Piece.Position.x || LockPhys.R != Piece.Rotation)
                {
                    LockPhys.X = Piece.Position.x;
                    LockPhys.R = Piece.Rotation;
                    if(LockPhys.MoveCount)
                    {
                        --LockPhys.MoveCount;
                        LockPhys.Timer = TickTime;
                    }else
                    {
                        LockPiece();
                    }
                }else
                if(LockPhys.Timer + (milliseconds) 500 <= TickTime)
                {
                    LockPiece();
                }
            }else
            {
                if(LockPhys.X != Piece.Position.x || LockPhys.R != Piece.Rotation)
                {
                    LockPhys.X = Piece.Position.x;
                    LockPhys.R = Piece.Rotation;
                }
                if(LockPhys.TimerSet){LockPhys.TimerSet = 0;}
            }
        };
    };

    std::vector<Board *> Board::AllBoards;
    
    time_milli PauseTime;

    void Pause()
    {
        using namespace std::chrono;
        PauseTime = time_point_cast<milliseconds>(system_clock::now());
        MenuStack::OpenMenu(PauseMenu);
    }

    void Resume()
    {
        using namespace std::chrono;
        milliseconds Difference = (time_point_cast<milliseconds>(system_clock::now()) - PauseTime);
        for(int i = 0; i < Board::AllBoards.size(); ++i)
        {
            Board::AllBoards[i]->Phys.DASDelay += Difference;
            Board::AllBoards[i]->Phys.DropDelay += Difference;
            Board::AllBoards[i]->LockPhys.Timer += Difference;
        }
    }

    void GetPieceBlocks(int8 Type, Block& Blocks)
    {
        switch(Type)
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
}

#endif