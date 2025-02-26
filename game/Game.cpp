#include "./Classes.hpp"

namespace Tetris::Game
{

void EventLoop(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_KILLFOCUS:
        {
            Pause();
            PostMessage(hwnd, WM_PRINT, 0, 0);
        }
        break;
        case WM_KEYDOWN:
        {
            if(wParam == VK_ESCAPE)
            {
                Pause();
                PostMessage(hwnd, WM_PRINT, 0, 0);
            }
        }
        break;
    }
}

bool Board::MoveDown()
{
    if(!CollisionDown(Piece.Blocks, Piece.Position.x, Piece.Position.y - 1))
    {
        --Piece.Position.y;
        RenderData.Flags.Set(RenderFlags::PIECE);
        PFlags.Unset(PhysFlags::TSpin);
        return 0;
    }
    return 1;
}

void Board::MoveLeft()
{
    if(!CollisionSides(Piece.Blocks, Piece.Position.x - 1, Piece.Position.y))
    {
        --Piece.Position.x;
        RenderData.Flags.Set(RenderFlags::PIECE);
        PFlags.Unset(PhysFlags::TSpin);
    }
}

void Board::MoveRight()
{
    if(!CollisionSides(Piece.Blocks, Piece.Position.x + 1, Piece.Position.y))
    {
        ++Piece.Position.x;
        RenderData.Flags.Set(RenderFlags::PIECE);
        PFlags.Unset(PhysFlags::TSpin);
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

    int8 Kick = 0;

    bool ValidPos = !Collision(TempBlock, Piece.Position.x, Piece.Position.y);
    if(!ValidPos)
    {
        for(int i = 0; i < 4; ++i)
        {
            Point Offset = Piece.Kicks(OldRotation, Dir, i);
            if(!Collision(TempBlock, Piece.Position.x + Offset.x, Piece.Position.y + Offset.y))
            {
                Piece.Position.x += Offset.x;
                Piece.Position.y += Offset.y;
                ValidPos = true;
                Kick = i + 1;
                break;
            }
        }
    }

    if(ValidPos)
    {
        Piece.Blocks = TempBlock;
        RenderData.Flags.Set(RenderFlags::PIECE);
        PFlags.Unset(PhysFlags::TSpin);
        if(Piece.Type == 2)
        {
            int8 CornerA, CornerB;
            int8 Total = 0;
            int8 Count = Piece.Rotation;
            
            int8 Res = Collision(&SpinTable[Count & 0b11], &Piece.Position);
            CornerA = Res;
            Total += Res;
            ++Count;

            Res = Collision(&SpinTable[Count & 0b11], &Piece.Position);
            CornerB = Res;
            Total += Res;
            ++Count;

            Res = Collision(&SpinTable[Count & 0b11], &Piece.Position);
            Total += Res;
            ++Count;

            Res = Collision(&SpinTable[Count & 0b11], &Piece.Position);
            Total += Res;

            if(Total >= 3)
            {
                PFlags.Set(PhysFlags::TSpin);
                if(CornerA & CornerB || Kick == 4){PFlags.Unset(PhysFlags::TSpinMini);}
                else{PFlags.Set(PhysFlags::TSpinMini);}
            }
        }
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
    RenderData.Flags.Set(RenderFlags::PIECE);
    PFlags.Unset(PhysFlags::TSpin | PhysFlags::TSpinMini);
    LockPiece();
}

void Board::ClearLines()
{
    int8 LinesCleared = 0;
    for(int8 Line = 3; Line >= 0; Line--)
    {
        if((Piece.Position.y + Line) >= 0)
        {
            if(
            Matrix[Piece.Position.y + Line][0] && Matrix[Piece.Position.y + Line][1] &&
            Matrix[Piece.Position.y + Line][2] && Matrix[Piece.Position.y + Line][3] &&
            Matrix[Piece.Position.y + Line][4] && Matrix[Piece.Position.y + Line][5] &&
            Matrix[Piece.Position.y + Line][6] && Matrix[Piece.Position.y + Line][7] &&
            Matrix[Piece.Position.y + Line][8] && Matrix[Piece.Position.y + Line][9])
            {
                ++LinesCleared;
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

    int AddedPoints = 0;
    if(PFlags.Get(PhysFlags::TSpin) && !PFlags.Get(PhysFlags::TSpinMini))
    {
        AddedPoints = 400 * (LinesCleared + 1);
    }else
    {
        if(PFlags.Get(PhysFlags::TSpin))
        {
            AddedPoints += 100;
        }

        switch (LinesCleared)
        {
            case 1:
                AddedPoints += 100;
            break;
            case 2:
                AddedPoints += 300;
            break;
            case 3:
                AddedPoints += 500;
            break;
            case 4:
                AddedPoints += 800;
            break;
            default:
            break;
        }
    }

    Points += AddedPoints * Level;
    Lines += LinesCleared;

    RenderData.Flags.Set(RenderFlags::LINES);
    RenderData.Flags.Set(RenderFlags::POINTS);
    RenderData.Flags.Set(RenderFlags::MATRIX);

    if(Level < MaxLevel)
    {
        Level = Lines / 10 + StartLevel;
        RenderData.Flags.Set(RenderFlags::LEVEL);
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

    PFlags.Unset
    (
        PhysFlags::HardDrop |
        PhysFlags::RCW |
        PhysFlags::RCCW |
        PhysFlags::TSpin
    );
    PFlags.Set(PhysFlags::Drop);

    RenderData.Flags.Set(RenderFlags::NEXT);
    RenderData.Flags.Set(RenderFlags::PIECESPAWN);
    if(Collision(Piece.Blocks, Piece.Position.x, Piece.Position.y))
    {
        Alive = false;
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
    RenderData.Flags.Set(RenderFlags::MATRIX);
    if(HeldPiece)
    {
        int8 Temp;
        Temp = HeldPiece;
        HeldPiece = Piece.Type;
        RenderData.Flags.Set(RenderFlags::HOLD);

        Piece.Spawn(Temp);
        RenderData.Flags.Set(RenderFlags::PIECESPAWN);
        if(Collision(Piece.Blocks, Piece.Position.x, Piece.Position.y))
        {
            return 1;
        }

        PFlags.Unset
        (
            PhysFlags::HardDrop |
            PhysFlags::RCW |
            PhysFlags::RCCW |
            PhysFlags::TSpin
        );
        PFlags.Set(PhysFlags::Drop);

        Phys.DropLag = 0;
        LockPhys.X = Piece.Position.x;
        LockPhys.Y = Piece.Position.y;
        LockPhys.R = Piece.Rotation;
        LockPhys.MoveCount = 15;
        LockPhys.TimerSet = 0;
        CanHold = false;
    }else
    {
        HeldPiece = Piece.Type;
        RenderData.Flags.Set(RenderFlags::HOLD);
        SpawnPiece();
        CanHold = false;
    }
    return 0;
}

void Board::StartGame()
{
    RenderData.Flags.Set(0x7fff);
    Init();
    GenerateBags(0);
    GenerateBags(1);
    GetSpeed();
    SpawnPiece();
}

}
