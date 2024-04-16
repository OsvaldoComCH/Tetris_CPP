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
OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL),
Font2 = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
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
    SelectObject(hdc, Font2);
    SetTextAlign(hdc, TA_RIGHT);
    SetBkColor(hdc, RGB(48,48,48));
    SetTextColor(hdc, Colors[9]);
    wstring WLines = std::to_wstring(Lines);
    TextOut(hdc, Pos.x - 190, Pos.y + 147, WLines.c_str(), WLines.length());
    
    RenderMatrix();
    RenderNext();
    RenderShadow(0);
    RenderPiece(0);
}
void CBoard::RenderLines()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, Font2);
    SetTextAlign(hdc, TA_RIGHT);
    SetBkColor(hdc, RGB(48,48,48));
    SetTextColor(hdc, Colors[9]);
    wstring WLines = std::to_wstring(Lines);
    TextOut(hdc, Pos.x - 190, Pos.y + 147, WLines.c_str(), WLines.length());
    ReleaseDC(Ghwnd, hdc);
}
void CBoard::RenderMatrix()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    for(int8 y = 0; y < 21; ++y)
    {
        for(int8 x = 0; x < 10; ++x)
        {
            if(Matrix[y][x]){SelectObject(hdc, Pens[0]);}
            else{SelectObject(hdc, Pens[1]);}
            SetDCBrushColor(hdc, Colors[Matrix[y][x]]);
            Rectangle(hdc, Pos.x-149+(x*30), Pos.y+266-(y*30), Pos.x-120+(x*30), Pos.y+295-(y*30));
        }
    }
    ReleaseDC(Ghwnd, hdc);
}
void CBoard::RenderPiece(bool Spawn)
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    bool ShadowFlag;
    if(!Spawn)
    {
        SetDCBrushColor(hdc, Colors[0]);
        SelectObject(hdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderY + RenderBlock.Pos[i][1] > 20){continue;}
            Rectangle(
                hdc,
                Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+266-((RenderY + RenderBlock.Pos[i][1])*30),
                Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+295-((RenderY + RenderBlock.Pos[i][1])*30)
            );
        }
        ShadowFlag = (RenderX != Piece.Position[0] && RenderR != Piece.Rotation);
        if(ShadowFlag)
        {
            for(int8 i = 0; i < 4; ++i)
            {
                if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
                Rectangle(
                    hdc,
                    Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
                    Pos.y+266-((ShadowY + RenderBlock.Pos[i][1])*30),
                    Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
                    Pos.y+295-((ShadowY + RenderBlock.Pos[i][1])*30)
                );
            }
        }
    }else{ShadowFlag = 1;}

    RenderBlock = Piece.Block;
    RenderX = Piece.Position[0];
    RenderY = Piece.Position[1];
    RenderR = Piece.Rotation;
    
    if(ShadowFlag)
    {
        int8 i = 0;
        while(++i)
        {
            if(CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
            {
                break;
            }
        }
        ShadowY = Piece.Position[1] - i + 1;
        SelectObject(hdc, Pens[Piece.Type + 1]);
        SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
            Rectangle(
                hdc,
                Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+266-((ShadowY + RenderBlock.Pos[i][1])*30),
                Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+295-((ShadowY + RenderBlock.Pos[i][1])*30)
            );
        }
    }
    SelectObject(hdc, Pens[0]);
    SetDCBrushColor(hdc, Colors[Piece.Type]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(RenderY + RenderBlock.Pos[i][1] > 20){continue;}
        Rectangle(
            hdc,
            Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
            Pos.y+266-((RenderY + RenderBlock.Pos[i][1])*30),
            Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
            Pos.y+295-((RenderY + RenderBlock.Pos[i][1])*30)
        );
    }
    ReleaseDC(Ghwnd, hdc);
}
/*void CBoard::RenderShadow(bool Spawn)
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    if(!Spawn)
    {
        if(RenderX == Piece.Position[0] && RenderR == Piece.Rotation)
        {
            return;
        }
        SetDCBrushColor(hdc, Colors[0]);
        SelectObject(hdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
            Rectangle(
                hdc,
                Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+266-((ShadowY + RenderBlock.Pos[i][1])*30),
                Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
                Pos.y+295-((ShadowY + RenderBlock.Pos[i][1])*30)
            );
        }
    }
    RenderBlock = Piece.Block;
    RenderX = Piece.Position[0];
    RenderY = Piece.Position[1];
    RenderR = Piece.Rotation;
    RenderBlock = Piece.Block;
    int8 i = 0;
    while(++i)
    {
        if(CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    ShadowY = Piece.Position[1] - i + 1;
    SelectObject(hdc, Pens[Piece.Type + 1]);
    SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
        Rectangle(
            hdc,
            Pos.x-149+((RenderX + RenderBlock.Pos[i][0])*30),
            Pos.y+266-((ShadowY + RenderBlock.Pos[i][1])*30),
            Pos.x-120+((RenderX + RenderBlock.Pos[i][0])*30),
            Pos.y+295-((ShadowY + RenderBlock.Pos[i][1])*30)
        );
    }
    ReleaseDC(Ghwnd, hdc);
}*/
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
void CBoard::RenderHold()
{
    HDC hdc = GetDC(Ghwnd);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(48,48,48));
    Rectangle(hdc, Pos.x - 179, Pos.y - 291, Pos.x - 281, Pos.y - 189);
    SetDCBrushColor(hdc, Colors[HeldPiece]);
    SelectObject(hdc, Pens[0]);
    switch(HeldPiece)
    {
        case 1:
            Rectangle(hdc, Pos.x - 180, Pos.y - 253, Pos.x - 205, Pos.y - 228);
            Rectangle(hdc, Pos.x - 205, Pos.y - 253, Pos.x - 230, Pos.y - 228);
            Rectangle(hdc, Pos.x - 230, Pos.y - 253, Pos.x - 255, Pos.y - 228);
            Rectangle(hdc, Pos.x - 255, Pos.y - 253, Pos.x - 280, Pos.y - 228);
        break;
        case 2:
            Rectangle(hdc, Pos.x - 193, Pos.y - 240, Pos.x - 218, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 240, Pos.x - 243, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 240, Pos.x - 268, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 265, Pos.x - 243, Pos.y - 240);
        break;
        case 3:
            Rectangle(hdc, Pos.x - 205, Pos.y - 240, Pos.x - 230, Pos.y - 215);
            Rectangle(hdc, Pos.x - 230, Pos.y - 240, Pos.x - 255, Pos.y - 215);
            Rectangle(hdc, Pos.x - 205, Pos.y - 265, Pos.x - 230, Pos.y - 240);
            Rectangle(hdc, Pos.x - 230, Pos.y - 265, Pos.x - 255, Pos.y - 240);
        break;
        case 4:
            Rectangle(hdc, Pos.x - 193, Pos.y - 240, Pos.x - 218, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 240, Pos.x - 243, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 240, Pos.x - 268, Pos.y - 215);
            Rectangle(hdc, Pos.x - 193, Pos.y - 265, Pos.x - 218, Pos.y - 240);
        break;
        case 5:
            Rectangle(hdc, Pos.x - 193, Pos.y - 240, Pos.x - 218, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 240, Pos.x - 243, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 240, Pos.x - 268, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 265, Pos.x - 268, Pos.y - 240);
        break;
        case 6:
            Rectangle(hdc, Pos.x - 193, Pos.y - 240, Pos.x - 218, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 240, Pos.x - 243, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 265, Pos.x - 268, Pos.y - 240);
            Rectangle(hdc, Pos.x - 218, Pos.y - 265, Pos.x - 243, Pos.y - 240);
        break;
        case 7:
            Rectangle(hdc, Pos.x - 193, Pos.y - 265, Pos.x - 218, Pos.y - 240);
            Rectangle(hdc, Pos.x - 218, Pos.y - 240, Pos.x - 243, Pos.y - 215);
            Rectangle(hdc, Pos.x - 243, Pos.y - 240, Pos.x - 268, Pos.y - 215);
            Rectangle(hdc, Pos.x - 218, Pos.y - 265, Pos.x - 243, Pos.y - 240);
        break;
    }
}
void CBoard::FlashLine(int8 Line)
{
    HDC hdc = GetDC(Ghwnd);
    for(int8 x = 0; x < 10; ++x)
    {
        SelectObject(hdc, Pens[0]);
        SetDCBrushColor(hdc, Colors[9]);
        Rectangle(hdc, Pos.x-149+(x*30), Pos.y+266-(Line*30), Pos.x-120+(x*30), Pos.y+295-(Line*30));
    }
    ReleaseDC(Ghwnd, hdc);
}
