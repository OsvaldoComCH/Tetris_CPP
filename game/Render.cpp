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
        /*
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
        */

        RenderData.Flags.Clear();
    }

    void Board::RenderLayout()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));

        SetDCBrushColor(hdc, RGB(96,96,96));
        Rectangle(hdc, 270, 35, 530, 570);
        Rectangle(hdc, 140, 45, 260, 165);
        Rectangle(hdc, 540, 45, 660, 165);
        Rectangle(hdc, 550, 170, 650, 500);

        SetDCBrushColor(hdc, Color::DarkGray);
        Rectangle(hdc, 275, 40, 525, 565);
        Rectangle(hdc, 145, 50, 255, 160);
        Rectangle(hdc, 545, 50, 655, 160);
        Rectangle(hdc, 555, 175, 645, 495);
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
                DrawBlock(hdc, RenderData.X + RenderData.Block[i][0], RenderData.Y + RenderData.Block[i][1]);
            }
            for(int8 i = 0; i < 4; ++i)
            {
                if(RenderData.Shadow + RenderData.Block[i][1] > 20){continue;}
                DrawBlock(hdc, RenderData.X + RenderData.Block[i][0], RenderData.Shadow + RenderData.Block[i][1]);
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
        //SelectObject(hdc, ShadowPens[Piece.Type + 1]);
        SetDCBrushColor(hdc, ShadowColors[Piece.Type - 1]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderData.Shadow + RenderData.Block[i][1] > 20){continue;}
            DrawBlock(hdc, RenderData.X + RenderData.Block[i][0], RenderData.Shadow + RenderData.Block[i][1]);
        }
        SelectObject(hdc, PiecePen);
        SetDCBrushColor(hdc, PieceColors[Piece.Type]);
        for(int8 i = 0; i < 4; ++i)
        {
            if(RenderData.Y + RenderData.Block[i][1] > 20){continue;}
            DrawBlock(hdc, RenderData.X + RenderData.Block[i][0], RenderData.Y + RenderData.Block[i][1]);
        }
    }

    void Board::RenderBkgd()
    {
        using namespace Render;
        Render::RenderBkgd(Layer, Color::Gray);
    }
}