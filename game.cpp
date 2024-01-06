#include "input.cpp"
void CBoard::MoveDown()
{
    if(!ColisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-1))
    {
        Piece.Position[1]--;
        RenderPiece(0);
    }
}
void CBoard::MoveLeft()
{
    if(!ColisionLeft(Piece.Block, Piece.Position[0]-1, Piece.Position[1]))
    {
        Piece.Position[0]--;
        RenderShadow(0);
        RenderPiece(0);
    }
}
void CBoard::MoveRight()
{
    if(!ColisionRight(Piece.Block, Piece.Position[0]+1, Piece.Position[1]))
    {
        ++Piece.Position[0];
        RenderShadow(0);
        RenderPiece(0);
    }
}
void CBoard::HardDrop()
{
    int8 i = 0;
    while(++i)
    {
        if(ColisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    Piece.Position[1] -= i - 1;
    RenderPiece(0);
    LockPiece();
}
int8 CBoard::RotatePiece(bool Dir)
{
    SBlock TempBlock;
    int8 TempRotation;
    if(!Dir)
    {
        TempRotation = Piece.Rotation + 1;
        if(TempRotation > 3){TempRotation = 0;}
    }
    else
    {
        TempRotation = Piece.Rotation - 1;
        if(TempRotation < 0){TempRotation = 3;}
    }
    switch(Piece.Type)
    {
        case 1://I
            switch(TempRotation)
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
            switch(TempRotation)
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
            switch(TempRotation)
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
            switch(TempRotation)
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
            switch(TempRotation)
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
            switch(TempRotation)
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
            switch(TempRotation)
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
    if(!ColisionFull(TempBlock, Piece.Position[0], Piece.Position[1]))
    {} else
    if(!ColisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[Piece.Rotation][Dir][0][0],
    Piece.Position[1] + Piece.Kick.Data[Piece.Rotation][Dir][0][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[Piece.Rotation][Dir][0][0];
        Piece.Position[1] += Piece.Kick.Data[Piece.Rotation][Dir][0][1];
    } else
    if(!ColisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[Piece.Rotation][Dir][1][0],
    Piece.Position[1] + Piece.Kick.Data[Piece.Rotation][Dir][1][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[Piece.Rotation][Dir][1][0];
        Piece.Position[1] += Piece.Kick.Data[Piece.Rotation][Dir][1][1];
    } else
    if(!ColisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[Piece.Rotation][Dir][2][0],
    Piece.Position[1] + Piece.Kick.Data[Piece.Rotation][Dir][2][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[Piece.Rotation][Dir][2][0];
        Piece.Position[1] += Piece.Kick.Data[Piece.Rotation][Dir][2][1];
    } else
    if(!ColisionFull(TempBlock, Piece.Position[0] + Piece.Kick.Data[Piece.Rotation][Dir][3][0],
    Piece.Position[1] + Piece.Kick.Data[Piece.Rotation][Dir][3][1]))
    {
        Piece.Position[0] += Piece.Kick.Data[Piece.Rotation][Dir][3][0];
        Piece.Position[1] += Piece.Kick.Data[Piece.Rotation][Dir][3][1];
    } else {return 1;}
    Piece.Block = TempBlock;
    Piece.Rotation = TempRotation;
    RenderShadow(0);
    RenderPiece(0);
    return 0;
}
void CBoard::LockPiece()
{
    FlashPiece();
    for(int8 i = 0; i < 4; ++i)
    {
        Matrix[Piece.Position[0]+Piece.Block.Pos[i][0]][Piece.Position[1]+Piece.Block.Pos[i][1]]=Piece.Type;
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
        Matrix[0][Piece.Position[1] + x] && Matrix[1][Piece.Position[1] + x] &&
        Matrix[2][Piece.Position[1] + x] && Matrix[3][Piece.Position[1] + x] &&
        Matrix[4][Piece.Position[1] + x] && Matrix[5][Piece.Position[1] + x] &&
        Matrix[6][Piece.Position[1] + x] && Matrix[7][Piece.Position[1] + x] &&
        Matrix[8][Piece.Position[1] + x] && Matrix[9][Piece.Position[1] + x])
        {
            FlashLine(Piece.Position[1] + x);
            for(int8 i = (Piece.Position[1] + x); i < 39; ++i)
            {
                for(int8 j = 0; j < 10; ++j)
                {
                    Matrix[j][i] = Matrix[j][i+1];
                }
            }
            for(int8 j = 0; j < 10; ++j)
            {
                Matrix[j][39] = 0;
            }
        }}
    }
    Sleep(200);
    RenderMatrix();
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
    RenderNext();
    if(ColisionFull(Piece.Block, Piece.Position[0], Piece.Position[1]))
    {
        return 1;
    }
    RenderPiece(1);
    RenderShadow(1);
    return 0;
}
void StartGame()
{
    Player.RenderMatrix();
    Player.SpawnPiece();
    Input();
}