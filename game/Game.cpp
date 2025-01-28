#include "./Classes.hpp"

namespace Tetris::Game
{

bool Board::MoveDown()
{
    if(!CollisionDown(Piece.Blocks, Piece.Position.x, Piece.Position.y - 1))
    {
        --Piece.Position.y;
        RenderData.Flags |= RF_PIECE;
        return 0;
    }
    return 1;
}

void Board::MoveLeft()
{
    if(!CollisionSides(Piece.Blocks, Piece.Position.x - 1, Piece.Position.y))
    {
        --Piece.Position.x;
        RenderData.Flags |= RF_PIECE;
    }
}

void Board::MoveRight()
{
    if(!CollisionSides(Piece.Blocks, Piece.Position.x + 1, Piece.Position.y))
    {
        ++Piece.Position.x;
        RenderData.Flags |= RF_PIECE;
    }
}

int8 Board::RotatePiece(bool Dir)
{
    Block TempBlock;
    int8 OldRotation;
    if(!Dir)
    {
        OldRotation = Piece.Rotation;
        ++Piece.Rotation;
        Piece.Rotation &= 0b11;
    }
    else
    {
        OldRotation = Piece.Rotation;
        --Piece.Rotation;
        Piece.Rotation &= 0b11;
    }
    switch(Piece.Type)
    {
        case 1://I
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,2},{1,2},{2,2},{3,2}}};
                break;
                case 1:
                    TempBlock = {{{2,3},{2,2},{2,1},{2,0}}};
                break;
                case 2:
                    TempBlock = {{{3,1},{2,1},{1,1},{0,1}}};
                break;
                case 3:
                    TempBlock = {{{1,0},{1,1},{1,2},{1,3}}};
                break;
            }
        break;
        case 2://T
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,2},{1,2},{2,2},{1,3}}};
                break;
                case 1:
                    TempBlock = {{{1,3},{1,2},{1,1},{2,2}}};
                break;
                case 2:
                    TempBlock = {{{2,2},{1,2},{0,2},{1,1}}};
                break;
                case 3:
                    TempBlock = {{{1,1},{1,2},{1,3},{0,2}}};
                break;
            }
        break;
        case 3://O
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{1,3},{2,3},{2,2},{1,2}}};
                break;
                case 1:
                    TempBlock = {{{2,3},{2,2},{1,2},{1,3}}};
                break;
                case 2:
                    TempBlock = {{{2,2},{1,2},{1,3},{2,3}}};
                break;
                case 3:
                    TempBlock = {{{1,2},{1,3},{2,3},{2,2}}};
                break;
            }
        break;
        case 4://L
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,2},{1,2},{2,2},{2,3}}};
                break;
                case 1:
                    TempBlock = {{{1,3},{1,2},{1,1},{2,1}}};
                break;
                case 2:
                    TempBlock = {{{2,2},{1,2},{0,2},{0,1}}};
                break;
                case 3:
                    TempBlock = {{{1,1},{1,2},{1,3},{0,3}}};
                break;
            }
        break;
        case 5://J
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,3},{0,2},{1,2},{2,2}}};
                break;
                case 1:
                    TempBlock = {{{2,3},{1,3},{1,2},{1,1}}};
                break;
                case 2:
                    TempBlock = {{{2,1},{2,2},{1,2},{0,2}}};
                break;
                case 3:
                    TempBlock = {{{0,1},{1,1},{1,2},{1,3}}};
                break;
            }
        break;
        case 6://Z
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,3},{1,3},{1,2},{2,2}}};
                break;
                case 1:
                    TempBlock = {{{2,3},{2,2},{1,2},{1,1}}};//It's look who it is {{{2,3},{2,2},{1,2},{2,2}}}
                break;
                case 2:
                    TempBlock = {{{2,1},{1,1},{1,2},{0,2}}};
                break;
                case 3:
                    TempBlock = {{{0,1},{0,2},{1,2},{1,3}}};
                break;
            }
        break;
        case 7://S
            switch(Piece.Rotation)
            {
                case 0:
                    TempBlock = {{{0,2},{1,2},{1,3},{2,3}}};
                break;
                case 1:
                    TempBlock = {{{1,3},{1,2},{2,2},{2,1}}};
                break;
                case 2:
                    TempBlock = {{{2,2},{1,2},{1,1},{0,1}}};
                break;
                case 3:
                    TempBlock = {{{1,1},{1,2},{0,2},{0,3}}};
                break;
            }
        break;
    }

    bool ValidPos = !Collision(TempBlock, Piece.Position.x, Piece.Position.y);
    if(!ValidPos)
    {
        for(int i = 0; i < 4; ++i)
        {
            Point Offset = Piece.Kicks(OldRotation, Dir, i);
            if(!Collision(TempBlock, Offset.x, Piece.Position.y + Offset.y))
            {
                Piece.Position.x += Offset.x;
                Piece.Position.y += Offset.y;
                ValidPos = true;
                break;
            }
        }
    }

    if(ValidPos)
    {
        Piece.Blocks = TempBlock;
        RenderData.Flags |= RF_PIECE;
        return 0;
    }
    Piece.Rotation = OldRotation;
    return 1;
}

void Board::LockPiece()
{
    for(int8 i = 0; i < 4; ++i)
    {
        Matrix[Piece.Position.y + Piece.Blocks[i].y][Piece.Position.x + Piece.Blocks[i].x] = Piece.Type;
    }
    Piece.Type = 0;
    ClearLines();
    SpawnPiece();
}

void Board::HardDrop()
{
    int8 i = 0;
    while(++i)
    {
        if(CollisionDown(Piece.Blocks, Piece.Position.x, Piece.Position.y-i))
        {
            break;
        }
    }
    Piece.Position.y -= i - 1;
    RenderData.Flags |= RF_PIECE;
    LockPiece();
}


void Board::ClearLines()
{
    for(int8 Line = 3; Line >= 0; Line--)
    {
        if((Piece.Position.y + Line) >= 0){
            if(
            Matrix[Piece.Position.y + Line][0] && Matrix[Piece.Position.y + Line][1] &&
            Matrix[Piece.Position.y + Line][2] && Matrix[Piece.Position.y + Line][3] &&
            Matrix[Piece.Position.y + Line][4] && Matrix[Piece.Position.y + Line][5] &&
            Matrix[Piece.Position.y + Line][6] && Matrix[Piece.Position.y + Line][7] &&
            Matrix[Piece.Position.y + Line][8] && Matrix[Piece.Position.y + Line][9])
            {
                ++Lines;
                for(int8 i = (Piece.Position.y + Line); i < 39; ++i)
                {
                    for(int8 j = 0; j < 10; ++j)
                    {
                        Matrix[i][j] = Matrix[i+1][j];
                    }
                }
                for(int8 j = 0; j < 10; ++j)
                {
                    Matrix[39][j] = 0;
                }
            }
        }
    }
    RenderData.Flags |= RF_LINES;
    RenderData.Flags |= RF_MATRIX;

    if(Level < MaxLevel)
    {
        Level = Lines / 10 + StartLevel;
        RenderData.Flags |= RF_LEVEL;
    }
    GetSpeed();
}

int8 Board::SpawnPiece()
{
    Piece.Spawn(NextPieces[NextPointer]);
    if(NextPointer == 13)
    {
        NextPointer = 0;
        GenerateBags(1);
    }else
    if(NextPointer == 6)
    {
        NextPointer += 1;
        GenerateBags(0);
    }
    else
    {
        NextPointer += 1;
    }

    CanHold = true;
    Phys.HDrop = false;
    Phys.RCW = false;
    Phys.RCCW = false;
    Phys.Drop = true;

    RenderData.Flags |= RF_NEXT;
    RenderData.Flags |= RF_PIECESPAWN;
    if(Collision(Piece.Blocks, Piece.Position.x, Piece.Position.y))
    {
        return 1;
    }

    LockPhys.X = Piece.Position.x;
    LockPhys.Y = Piece.Position.y;
    LockPhys.R = Piece.Rotation;
    LockPhys.MoveCount = 15;
    LockPhys.TimerSet = 0;
    return 0;
}
int8 Board::Hold()
{
    RenderData.Flags |= RF_MATRIX;
    if(HeldPiece)
    {
        int8 Temp;
        Temp = HeldPiece;
        HeldPiece = Piece.Type;
        RenderData.Flags |= RF_HOLD;

        Piece.Spawn(Temp);
        RenderData.Flags |= RF_PIECESPAWN;
        if(Collision(Piece.Blocks, Piece.Position.x, Piece.Position.y))
        {
            return 1;
        }

        LockPhys.X = Piece.Position.x;
        LockPhys.Y = Piece.Position.y;
        LockPhys.R = Piece.Rotation;
        LockPhys.MoveCount = 15;
        LockPhys.TimerSet = 0;
        CanHold = false;
    }else
    {
        HeldPiece = Piece.Type;
        RenderData.Flags |= RF_HOLD;
        SpawnPiece();
        CanHold = false;
    }
    return 0;
}

void Board::StartGame()
{
    NextPointer = 0;
    Lines = 0;
    Level = StartLevel;
    HeldPiece = 0;
    Phys.DropMult = 1;
    Phys.DropLag = 0;
    Phys.DASLag = 0;
    Phys.LDAS = false;
    Phys.RDAS = false;
    Phys.LeftHeld = false;
    Phys.RightHeld = false;
    Phys.CanLeft = true;
    Phys.CanRight = true;
    Phys.Left = true;
    Phys.Right = true;
    RenderData.Flags = 255;
    Init();
    GenerateBags(0);
    GenerateBags(1);
    GetSpeed();
    SpawnPiece();
}

}