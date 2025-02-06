#include "Classes.hpp"

namespace Tetris::Game
{
    void Board::Render()
    {
        if(RenderData.Flags.Get(RenderFlags::MATRIX))
        {
            RenderMatrix();
        }
        /*
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
        */

        RenderData.Flags.Clear();
    }

    void Board::RenderMatrix()
    {
        using namespace Render;
        HDC hdc = Layer->hdc;

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SelectObject(hdc, GetStockObject(NULL_PEN));

        SetDCBrushColor(hdc, RGB(96,96,96));
        Rectangle(hdc, 270, 35, 530, 570);
    }

    void Board::RenderBkgd()
    {
        using namespace Render;
        Render::RenderBkgd(Layer, Color::Gray);
    }
}