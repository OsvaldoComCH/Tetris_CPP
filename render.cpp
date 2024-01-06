#include "classes.cpp"

const HPEN Pens[9] = {
    CreatePen(PS_SOLID, 2, RGB(0,0,0)),
    CreatePen(PS_SOLID, 2, RGB(96,96,96)),
    CreatePen(PS_SOLID, 2, RGB(0,255,255)),
    CreatePen(PS_SOLID, 2, RGB(255,0,255)),
    CreatePen(PS_SOLID, 2, RGB(255,255,0)),
    CreatePen(PS_SOLID, 2, RGB(255,128,0)),
    CreatePen(PS_SOLID, 2, RGB(0,0,255)),
    CreatePen(PS_SOLID, 2, RGB(255,0,0)),
    CreatePen(PS_SOLID, 2, RGB(0,255,0))
};
const HFONT Font = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
const COLORREF Colors[10] = {
    RGB(64,64,64),
    RGB(0,224,224),
    RGB(224,0,224),
    RGB(224,224,0),
    RGB(224,112,0),
    RGB(0,0,224),
    RGB(224,0,0),
    RGB(0,224,0),
    RGB(128,128,128),
    RGB(255,255,255)
};
const COLORREF ShadowColors[7] = {
    RGB(64,96,96),
    RGB(96,64,96),
    RGB(96,96,64),
    RGB(128,96,32),
    RGB(72,72,112),
    RGB(112,72,72),
    RGB(72,112,72)
};
void RenderBkgd()
{
    PAINTSTRUCT PS;
    HDC hdc = BeginPaint(Ghwnd, &PS);
    //Exterior
    FillRect(hdc, &PS.rcPaint, CreateSolidBrush(RGB(128, 128, 128)));
    Rectangle(hdc, 320, 15, 640, 665);//Matrix
    Rectangle(hdc, 660, 60, 760, 520);//Next 5
    Rectangle(hdc, 650, 60, 770, 180);//Next
    Rectangle(hdc, 650, 560, 850, 630);//Points
    Rectangle(hdc, 190, 60, 310, 180);//Hold
    Rectangle(hdc, 190, 390, 310, 460);//Level
    Rectangle(hdc, 190, 490, 310, 560);//Lines
    //Interior
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(48,48,48));
    Rectangle(hdc, 670, 190, 750, 510);//Next 5
    Rectangle(hdc, 660, 70, 760, 170);//Next
    Rectangle(hdc, 660, 570, 840, 620);//Points
    Rectangle(hdc, 200, 70, 300, 170);//Hold
    Rectangle(hdc, 200, 400, 300, 450);//Level
    Rectangle(hdc, 200, 500, 300, 550);//Lines
    SelectObject(hdc, CreatePen(PS_NULL, 1, 0x00000000));
    Rectangle(hdc, 329, 24, 631, 656);//Matrix
    //Text
    SelectObject(hdc, Font);
    SetBkColor(hdc, RGB(128, 128, 128));
    TextOut(hdc, 678, 30, L"NEXT", 4);
    TextOut(hdc, 218, 30, L"HOLD", 4);
    TextOut(hdc, 660, 530, L"POINTS", 6);
    TextOut(hdc, 200, 360, L"LEVEL", 5);
    TextOut(hdc, 200, 460, L"LINES", 5);
    EndPaint(Ghwnd, &PS);
}

void CBoard::RenderBkgd(HDC hdc)
{
    RECT R;
    GetClientRect(Ghwnd, &R);
    FillRect(hdc, &R, CreateSolidBrush(RGB(128, 128, 128)));

    //Exterior
    Rectangle(hdc, Pos.x - 160, Pos.y - 345, Pos.x + 160, Pos.y + 305);//Matrix
    Rectangle(hdc, Pos.x + 180, Pos.y - 300, Pos.x + 280, Pos.y + 160);//Next 5
    Rectangle(hdc, Pos.x + 170, Pos.y - 300, Pos.x + 290, Pos.y - 180);//Next
    Rectangle(hdc, Pos.x + 170, Pos.y + 200, Pos.x + 370, Pos.y + 270);//Points
    Rectangle(hdc, Pos.x - 290, Pos.y - 300, Pos.x - 170, Pos.y - 180);//Hold
    Rectangle(hdc, Pos.x - 290, Pos.y + 30, Pos.x - 170, Pos.y + 100);//Level
    Rectangle(hdc, Pos.x - 290, Pos.y + 130, Pos.x - 170, Pos.y + 200);//Lines

    //Interior
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(48,48,48));
    Rectangle(hdc, Pos.x + 190, Pos.y - 170, Pos.x + 270, Pos.y + 150);//Next 5
    Rectangle(hdc, Pos.x + 180, Pos.y - 290, Pos.x + 280, Pos.y - 190);//Next
    Rectangle(hdc, Pos.x + 180, Pos.y + 210, Pos.x + 360, Pos.y + 260);//Points
    Rectangle(hdc, Pos.x - 280, Pos.y - 290, Pos.x - 180, Pos.y - 190);//Hold
    Rectangle(hdc, Pos.x - 280, Pos.y + 40, Pos.x - 180, Pos.y + 90);//Level
    Rectangle(hdc, Pos.x - 280, Pos.y + 140, Pos.x - 180, Pos.y + 190);//Lines
    SelectObject(hdc, CreatePen(PS_NULL, 1, 0x00000000));
    Rectangle(hdc, Pos.x - 151, Pos.y - 336, Pos.x + 151, Pos.y + 296);//Matrix

    //Text
    SelectObject(hdc, Font);
    SetBkColor(hdc, RGB(128, 128, 128));
    TextOut(hdc, Pos.x + 198, Pos.y - 330, L"NEXT", 4);
    TextOut(hdc, Pos.x - 262, Pos.y - 330, L"HOLD", 4);
    TextOut(hdc, Pos.x + 180, Pos.y + 170, L"POINTS", 6);
    TextOut(hdc, Pos.x - 280, Pos.y, L"LEVEL", 5);
    TextOut(hdc, Pos.x - 280, Pos.y + 100, L"LINES", 5);
    
    RenderMatrix();
    RenderNext();
    RenderShadow(0);
    RenderPiece(0);
}
/*void RenderBoard()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    for(int8 y = 0; y < 21; ++y)
    {
        for(int8 x = 0; x < 10; ++x)
        {
            if(Matrix[x][y]){SelectObject(hdc, Pens[0]);}
            else{SelectObject(hdc, Pens[1]);}
            SetDCBrushColor(hdc, Colors[Matrix[x][y]]);
            Rectangle(hdc, 331+(x*30), 626-(y*30), 360+(x*30), 655-(y*30));
        }
    }
    ReleaseDC(Ghwnd, hdc);
}*/

void CBoard::RenderMatrix()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    for(int8 y = 0; y < 21; ++y)
    {
        for(int8 x = 0; x < 10; ++x)
        {
            if(Matrix[x][y]){SelectObject(hdc, Pens[0]);}
            else{SelectObject(hdc, Pens[1]);}
            SetDCBrushColor(hdc, Colors[Matrix[x][y]]);
            Rectangle(hdc, Pos.x-149+(x*30), Pos.y+266-(y*30), Pos.x-120+(x*30), Pos.y+295-(y*30));
        }
    }
    ReleaseDC(Ghwnd, hdc);
}
/*void RenderPiece(int8 Mode)//0-default, 1-erase, 2-flash
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    if(!Mode)
    {
        SelectObject(hdc, Pens[0]);
        SetDCBrushColor(hdc, Colors[Piece.Type]);
    }else
    if(Mode == 1)
    {
        SelectObject(hdc, Pens[1]);
        SetDCBrushColor(hdc, Colors[0]);
    }else
    {
        SelectObject(hdc, Pens[0]);
        SetDCBrushColor(hdc, Colors[9]);
    }
    for(int8 i = 0; i < 4; ++i)
    {
        if(Piece.Position[1] + Piece.Block.Pos[i][1] < 21)
        {
            Rectangle(
                hdc,
                331+((Piece.Position[0] + Piece.Block.Pos[i][0])*30),
                626-((Piece.Position[1] + Piece.Block.Pos[i][1])*30),
                360+((Piece.Position[0] + Piece.Block.Pos[i][0])*30),
                655-((Piece.Position[1] + Piece.Block.Pos[i][1])*30)
            );
        }
    }
    ReleaseDC(Ghwnd, hdc);
}*/

void CBoard::RenderPiece(bool Spawn)
{
    HDC hdc = GetDC(Ghwnd);
    static SBlock PrevBlock;
    static int8 XPos, YPos;
    SelectObject(hdc, GetStockObject(DC_BRUSH));

    if(!Spawn)
    {
        SetDCBrushColor(hdc, Colors[0]);
        SelectObject(hdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(YPos + PrevBlock.Pos[i][1] > 20){continue;}
            Rectangle(
                hdc,
                Pos.x-149+((XPos + PrevBlock.Pos[i][0])*30),
                Pos.y+266-((YPos + PrevBlock.Pos[i][1])*30),
                Pos.x-120+((XPos + PrevBlock.Pos[i][0])*30),
                Pos.y+295-((YPos + PrevBlock.Pos[i][1])*30)
            );
        }
    }
    PrevBlock = Piece.Block;
    XPos = Piece.Position[0];
    YPos = Piece.Position[1];

    SelectObject(hdc, Pens[0]);
    SetDCBrushColor(hdc, Colors[Piece.Type]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(YPos + PrevBlock.Pos[i][1] > 20){continue;}
        Rectangle(
            hdc,
            Pos.x-149+((XPos + PrevBlock.Pos[i][0])*30),
            Pos.y+266-((YPos + PrevBlock.Pos[i][1])*30),
            Pos.x-120+((XPos + PrevBlock.Pos[i][0])*30),
            Pos.y+295-((YPos + PrevBlock.Pos[i][1])*30)
        );
    }
    ReleaseDC(Ghwnd, hdc);
}
void CBoard::FlashPiece()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SelectObject(hdc, Pens[0]);
    SetDCBrushColor(hdc, Colors[9]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(Piece.Position[1] + Piece.Block.Pos[i][1] < 21)
        {
            Rectangle(
                hdc,
                Pos.x -149 +((Piece.Position[0] + Piece.Block.Pos[i][0])*30),
                Pos.y +266 -((Piece.Position[1] + Piece.Block.Pos[i][1])*30),
                Pos.x -120 +((Piece.Position[0] + Piece.Block.Pos[i][0])*30),
                Pos.y +295 -((Piece.Position[1] + Piece.Block.Pos[i][1])*30)
            );
        }
    }
    ReleaseDC(Ghwnd, hdc);
}
/*void RenderNext()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(48,48,48));
    Rectangle(hdc, 659, 69, 761, 171);
    Rectangle(hdc, 669, 189, 751, 511);
    SetDCBrushColor(hdc, Colors[NextPieces[NextPointer]]);
    SelectObject(hdc, Pens[0]);
    switch(NextPieces[NextPointer])
    {
        case 1:
            Rectangle(hdc, 660, 107, 685, 132);
            Rectangle(hdc, 685, 107, 710, 132);
            Rectangle(hdc, 710, 107, 735, 132);
            Rectangle(hdc, 735, 107, 760, 132);
        break;
        case 2:
            Rectangle(hdc, 673, 120, 698, 145);
            Rectangle(hdc, 698, 120, 723, 145);
            Rectangle(hdc, 723, 120, 748, 145);
            Rectangle(hdc, 698, 95, 723, 120);
        break;
        case 3:
            Rectangle(hdc, 685, 120, 710, 145);
            Rectangle(hdc, 710, 120, 735, 145);
            Rectangle(hdc, 685, 95, 710, 120);
            Rectangle(hdc, 710, 95, 735, 120);
        break;
        case 4:
            Rectangle(hdc, 673, 120, 698, 145);
            Rectangle(hdc, 698, 120, 723, 145);
            Rectangle(hdc, 723, 120, 748, 145);
            Rectangle(hdc, 723, 95, 748, 120);
        break;
        case 5:
            Rectangle(hdc, 673, 120, 698, 145);
            Rectangle(hdc, 698, 120, 723, 145);
            Rectangle(hdc, 723, 120, 748, 145);
            Rectangle(hdc, 673, 95, 698, 120);
        break;
        case 6:
            Rectangle(hdc, 673, 95, 698, 120);
            Rectangle(hdc, 698, 120, 723, 145);
            Rectangle(hdc, 723, 120, 748, 145);
            Rectangle(hdc, 698, 95, 723, 120);
        break;
        case 7:
            Rectangle(hdc, 673, 120, 698, 145);
            Rectangle(hdc, 698, 120, 723, 145);
            Rectangle(hdc, 723, 95, 748, 120);
            Rectangle(hdc, 698, 95, 723, 120);
        break;
    }
    int8 TempType;
    SelectObject(hdc, GetStockObject(DC_PEN));
    for(int8 x = 0; x < 4; ++x)
    {
        TempType = NextPieces[(NextPointer + 1 + x) % 14];
        SetDCBrushColor(hdc, Colors[TempType]);
        switch(TempType)
        {//Rectangle(hdc, 670, 190, 750, 510);
            case 1:
                Rectangle(hdc, 670, 220 + (x*80), 690, 240 + (x*80));
                Rectangle(hdc, 690, 220 + (x*80), 710, 240 + (x*80));
                Rectangle(hdc, 710, 220 + (x*80), 730, 240 + (x*80));
                Rectangle(hdc, 730, 220 + (x*80), 750, 240 + (x*80));
            break;
            case 2:
                Rectangle(hdc, 680, 230 + (x*80), 700, 250 + (x*80));
                Rectangle(hdc, 700, 230 + (x*80), 720, 250 + (x*80));
                Rectangle(hdc, 720, 230 + (x*80), 740, 250 + (x*80));
                Rectangle(hdc, 700, 210 + (x*80), 720, 230 + (x*80));
            break;
            case 3:
                Rectangle(hdc, 690, 230 + (x*80), 710, 250 + (x*80));
                Rectangle(hdc, 690, 210 + (x*80), 710, 230 + (x*80));
                Rectangle(hdc, 710, 230 + (x*80), 730, 250 + (x*80));
                Rectangle(hdc, 710, 210 + (x*80), 730, 230 + (x*80));
            break;
            case 4:
                Rectangle(hdc, 680, 230 + (x*80), 700, 250 + (x*80));
                Rectangle(hdc, 700, 230 + (x*80), 720, 250 + (x*80));
                Rectangle(hdc, 720, 230 + (x*80), 740, 250 + (x*80));
                Rectangle(hdc, 720, 210 + (x*80), 740, 230 + (x*80));
            break;
            case 5:
                Rectangle(hdc, 680, 230 + (x*80), 700, 250 + (x*80));
                Rectangle(hdc, 700, 230 + (x*80), 720, 250 + (x*80));
                Rectangle(hdc, 720, 230 + (x*80), 740, 250 + (x*80));
                Rectangle(hdc, 680, 210 + (x*80), 700, 230 + (x*80));
            break;
            case 6:
                Rectangle(hdc, 680, 210 + (x*80), 700, 230 + (x*80));
                Rectangle(hdc, 700, 230 + (x*80), 720, 250 + (x*80));
                Rectangle(hdc, 720, 230 + (x*80), 740, 250 + (x*80));
                Rectangle(hdc, 700, 210 + (x*80), 720, 230 + (x*80));
            break;
            case 7:
                Rectangle(hdc, 680, 230 + (x*80), 700, 250 + (x*80));
                Rectangle(hdc, 700, 230 + (x*80), 720, 250 + (x*80));
                Rectangle(hdc, 720, 210 + (x*80), 740, 230 + (x*80));
                Rectangle(hdc, 700, 210 + (x*80), 720, 230 + (x*80));
            break;
        }
    }
    ReleaseDC(Ghwnd, hdc);
}*/

void CBoard::RenderNext()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(48,48,48));
    Rectangle(hdc, Pos.x + 179, Pos.y - 291, Pos.x + 281, Pos.y - 189);
    Rectangle(hdc, Pos.x + 189, Pos.y - 171, Pos.x + 271, Pos.y + 149);
    SetDCBrushColor(hdc, Colors[NextPieces[NextPointer]]);
    SelectObject(hdc, Pens[0]);
    switch(NextPieces[NextPointer])
    {
        case 1:
            Rectangle(hdc, Pos.x + 180, Pos.y - 253, Pos.x + 205, Pos.y - 228);
            Rectangle(hdc, Pos.x + 205, Pos.y - 253, Pos.x + 230, Pos.y - 228);
            Rectangle(hdc, Pos.x + 230, Pos.y - 253, Pos.x + 255, Pos.y - 228);
            Rectangle(hdc, Pos.x + 255, Pos.y - 253, Pos.x + 280, Pos.y - 228);
        break;
        case 2:
            Rectangle(hdc, Pos.x + 193, Pos.y - 240, Pos.x + 218, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 240, Pos.x + 243, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 240, Pos.x + 268, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 265, Pos.x + 243, Pos.y - 240);
        break;
        case 3:
            Rectangle(hdc, Pos.x + 205, Pos.y - 240, Pos.x + 230, Pos.y - 215);
            Rectangle(hdc, Pos.x + 230, Pos.y - 240, Pos.x + 255, Pos.y - 215);
            Rectangle(hdc, Pos.x + 205, Pos.y - 265, Pos.x + 230, Pos.y - 240);
            Rectangle(hdc, Pos.x + 230, Pos.y - 265, Pos.x + 255, Pos.y - 240);
        break;
        case 4:
            Rectangle(hdc, Pos.x + 193, Pos.y - 240, Pos.x + 218, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 240, Pos.x + 243, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 240, Pos.x + 268, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 265, Pos.x + 268, Pos.y - 240);
        break;
        case 5:
            Rectangle(hdc, Pos.x + 193, Pos.y - 240, Pos.x + 218, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 240, Pos.x + 243, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 240, Pos.x + 268, Pos.y - 215);
            Rectangle(hdc, Pos.x + 193, Pos.y - 265, Pos.x + 218, Pos.y - 240);
        break;
        case 6:
            Rectangle(hdc, Pos.x + 193, Pos.y - 265, Pos.x + 218, Pos.y - 240);
            Rectangle(hdc, Pos.x + 218, Pos.y - 240, Pos.x + 243, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 240, Pos.x + 268, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 265, Pos.x + 243, Pos.y - 240);
        break;
        case 7:
            Rectangle(hdc, Pos.x + 193, Pos.y - 240, Pos.x + 218, Pos.y - 215);
            Rectangle(hdc, Pos.x + 218, Pos.y - 240, Pos.x + 243, Pos.y - 215);
            Rectangle(hdc, Pos.x + 243, Pos.y - 265, Pos.x + 268, Pos.y - 240);
            Rectangle(hdc, Pos.x + 218, Pos.y - 265, Pos.x + 243, Pos.y - 240);
        break;
    }
    int8 TempType;
    SelectObject(hdc, GetStockObject(DC_PEN));
    for(int8 x = 0; x < 4; ++x)
    {
        //Pos = {480, 360}
        TempType = NextPieces[(NextPointer + 1 + x) % 14];
        SetDCBrushColor(hdc, Colors[TempType]);
        switch(TempType)
        {
            case 1:
                Rectangle(hdc, Pos.x + 190, Pos.y - 140 + (x*80), Pos.x + 210, Pos.y - 120 + (x*80));
                Rectangle(hdc, Pos.x + 210, Pos.y - 140 + (x*80), Pos.x + 230, Pos.y - 120 + (x*80));
                Rectangle(hdc, Pos.x + 230, Pos.y - 140 + (x*80), Pos.x + 250, Pos.y - 120 + (x*80));
                Rectangle(hdc, Pos.x + 250, Pos.y - 140 + (x*80), Pos.x + 270, Pos.y - 120 + (x*80));
            break;
            case 2:
                Rectangle(hdc, Pos.x + 200, Pos.y - 130 + (x*80), Pos.x + 220, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 130 + (x*80), Pos.x + 240, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 130 + (x*80), Pos.x + 260, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 150 + (x*80), Pos.x + 240, Pos.y - 130 + (x*80));
            break;
            case 3:
                Rectangle(hdc, Pos.x + 210, Pos.y - 150 + (x*80), Pos.x + 230, Pos.y - 130 + (x*80));
                Rectangle(hdc, Pos.x + 230, Pos.y - 150 + (x*80), Pos.x + 250, Pos.y - 130 + (x*80));
                Rectangle(hdc, Pos.x + 210, Pos.y - 130 + (x*80), Pos.x + 230, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 230, Pos.y - 130 + (x*80), Pos.x + 250, Pos.y - 110 + (x*80));
            break;
            case 4:
                Rectangle(hdc, Pos.x + 200, Pos.y - 130 + (x*80), Pos.x + 220, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 130 + (x*80), Pos.x + 240, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 130 + (x*80), Pos.x + 260, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 150 + (x*80), Pos.x + 260, Pos.y - 130 + (x*80));
            break;
            case 5:
                Rectangle(hdc, Pos.x + 200, Pos.y - 130 + (x*80), Pos.x + 220, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 130 + (x*80), Pos.x + 240, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 130 + (x*80), Pos.x + 260, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 200, Pos.y - 150 + (x*80), Pos.x + 220, Pos.y - 130 + (x*80));
            break;
            case 6:
                Rectangle(hdc, Pos.x + 200, Pos.y - 150 + (x*80), Pos.x + 220, Pos.y - 130 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 130 + (x*80), Pos.x + 240, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 130 + (x*80), Pos.x + 260, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 150 + (x*80), Pos.x + 240, Pos.y - 130 + (x*80));
            break;
            case 7:
                Rectangle(hdc, Pos.x + 200, Pos.y - 130 + (x*80), Pos.x + 220, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 130 + (x*80), Pos.x + 240, Pos.y - 110 + (x*80));
                Rectangle(hdc, Pos.x + 240, Pos.y - 150 + (x*80), Pos.x + 260, Pos.y - 130 + (x*80));
                Rectangle(hdc, Pos.x + 220, Pos.y - 150 + (x*80), Pos.x + 240, Pos.y - 130 + (x*80));
            break;
        }
    }
    ReleaseDC(Ghwnd, hdc);
}
/*void FlashLine(int8 Line)
{
    HDC hdc = GetDC(Ghwnd);
    for(int8 x = 0; x < 10; ++x)
    {
        //Pos = {480, 360}
        SelectObject(hdc, Pens[0]);
        SetDCBrushColor(hdc, Colors[9]);
        Rectangle(hdc, 331+(x*30), 626-(Line*30), 360+(x*30), 655-(Line*30));
    }
    ReleaseDC(Ghwnd, hdc);
}*/

void CBoard::FlashLine(int8 Line)
{
    HDC hdc = GetDC(Ghwnd);
    for(int8 x = 0; x < 10; ++x)
    {
        //Pos = {480, 360}
        SelectObject(hdc, Pens[0]);
        SetDCBrushColor(hdc, Colors[9]);
        Rectangle(hdc, Pos.x-149+(x*30), Pos.y+266-(Line*30), Pos.x-120+(x*30), Pos.y+295-(Line*30));
    }
    ReleaseDC(Ghwnd, hdc);
}
/*void RenderShadow(bool Spawn)
{
    static SBlock Shadow;
    static int8 XPos, YPos;
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, Colors[0]);
    //erase the previous shadow
    if(!Spawn)
    {
        SelectObject(hdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            Rectangle(
                hdc,
                331+((XPos + Shadow.Pos[i][0])*30),
                626-((YPos + Shadow.Pos[i][1])*30),
                360+((XPos + Shadow.Pos[i][0])*30),
                655-((YPos + Shadow.Pos[i][1])*30)
            );
        }
        Shadow = Piece.Block;
    }else
    {
        Shadow = Piece.Block;
    }
    int8 i = 0;
    while(++i)
    {
        if(ColisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    XPos = Piece.Position[0];
    YPos = Piece.Position[1] - i + 1;
    SelectObject(hdc, Pens[Piece.Type + 1]);
    SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
    for(int8 i = 0; i < 4; ++i)
    {
        Rectangle(
            hdc,
            331+((XPos + Shadow.Pos[i][0])*30),
            626-((YPos + Shadow.Pos[i][1])*30),
            360+((XPos + Shadow.Pos[i][0])*30),
            655-((YPos + Shadow.Pos[i][1])*30)
        );
    }
    ReleaseDC(Ghwnd, hdc);
}*/

void CBoard::RenderShadow(bool Spawn)
{
    HDC hdc = GetDC(Ghwnd);
    static SBlock Shadow;
    static int8 XPos, YPos;
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, Colors[0]);
    //erase the previous shadow
    if(!Spawn)
    {
        SelectObject(hdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(YPos + Shadow.Pos[i][1] > 20){continue;}
            Rectangle(
                hdc,
                Pos.x-149+((XPos + Shadow.Pos[i][0])*30),
                Pos.y+266-((YPos + Shadow.Pos[i][1])*30),
                Pos.x-120+((XPos + Shadow.Pos[i][0])*30),
                Pos.y+295-((YPos + Shadow.Pos[i][1])*30)
            );
        }
        Shadow = Piece.Block;
    }else
    {
        Shadow = Piece.Block;
    }
    int8 i = 0;
    while(++i)
    {
        if(ColisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    XPos = Piece.Position[0];
    YPos = Piece.Position[1] - i + 1;
    SelectObject(hdc, Pens[Piece.Type + 1]);
    SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(YPos + Shadow.Pos[i][1] > 20){continue;}
        Rectangle(
            hdc,
            Pos.x-149+((XPos + Shadow.Pos[i][0])*30),
            Pos.y+266-((YPos + Shadow.Pos[i][1])*30),
            Pos.x-120+((XPos + Shadow.Pos[i][0])*30),
            Pos.y+295-((YPos + Shadow.Pos[i][1])*30)
        );
    }
    ReleaseDC(Ghwnd, hdc);
}