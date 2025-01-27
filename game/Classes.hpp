#ifndef TETRISGAME
#define TETRISGAME

#include <Windows.h>
#include <chrono>
#include <random>
#include "../headers/Constants.hpp"

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
        Piece Piece;

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
    };
}

#endif