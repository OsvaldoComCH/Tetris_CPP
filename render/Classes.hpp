#ifndef TETRISRENDER
#define TETRISRENDER

#include <Windows.h>
#include <vector>
#include <cstdarg>
#include "../headers/Constants.hpp"

namespace Tetris::Render
{
    class Layer
    {
        public:
        HDC hdc;
        HBITMAP Bmp;
        RECT Area;

        Layer(HDC ParentDC, int x, int y, int w, int h)
        {
            hdc = CreateCompatibleDC(ParentDC);
            Bmp = CreateCompatibleBitmap(ParentDC, w, h);
            SelectObject(hdc, Bmp);
            Area = {x, y, x + w, y + x};
        }

        ~Layer()
        {
            DeleteDC(hdc);
            DeleteObject(Bmp);
        }
    };

    std::vector<Layer *> _Layers;
    Layer MainLayer;

    void InitLayers(HDC hdc)
    {
        MainLayer = Layer(hdc, 0, 0, 800, 600);
    }

    Layer * GetLayer()
    {
        return _Layers.back();
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

    void TransferLayers(int Layer, ...)
    {
        va_list ArgList;
    }
}

#endif