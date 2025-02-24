#include "Classes.hpp"

namespace Tetris::Game
{
    void Board::Render()
    {
        if(RenderData.Flags.Get(RenderFlags::LAYOUT))
        {
            RenderLayout();
        }
        if(RenderData.Flags.Get(RenderFlags::MATRIX))
        {
            RenderMatrix();
        }
        if(RenderData.Flags.Get(RenderFlags::PIECESPAWN))
        {
            RenderPiece(1);
        }
        if(RenderData.Flags.Get(RenderFlags::PIECE))
        {
            RenderPiece(0);
        }
        if(RenderData.Flags.Get(RenderFlags::HOLD))
        {
            RenderHold();
        }
        if(RenderData.Flags.Get(RenderFlags::NEXT))
        {
            RenderNext();
        }
        if(RenderData.Flags.Get(RenderFlags::LINES))
        {
            RenderLines();
        }
        if(RenderData.Flags.Get(RenderFlags::LEVEL))
        {
            RenderLevel();
        }
        if(RenderData.Flags.Get(RenderFlags::COVER))
        {
            RenderCover();
        }

        RenderData.Flags.Clear();
    }

    void Board::RenderLayout()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, DefFont);

        SetDCBrushColor(hdc, RGB(96,96,96));
        Rectangle(hdc, 270, 35, 530, 570);
        Rectangle(hdc, 140, 45, 260, 165);
        Rectangle(hdc, 140, 340, 260, 550);
        Rectangle(hdc, 540, 45, 660, 165);
        Rectangle(hdc, 550, 170, 650, 440);

        SetDCBrushColor(hdc, Color::DarkGray);
        Rectangle(hdc, 275, 40, 525, 565);
        Rectangle(hdc, 145, 50, 255, 160);
        Rectangle(hdc, 145, 345, 255, 545);
        Rectangle(hdc, 545, 50, 655, 160);
        Rectangle(hdc, 555, 175, 645, 435);
        
        SetDCBrushColor(hdc, RGB(96,96,96));
        Rectangle(hdc, 150, 385, 250, 415);
        Rectangle(hdc, 150, 445, 250, 475);
        Rectangle(hdc, 150, 505, 250, 535);

        SetTextColor(hdc, Color::Black);
        SetBkColor(hdc, Color::Gray);
        SetTextAlign(hdc, TA_CENTER);
        TextOut(hdc, 200, 20, L"HOLD", 4);
        TextOut(hdc, 600, 20, L"NEXT", 4);

        SelectObject(hdc, StatsFont);
        SetTextColor(hdc, Color::LightGray);
        SetBkColor(hdc, Color::DarkGray);
        SetTextAlign(hdc, TA_LEFT);
        TextOut(hdc, 150, 360, L"Level", 5);
        TextOut(hdc, 150, 420, L"Lines", 5);
        TextOut(hdc, 150, 480, L"Points", 6);
    }

    void Board::RenderCover()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, DefFont);


        SetDCBrushColor(hdc, Color::DarkGray);
        Rectangle(hdc, 275, 40, 526, 566);
        Rectangle(hdc, 145, 50, 255, 160);
        Rectangle(hdc, 545, 50, 655, 160);
        Rectangle(hdc, 555, 175, 645, 435);
    }

    void Board::RenderMatrix()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        for(int y = 0; y < 21; ++y)
        {
            for(int x = 0; x < 10; ++x)
            {
                int8 Blk = Matrix[y][x];
                if(Blk){SelectObject(hdc, PiecePen);}
                else{SelectObject(hdc, BkgdPen);}

                SetDCBrushColor(hdc, PieceColors[Blk]);

                DrawBlock(hdc, x, y);
            }
        }
    }

    void Board::RenderPiece(bool Spawn)
    {
        using namespace Render;
        HDC hdc = Layer->hdc;
        
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        if(!Spawn)
        {
            SetDCBrushColor(hdc, PieceColors[0]);
            SelectObject(hdc, BkgdPen);
            for(int8 i = 0; i < 4; ++i)
            {
                if(RenderData.Y + RenderData.Block[i][1] > 20){continue;}
                DrawBlock
                (
                    hdc,
                    RenderData.X + RenderData.Block[i][0],
                    RenderData.Y + RenderData.Block[i][1]
                );
            }
            for(int8 i = 0; i < 4; ++i)
            {
                if(RenderData.Shadow + RenderData.Block[i][1] > 20){continue;}
                DrawBlock
                (
                    hdc,
                    RenderData.X + RenderData.Block[i][0],
                    RenderData.Shadow + RenderData.Block[i][1]
                );
            }
        }

        RenderData.Block = Piece.Blocks;
        RenderData.X = Piece.Position.x;
        RenderData.Y = Piece.Position.y;

        int8 i = 0;
        while(++i)
        {
            if(CollisionDown(Piece.Blocks, Piece.Position.x, Piece.Position.y - i))
            {
                break;
            }
        }
        RenderData.Shadow = Piece.Position.y - i + 1;
        SelectObject(hdc, ShadowPens[Piece.Type - 1]);
        SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderData.Shadow + RenderData.Block[i][1] > 20){continue;}
            DrawBlock
            (
                hdc,
                RenderData.X + RenderData.Block[i][0],
                RenderData.Shadow + RenderData.Block[i][1]
            );
        }
        SelectObject(hdc, PiecePen);
        SetDCBrushColor(hdc, PieceColors[Piece.Type]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderData.Y + RenderData.Block[i][1] > 20){continue;}
            DrawBlock
            (
                hdc,
                RenderData.X + RenderData.Block[i][0],
                RenderData.Y + RenderData.Block[i][1]
            );
        }
    }

    void Board::RenderHold()
    {
        using namespace Render;
        if(!HeldPiece){return;}

        HDC hdc = Layer->hdc;
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));

        SetDCBrushColor(hdc, Color::DarkGray);
        Rectangle(hdc, 145, 50, 255, 160);

        Block Blk;
        GetPieceBlocks(HeldPiece, Blk);
        short OriginX, OriginY;

        if(HeldPiece == 1)
        {
            OriginX = 150;
            OriginY = 167;
        }else
        if(HeldPiece == 3)
        {
            OriginX = 150;
            OriginY = 180;
        }else
        {
            OriginX = 162;
            OriginY = 180;
        }

        SelectObject(hdc, PiecePen);
        SetDCBrushColor(hdc, PieceColors[HeldPiece]);

        for(int i = 0; i < 4; ++i)
        {
            short DeslocX = Blk[i].x * 25;
            short DeslocY = Blk[i].y * 25;

            Rectangle
            (
                hdc,
                OriginX + DeslocX,
                OriginY - DeslocY,
                OriginX + DeslocX + 25,
                OriginY - DeslocY - 25
            );
        }
    }

    void Board::RenderNext()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));

        SetDCBrushColor(hdc, Color::DarkGray);
        Rectangle(hdc, 545, 50, 655, 160);
        Rectangle(hdc, 555, 175, 645, 435);

        int8 Piece = NextPieces[NextPointer];

        Block Blk;
        GetPieceBlocks(Piece, Blk);
        short OriginX, OriginY;

        if(Piece == 1)
        {
            OriginX = 550;
            OriginY = 167;
        }else
        if(Piece == 3)
        {
            OriginX = 550;
            OriginY = 180;
        }else
        {
            OriginX = 562;
            OriginY = 180;
        }

        SelectObject(hdc, PiecePen);
        SetDCBrushColor(hdc, PieceColors[Piece]);

        for(int i = 0; i < 4; ++i)
        {
            short DeslocX = Blk[i].x * 25;
            short DeslocY = Blk[i].y * 25;

            Rectangle
            (
                hdc,
                OriginX + DeslocX,
                OriginY - DeslocY,
                OriginX + DeslocX + 25,
                OriginY - DeslocY - 25
            );
        }

        
        int8 Next = NextPointer;
        for(int i = 0; i < 4; ++i)
        {
            ++Next;
            if(Next >= 14){Next -= 14;}
            int8 Piece = NextPieces[Next];

            GetPieceBlocks(Piece, Blk);
            short OriginX, OriginY;

            if(Piece == 1)
            {
                OriginX = 560;
                OriginY = 265 + 60 * i;
            }else
            if(Piece == 3)
            {
                OriginX = 560;
                OriginY = 275 + 60 * i;
            }else
            {
                OriginX = 570;
                OriginY = 275 + 60 * i;
            }
    
            SelectObject(hdc, PiecePen);
            SetDCBrushColor(hdc, PieceColors[Piece]);
    
            for(int i = 0; i < 4; ++i)
            {
                short DeslocX = Blk[i].x * 20;
                short DeslocY = Blk[i].y * 20;
    
                Rectangle
                (
                    hdc,
                    OriginX + DeslocX,
                    OriginY - DeslocY,
                    OriginX + DeslocX + 20,
                    OriginY - DeslocY - 20
                );
            }            
        }
    }

    void Board::RenderLines()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, StatsFont);
        SetTextColor(hdc, Color::White);
        SetBkColor(hdc, RGB(96,96,96));
        SetTextAlign(hdc, TA_CENTER);
        std::wstring Str = std::to_wstring(Lines);
        TextOut(hdc, 200, 450, Str.c_str(), Str.length());
    }

    void Board::RenderLevel()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, StatsFont);
        SetTextColor(hdc, Color::White);
        SetBkColor(hdc, RGB(96,96,96));
        SetTextAlign(hdc, TA_CENTER);
        std::wstring Str = std::to_wstring(Level);
        TextOut(hdc, 200, 390, Str.c_str(), Str.length());
    }

    void Board::RenderPoints()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, StatsFont);
        SetTextColor(hdc, Color::White);
        SetBkColor(hdc, RGB(96,96,96));
        SetTextAlign(hdc, TA_CENTER);
        std::wstring Str = std::to_wstring(Points);
        TextOut(hdc, 200, 510, Str.c_str(), Str.length());
    }

    void Board::RenderBkgd()
    {
        using namespace Render;
        Render::RenderBkgd(Layer, Color::Gray);
    }
}