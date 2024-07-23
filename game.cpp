#include "input.cpp"
bool CBoard::MoveDown()
{
    if(!CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-1))
    {
        Piece.Position[1]--;
        RenderFlags |= RF_PIECE;
        return 0;
    }
    return 1;
}
void CBoard::MoveLeft()
{
    if(!CollisionLeft(Piece.Block, Piece.Position[0]-1, Piece.Position[1]))
    {
        Piece.Position[0]--;
        RenderFlags |= RF_PIECE;
    }
}
void CBoard::MoveRight()
{
    if(!CollisionRight(Piece.Block, Piece.Position[0]+1, Piece.Position[1]))
    {
        ++Piece.Position[0];
        RenderFlags |= RF_PIECE;
    }
}
void CBoard::HardDrop()
{
    int8 i = 0;
    while(++i)
    {
        if(CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    Piece.Position[1] -= i - 1;
    RenderFlags |= RF_PIECE;
    LockPiece();
}
int8 CBoard::RotatePiece(bool Dir)
{
    SBlock TempBlock;
    int8 OldRotation;
    if(!Dir)
    {
        OldRotation = Piece.Rotation;
        ++Piece.Rotation;
    }
    else
    {
        OldRotation = Piece.Rotation;
        --Piece.Rotation;
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
    if(!CollisionFull(TempBlock, Piece.Position[0], Piece.Position[1]))
    {}else
    if(!CollisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[OldRotation][Dir][0][0],
    Piece.Position[1] + Piece.Kick.Data[OldRotation][Dir][0][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[OldRotation][Dir][0][0];
        Piece.Position[1] += Piece.Kick.Data[OldRotation][Dir][0][1];
    }else
    if(!CollisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[OldRotation][Dir][1][0],
    Piece.Position[1] + Piece.Kick.Data[OldRotation][Dir][1][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[OldRotation][Dir][1][0];
        Piece.Position[1] += Piece.Kick.Data[OldRotation][Dir][1][1];
    }else
    if(!CollisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[OldRotation][Dir][2][0],
    Piece.Position[1] + Piece.Kick.Data[OldRotation][Dir][2][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[OldRotation][Dir][2][0];
        Piece.Position[1] += Piece.Kick.Data[OldRotation][Dir][2][1];
    }else
    if(!CollisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[OldRotation][Dir][3][0],
    Piece.Position[1] + Piece.Kick.Data[OldRotation][Dir][3][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[OldRotation][Dir][3][0];
        Piece.Position[1] += Piece.Kick.Data[OldRotation][Dir][3][1];
    }else
    {
        Piece.Rotation = OldRotation;
        return 1;
    }
    Piece.Block = TempBlock;
    RenderFlags |= RF_PIECE;
    return 0;
}
void CBoard::LockPiece()
{
    //FlashPiece();
    for(int8 i = 0; i < 4; ++i)
    {
        Matrix[Piece.Position[1]+Piece.Block.Pos[i][1]][Piece.Position[0]+Piece.Block.Pos[i][0]]=Piece.Type;
    }
    Piece.Type = 0;
    ClearLines();
    SpawnPiece();
}
void CBoard::ClearLines()
{
    for(int8 x = 3; x >= 0; x--)
    {
        if((Piece.Position[1] + x) >= 0){
            if(
            Matrix[Piece.Position[1] + x][0] && Matrix[Piece.Position[1] + x][1] &&
            Matrix[Piece.Position[1] + x][2] && Matrix[Piece.Position[1] + x][3] &&
            Matrix[Piece.Position[1] + x][4] && Matrix[Piece.Position[1] + x][5] &&
            Matrix[Piece.Position[1] + x][6] && Matrix[Piece.Position[1] + x][7] &&
            Matrix[Piece.Position[1] + x][8] && Matrix[Piece.Position[1] + x][9])
            {
                ++Lines;
                //FlashLine(Piece.Position[1] + x);
                for(int8 i = (Piece.Position[1] + x); i < 39; ++i)
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
    //Sleep(100);
    RenderFlags |= RF_LINES;
    RenderFlags |= RF_MATRIX;

    if(Level < Menu.MaxLevel)
    {
        Level = Lines / 10 + Menu.StartLevel;
        RenderFlags |= RF_LEVEL;
    }
    GetSpeed();
}
int8 CBoard::SpawnPiece()
{
    Piece = CPiece(NextPieces[NextPointer]);
    if(NextPointer == 13)
    {
        NextPointer = 0;
        GenBag(1);
    }else
    if(NextPointer == 6)
    {
        NextPointer += 1;
        GenBag(0);
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

    RenderFlags |= RF_NEXT;
    RenderFlags |= RF_PIECESPAWN;
    if(CollisionFull(Piece.Block, Piece.Position[0], Piece.Position[1]))
    {
        Player1.SetMode(0);
        return 1;
    }
    X = Piece.Position[0];
    Y = Piece.Position[1];
    R = Piece.Rotation;
    MoveCount = 15;
    TimerSet = 0;
    return 0;
}
int8 CBoard::Hold()
{
    RenderFlags |= RF_MATRIX;
    if(HeldPiece)
    {
        int8 Temp;
        Temp = HeldPiece;
        HeldPiece = Piece.Type;
        RenderFlags |= RF_HOLD;
        Piece = CPiece(Temp);
        RenderFlags |= RF_PIECESPAWN;
        if(CollisionFull(Piece.Block, Piece.Position[0], Piece.Position[1]))
        {
            Player1.SetMode(0);
            return 1;
        }
        X = Piece.Position[0];
        Y = Piece.Position[1];
        R = Piece.Rotation;
        MoveCount = 15;
        TimerSet = 0;
        CanHold = false;
    }else
    {
        HeldPiece = Piece.Type;
        RenderFlags |= RF_HOLD;
        SpawnPiece();
        CanHold = false;
    }
    return 0;
}
void CBoard::StartGame()
{
    Player1.SetMode(1);
    NextPointer = 0;
    Lines = 0;
    Level = Menu.StartLevel;
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
    RenderFlags = 255;
    InitMatrix();
    GenBag(0);
    GenBag(1);
    GetSpeed();
    SpawnPiece();
    Render();
    Input();
}
