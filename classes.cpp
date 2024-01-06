#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <thread>
#include "./VirtualKeyCodes.h"
using namespace std;

typedef signed char int8;
typedef struct SBlock
{
    int8 Pos[4][2];
} SBlock;

HWND Ghwnd; //Global handle to the window

class CPiece
{
    public:
    int8 Type;//1=I, 2=T, 3=O, 4=L, 5=J, 6=Z, 7=S
    int8 Rotation;//0-up, 1-right, 2-down, 3-left
    int8 Position[2];//X, Y
    SBlock Block;
    struct Kick
    {
        int8 Data[4][2][4][2];//Rotation - Direction(0-CW,1-CCW) - Kicks - (X, Y)
    } Kick;
    CPiece(){};
    void GetKickData()
    {
        switch(Type)
        {
            case 1:
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
            break;
            default:
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
            break;
        }
    }
    SBlock SpawnBlocks()
    {
        switch (Type)
        {
            case 1:
                return {{{0,2},{1,2},{2,2},{3,2}}};
            break;
            case 2:
                return {{{0,2},{1,2},{2,2},{1,3}}};
            break;
            case 3:
                return {{{1,3},{2,3},{2,2},{1,2}}};
            break;
            case 4:
                return {{{0,2},{1,2},{2,2},{2,3}}};
            break;
            case 5:
                return {{{0,3},{0,2},{1,2},{2,2}}};
            break;
            case 6:
                return {{{0,3},{1,3},{1,2},{2,2}}};
            break;
            case 7:
                return {{{0,2},{1,2},{1,3},{2,3}}};
            break;
            default:
                return {{{0,2},{1,2},{2,2},{1,3}}};
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
        this->Block = SpawnBlocks();
    }
};

class CBoard
{
    public:
    POINT Pos;
    int8 Matrix[10][40];
    int8 NextPieces[14];
    int8 NextPointer = 0;
    CPiece Piece;

    int8 SpawnPiece();
    void GetMatrixPos()
    {
        RECT r;
        GetWindowRect(Ghwnd, &r);
        Pos.x = (r.right - r.left)/2;
        Pos.y = (r.bottom - r.top)/2;
        //std::cout << Pos.x << "\t" << Pos.y;
    }
    void GenBag(bool Bag)
    {
        int8 ptypes = 0, next;
        for(int8 x = 0; x < 7; ++x)
        {
            while(true)
            {
                next = (unsigned) (int8) rand() % 7;
                if(!((ptypes >> next) & 1))
                {
                    ptypes += 1 << next;
                    NextPieces[(Bag * 7) + x] = next + 1;
                    break;
                }
            } 
        }
    }
    int8 ColisionFull(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[XPos+Blk.Pos[i][0]][YPos+Blk.Pos[i][1]]
            || (unsigned) (XPos+Blk.Pos[i][0]) > 9
            || YPos+Blk.Pos[i][1] & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    int8 ColisionDown(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[XPos+Blk.Pos[i][0]][YPos+Blk.Pos[i][1]]
            || YPos+Blk.Pos[i][1] & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    int8 ColisionLeft(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[XPos+Blk.Pos[i][0]][YPos+Blk.Pos[i][1]]
            || XPos+Blk.Pos[i][0] & 0x80)
            {
                return 1;
            }
        }
        return 0;
    }
    int8 ColisionRight(SBlock& Blk, int8 XPos, int8 YPos)
    {
        for(int8 i = 0; i < 4; ++i)
        {
            if(this->Matrix[XPos+Blk.Pos[i][0]][YPos+Blk.Pos[i][1]]
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

    void RenderMatrix();
    void RenderBkgd(HDC hdc);
    void RenderPiece(bool Spawn);
    void FlashPiece();
    void RenderNext();
    void FlashLine(int8 Line);
    void RenderShadow(bool Spawn);

    CBoard()
    {
        time_t t;
        GetMatrixPos();
        srand((unsigned) time(&t));
        GenBag(0);
        GenBag(1);
        for(int8 i = 39; i >= 0; i--)
        {
            for(int8 j = 0; j < 10; ++j)
            {
                this->Matrix[j][i] = 0;
            }
        }
    };
};
CBoard Player;