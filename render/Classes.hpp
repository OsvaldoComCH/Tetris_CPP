#ifndef TETRISRENDER
#define TETRISRENDER

#include <Windows.h>
#include <vector>
#include "../headers/Constants.hpp"

namespace Tetris::Render
{
    class Layer
    {
        public:
        HDC hdc;
        HBITMAP Bmp;
        struct {int x,y,w,h;} Area;

        Layer(){}
        Layer(HDC ParentDC, int x, int y, int w, int h)
        {
            this->Initialize(ParentDC, x, y, w, h);
        }

        void Initialize(HDC ParentDC, int x, int y, int w, int h)
        {
            hdc = CreateCompatibleDC(ParentDC);
            Bmp = CreateCompatibleBitmap(ParentDC, w, h);
            SelectObject(hdc, Bmp);
            Area = {x, y, w, h};
        }

        ~Layer()
        {
            DeleteDC(hdc);
            DeleteObject(Bmp);
        }
    };

    std::vector<Layer *> _Layers;
    Layer MainLayer;

    void InitLayers()
    {
        HDC hdc = GetDC(hwnd);
        MainLayer.Initialize(hdc, 0, 0, 800, 600);
        ReleaseDC(hwnd, hdc);
    }

    Layer * GetLayer()
    {
        return _Layers.back();
    }

    Layer * GetLayer(int Layer)
    {
        if(Layer >= _Layers.size()){Layer = _Layers.size() - 1;}
        return _Layers[Layer];
    }

    Layer * CreateLayer(int x, int y, int w, int h)
    {
        Layer * L = new Layer(MainLayer.hdc, x, y, w, h);
        _Layers.push_back(L);
        return L;
    }

    void DestroyLayer()
    {
        delete _Layers.back();
        _Layers.pop_back();
    }

    void DestroyLayer(Layer * L)
    {
        for(int i = 0; i < _Layers.size(); ++i)
        {
            if(_Layers[i] == L)
            {
                _Layers.erase(_Layers.begin() + i);
            }
        }
    }

    void TransferToWindow()
    {
        HDC WndDC = GetDC(hwnd);
        SetStretchBltMode(WndDC, HALFTONE);
        StretchBlt
        (
            WndDC, 0, 0, 160 * CFG.WindowSize, 120 * CFG.WindowSize,
            MainLayer.hdc, 0, 0, 800, 600, SRCCOPY
        );
        ReleaseDC(hwnd, WndDC);
    }

    void TransferTopLayer()
    {
        Layer * L = _Layers.back();
        BitBlt(MainLayer.hdc, L->Area.x, L->Area.y, L->Area.w, L->Area.h, L->hdc, 0, 0, SRCCOPY);
    }

    void TransferLayer(unsigned Layer)
    {
        if(Layer >= _Layers.size()){Layer = _Layers.size() - 1;}
        Render::Layer * L = _Layers[Layer];
        BitBlt(MainLayer.hdc, L->Area.x, L->Area.y, L->Area.w, L->Area.h, L->hdc, 0, 0, SRCCOPY);
    }

    void TransferLayer(Layer * L)
    {
        BitBlt(MainLayer.hdc, L->Area.x, L->Area.y, L->Area.w, L->Area.h, L->hdc, 0, 0, SRCCOPY);
    }
    
    void TransferAllLayers()
    {
        for(int i = 0; i < _Layers.size(); ++i)
        {
            Layer * L = _Layers[i];
            BitBlt(MainLayer.hdc, L->Area.x, L->Area.y, L->Area.w, L->Area.h, L->hdc, 0, 0, SRCCOPY);
        }
    }
   
    void RenderBkgd()
    {
        Layer * L = GetLayer(0);
        SelectObject(L->hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(L->hdc, Color::Gray);
        SelectObject(L->hdc, GetStockObject(NULL_PEN));
        Rectangle(L->hdc, -1, -1, L->Area.w + 1, L->Area.h + 1);
    }

    void RenderBkgd(Layer * L, COLORREF Color)
    {
        HGDIOBJ OldBrush = SelectObject(L->hdc, GetStockObject(DC_BRUSH));
        HGDIOBJ OldPen = SelectObject(L->hdc, GetStockObject(NULL_PEN));

        SetDCBrushColor(L->hdc, Color);
        Rectangle(L->hdc, -1, -1, L->Area.w + 1, L->Area.h + 1);

        SelectObject(L->hdc, OldBrush);
        SelectObject(L->hdc, OldPen);
    }
}

#endif