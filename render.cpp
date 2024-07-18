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

void CBoard::RenderBkgd()
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    SelectObject(Ghdc, GetStockObject(NULL_PEN));
    SetDCBrushColor(Ghdc, RGB(128,128,128));
    Rectangle(Ghdc, 0, 0, 700, 700);
    
    SelectObject(Ghdc, GetStockObject(DC_PEN));
    
    SetDCBrushColor(Ghdc, RGB(255,255,255));

    //Exterior
    Rectangle(Ghdc, 155, 25, 475, 675);//Matrix
    Rectangle(Ghdc, 485, 70, 605, 190);//Next
    Rectangle(Ghdc, 495, 190, 595, 530);//Next 5
    Rectangle(Ghdc, 485, 570, 675, 640);//Points
    Rectangle(Ghdc, 25, 70, 145, 190);//Hold
    Rectangle(Ghdc, 25, 400, 145, 470);//Level
    Rectangle(Ghdc, 25, 500, 145, 570);//Lines

    //Interior
    SetDCBrushColor(Ghdc, RGB(48,48,48));
   
    SelectObject(Ghdc, GetStockObject(NULL_PEN));
    Rectangle(Ghdc, 164, 34, 466, 666);//Matrix
    SelectObject(Ghdc, GetStockObject(DC_PEN));
    Rectangle(Ghdc, 494, 79, 596, 181);//Next
    Rectangle(Ghdc, 505, 200, 585, 520);//Next 5
    Rectangle(Ghdc, 495, 580, 665, 630);//Points
    Rectangle(Ghdc, 34, 79, 136, 181);//Hold
    Rectangle(Ghdc, 35, 410, 135, 460);//Level
    Rectangle(Ghdc, 35, 510, 135, 560);//Lines

    //Text
    SelectObject(Ghdc, Font);
    SetTextAlign(Ghdc, TA_LEFT);
    SetBkColor(Ghdc, RGB(128, 128, 128));
    SetTextColor(Ghdc, RGB(0,0,0));
    TextOut(Ghdc, 513, 40, L"NEXT", 4);
    TextOut(Ghdc, 53, 40, L"HOLD", 4);
    TextOut(Ghdc, 495, 540, L"POINTS", 6);
    TextOut(Ghdc, 35, 370, L"LEVEL", 5);
    TextOut(Ghdc, 35, 470, L"LINES", 5);
    
    RenderLines();
    RenderMatrix();
    RenderNext();
    RenderPiece(0);
}

void CBoard::RenderLines()
{
    SelectObject(Ghdc, Font2);
    SetTextAlign(Ghdc, TA_RIGHT);
    SetBkColor(Ghdc, RGB(48,48,48));
    SetTextColor(Ghdc, Colors[9]);
    wstring WLines = std::to_wstring(Lines);
    TextOut(Ghdc, 125, 517, WLines.c_str(), WLines.length());
}

void CBoard::RenderMatrix()
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    for(int8 y = 0; y < 21; ++y)
    {
        for(int8 x = 0; x < 10; ++x)
        {
            if(Matrix[y][x]){SelectObject(Ghdc, Pens[0]);}
            else{SelectObject(Ghdc, Pens[1]);}
            SetDCBrushColor(Ghdc, Colors[Matrix[y][x]]);
            DrawBlock(x, y);
        }
    }
}

void CBoard::RenderPiece(bool Spawn)
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    if(!Spawn)
    {
        SetDCBrushColor(Ghdc, Colors[0]);
        SelectObject(Ghdc, Pens[1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderY + RenderBlock.Pos[i][1] > 20){continue;}
            DrawBlock(RenderX + RenderBlock.Pos[i][0], RenderY + RenderBlock.Pos[i][1]);
        }
        for(int8 i = 0; i < 4; ++i)
        {
            if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
            DrawBlock(RenderX + RenderBlock.Pos[i][0], ShadowY + RenderBlock.Pos[i][1]);
        }
    }

    RenderBlock = Piece.Block;
    RenderX = Piece.Position[0];
    RenderY = Piece.Position[1];

    int8 i = 0;
    while(++i)
    {
        if(CollisionDown(Piece.Block, Piece.Position[0], Piece.Position[1]-i))
        {
            break;
        }
    }
    ShadowY = Piece.Position[1] - i + 1;
    SelectObject(Ghdc, Pens[Piece.Type + 1]);
    SetDCBrushColor(Ghdc, ShadowColors[Piece.Type - 1]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(ShadowY + RenderBlock.Pos[i][1] > 20){continue;}
        DrawBlock(RenderX + RenderBlock.Pos[i][0], ShadowY + RenderBlock.Pos[i][1]);
    }
    SelectObject(Ghdc, Pens[0]);
    SetDCBrushColor(Ghdc, Colors[Piece.Type]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(RenderY + RenderBlock.Pos[i][1] > 20){continue;}
        DrawBlock(RenderX + RenderBlock.Pos[i][0], RenderY + RenderBlock.Pos[i][1]);
    }
}

void CBoard::FlashPiece()
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    SelectObject(Ghdc, Pens[0]);
    SetDCBrushColor(Ghdc, Colors[9]);
    for(int8 i = 0; i < 4; ++i)
    {
        if(Piece.Position[1] + Piece.Block.Pos[i][1] < 21)
        {
            DrawBlock(
                Piece.Position[0] + Piece.Block.Pos[i][0],
                Piece.Position[1] + Piece.Block.Pos[i][1]
            );
        }
    }
}

void CBoard::RenderNext()
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    SelectObject(Ghdc, GetStockObject(DC_PEN));
    SetDCBrushColor(Ghdc, RGB(48,48,48));
    Rectangle(Ghdc, 494, 79, 596, 181);//Next
    Rectangle(Ghdc, 505, 200, 585, 520);//Next 5
    SetDCBrushColor(Ghdc, Colors[NextPieces[NextPointer]]);
    SelectObject(Ghdc, Pens[0]);
    switch(NextPieces[NextPointer])
    {
        case 1:
            Rectangle(Ghdc, 495, 117, 520, 142);
            Rectangle(Ghdc, 520, 117, 545, 142);
            Rectangle(Ghdc, 545, 117, 570, 142);
            Rectangle(Ghdc, 570, 117, 595, 142);
        break;
        case 2:
            Rectangle(Ghdc, 508, 130, 533, 155);
            Rectangle(Ghdc, 533, 130, 558, 155);
            Rectangle(Ghdc, 558, 130, 583, 155);
            Rectangle(Ghdc, 533, 105, 558, 130);
        break;
        case 3:
            Rectangle(Ghdc, 520, 130, 545, 155);
            Rectangle(Ghdc, 545, 130, 570, 155);
            Rectangle(Ghdc, 520, 105, 545, 130);
            Rectangle(Ghdc, 545, 105, 570, 130);
        break;
        case 4:
            Rectangle(Ghdc, 508, 130, 533, 155);
            Rectangle(Ghdc, 533, 130, 558, 155);
            Rectangle(Ghdc, 558, 130, 583, 155);
            Rectangle(Ghdc, 558, 105, 583, 130);
        break;
        case 5:
            Rectangle(Ghdc, 508, 130, 533, 155);
            Rectangle(Ghdc, 533, 130, 558, 155);
            Rectangle(Ghdc, 558, 130, 583, 155);
            Rectangle(Ghdc, 508, 105, 533, 130);
        break;
        case 6:
            Rectangle(Ghdc, 508, 105, 533, 130);
            Rectangle(Ghdc, 533, 130, 558, 155);
            Rectangle(Ghdc, 558, 130, 583, 155);
            Rectangle(Ghdc, 533, 105, 558, 130);
        break;
        case 7:
            Rectangle(Ghdc, 508, 130, 533, 155);
            Rectangle(Ghdc, 533, 130, 558, 155);
            Rectangle(Ghdc, 558, 105, 583, 130);
            Rectangle(Ghdc, 533, 105, 558, 130);
        break;
    }
    int8 TempType;
    SelectObject(Ghdc, GetStockObject(DC_PEN));
    for(int8 x = 0; x < 4; ++x)
    {
        TempType = NextPieces[(NextPointer + 1 + x) % 14];
        SetDCBrushColor(Ghdc, Colors[TempType]);
        switch(TempType)
        {
            case 1:
                Rectangle(Ghdc, 505, 230 + (x*80), 525, 250 + (x*80));
                Rectangle(Ghdc, 525, 230 + (x*80), 545, 250 + (x*80));
                Rectangle(Ghdc, 545, 230 + (x*80), 565, 250 + (x*80));
                Rectangle(Ghdc, 565, 230 + (x*80), 585, 250 + (x*80));
            break;
            case 2:
                Rectangle(Ghdc, 515, 240 + (x*80), 535, 260 + (x*80));
                Rectangle(Ghdc, 535, 240 + (x*80), 555, 260 + (x*80));
                Rectangle(Ghdc, 555, 240 + (x*80), 575, 260 + (x*80));
                Rectangle(Ghdc, 535, 220 + (x*80), 555, 240 + (x*80));
            break;
            case 3:
                Rectangle(Ghdc, 525, 220 + (x*80), 545, 240 + (x*80));
                Rectangle(Ghdc, 545, 220 + (x*80), 565, 240 + (x*80));
                Rectangle(Ghdc, 525, 240 + (x*80), 545, 260 + (x*80));
                Rectangle(Ghdc, 545, 240 + (x*80), 565, 260 + (x*80));
            break;
            case 4:
                Rectangle(Ghdc, 515, 240 + (x*80), 535, 260 + (x*80));
                Rectangle(Ghdc, 535, 240 + (x*80), 555, 260 + (x*80));
                Rectangle(Ghdc, 555, 240 + (x*80), 575, 260 + (x*80));
                Rectangle(Ghdc, 555, 220 + (x*80), 575, 240 + (x*80));
            break;
            case 5:
                Rectangle(Ghdc, 515, 240 + (x*80), 535, 260 + (x*80));
                Rectangle(Ghdc, 535, 240 + (x*80), 555, 260 + (x*80));
                Rectangle(Ghdc, 555, 240 + (x*80), 575, 260 + (x*80));
                Rectangle(Ghdc, 515, 220 + (x*80), 535, 240 + (x*80));
            break;
            case 6:
                Rectangle(Ghdc, 515, 220 + (x*80), 535, 240 + (x*80));
                Rectangle(Ghdc, 535, 240 + (x*80), 555, 260 + (x*80));
                Rectangle(Ghdc, 555, 240 + (x*80), 575, 260 + (x*80));
                Rectangle(Ghdc, 535, 220 + (x*80), 555, 240 + (x*80));
            break;
            case 7:
                Rectangle(Ghdc, 515, 240 + (x*80), 535, 260 + (x*80));
                Rectangle(Ghdc, 535, 240 + (x*80), 555, 260 + (x*80));
                Rectangle(Ghdc, 555, 220 + (x*80), 575, 240 + (x*80));
                Rectangle(Ghdc, 535, 220 + (x*80), 555, 240 + (x*80));
            break;
        }
    }
}

void CBoard::RenderHold()
{
    SelectObject(Ghdc, GetStockObject(DC_BRUSH));
    SelectObject(Ghdc, GetStockObject(DC_PEN));
    SetDCBrushColor(Ghdc, RGB(48,48,48));
    Rectangle(Ghdc, 34, 79, 136, 181);
    SetDCBrushColor(Ghdc, Colors[HeldPiece]);
    SelectObject(Ghdc, Pens[0]);
    switch(HeldPiece)
    {
        case 1:
            Rectangle(Ghdc, 135, 117, 110, 142);
            Rectangle(Ghdc, 110, 117, 85, 142);
            Rectangle(Ghdc, 85, 117, 60, 142);
            Rectangle(Ghdc, 60, 117, 35, 142);
        break;
        case 2:
            Rectangle(Ghdc, 122, 130, 97, 155);
            Rectangle(Ghdc, 97, 130, 72, 155);
            Rectangle(Ghdc, 72, 130, 47, 155);
            Rectangle(Ghdc, 97, 105, 72, 130);
        break;
        case 3:
            Rectangle(Ghdc, 110, 130, 85, 155);
            Rectangle(Ghdc, 85, 130, 60, 155);
            Rectangle(Ghdc, 110, 105, 85, 130);
            Rectangle(Ghdc, 85, 105, 60, 130);
        break;
        case 4:
            Rectangle(Ghdc, 122, 130, 97, 155);
            Rectangle(Ghdc, 97, 130, 72, 155);
            Rectangle(Ghdc, 72, 130, 47, 155);
            Rectangle(Ghdc, 122, 105, 97, 130);
        break;
        case 5:
            Rectangle(Ghdc, 122, 130, 97, 155);
            Rectangle(Ghdc, 97, 130, 72, 155);
            Rectangle(Ghdc, 72, 130, 47, 155);
            Rectangle(Ghdc, 72, 105, 47, 130);
        break;
        case 6:
            Rectangle(Ghdc, 122, 130, 97, 155);
            Rectangle(Ghdc, 97, 130, 72, 155);
            Rectangle(Ghdc, 72, 105, 47, 130);
            Rectangle(Ghdc, 97, 105, 72, 130);
        break;
        case 7:
            Rectangle(Ghdc, 122, 105, 97, 130);
            Rectangle(Ghdc, 97, 130, 72, 155);
            Rectangle(Ghdc, 72, 130, 47, 155);
            Rectangle(Ghdc, 97, 105, 72, 130);
        break;
    }
}

void CBoard::FlashLine(int8 Line)
{
    for(int8 x = 0; x < 10; ++x)
    {
        SelectObject(Ghdc, Pens[0]);
        SetDCBrushColor(Ghdc, Colors[9]);
        DrawBlock(x, Line);
    }
}

void RenderScreen()
{
    HDC hdc = GetDC(Ghwnd);
    RECT ScreenRect;
    POINT ScreenCenter, DrawPos;
    SetStretchBltMode(hdc, HALFTONE);
    GetClientRect(Ghwnd, &ScreenRect);
    int Width = ScreenRect.right - ScreenRect.left;
    int Height = ScreenRect.bottom - ScreenRect.top;
    ScreenCenter.x = Width >> 1;
    ScreenCenter.y = Height >> 1;
    if(!Height){Height = 1;}
    float ScreenRatio = Width / Height;
    if(ScreenRatio >= 1)//Draw area ratio
    {
        DrawPos.y = 0;
        DrawPos.x = ScreenCenter.x - ScreenCenter.y;
        StretchBlt(hdc, DrawPos.x, DrawPos.y, Height, Height,
        Player1.Ghdc, 0, 0, 700, 700, SRCCOPY);
    }else
    {
        DrawPos.x = 0;
        DrawPos.y = ScreenCenter.y - ScreenCenter.x;
        StretchBlt(hdc, DrawPos.x, DrawPos.y, Width, Width,
        Player1.Ghdc, 0, 0, 700, 700, SRCCOPY);
    }

    /*if(CBoard::Mode == 2)
    {
        HDC PauseDC = CreateCompatibleDC(hdc);
        HBITMAP Bitmap = CreateCompatibleBitmap(hdc, 240, 320);
        SelectObject(PauseDC, Bitmap);

        DeleteObject(Bitmap);
        DeleteDC(PauseDC);
    }*/

    ReleaseDC(Ghwnd, hdc);
}