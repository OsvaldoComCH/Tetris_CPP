#ifndef TETRISCONSTANTS
#define TETRISCONSTANTS

#include "Config.hpp"
#include <Windows.h>

namespace Tetris
{
    Config CFG;
    Config TempCFG;
    HWND hwnd;

    namespace Render
    {
        static const HFONT DefFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

        static constexpr COLORREF Colors[10] = {
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
        static constexpr COLORREF ShadowColors[7] = {
            RGB(64,96,96),
            RGB(96,64,96),
            RGB(96,96,64),
            RGB(128,96,32),
            RGB(72,72,112),
            RGB(112,72,72),
            RGB(72,112,72)
        };
        
        enum Color
        {
            DarkGray=Colors[0],
            Cyan=Colors[1],
            Purple=Colors[2],
            Yellow=Colors[3],
            Orange=Colors[4],
            Blue=Colors[5],
            Red=Colors[6],
            Green=Colors[7],
            Gray=Colors[8],
            White=Colors[9]
        };

        void ScaleBlt(HDC DestDC, HDC SrcDC, RECT * TransferArea)
        {
            SetStretchBltMode(DestDC, HALFTONE);

            float Scale = ((float)CFG.WindowSize) / 5;
            StretchBlt
            (
                DestDC,
                TransferArea->left * Scale,
                TransferArea->top * Scale,
                (TransferArea->right - TransferArea->left) * Scale,
                (TransferArea->bottom - TransferArea->top) * Scale,
                SrcDC,
                0, 0,
                TransferArea->right - TransferArea->left,
                TransferArea->bottom - TransferArea->top,
                SRCCOPY
            );
        }

        static void FillWndBkgd(HDC hdc)
        {
            HGDIOBJ OldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(hdc, Color::Gray);
            SelectObject(hdc, GetStockObject(NULL_PEN));
            RECT R;
            GetClientRect(hwnd, &R);
            Rectangle(hdc, R.left-1, R.top-1, R.right+1, R.bottom+1);
            SelectObject(hdc, OldBrush);
        }
    };

    void ApplyCfgChanges()
    {
        if(CFG.WindowSize != TempCFG.WindowSize)
        {
            RECT R;
            GetClientRect(hwnd, &R);
            POINT P = {R.right >> 1, R.bottom >> 1};
            ClientToScreen(hwnd, &P);

            int w = 160 * TempCFG.WindowSize;
            int h = 120 * TempCFG.WindowSize;
            
            R.left = P.x - (w >> 1);
            R.top = P.y - (h >> 1);
            R.right = R.left + w;
            R.bottom = R.top + h;

            AdjustWindowRectEx(&R, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), false, 0);

            w = R.right - R.left;
            h = R.bottom - R.top;
            if(R.left < 0){R.left = 0;}
            if(R.top < 0){R.top = 0;}

            SetWindowPos(hwnd, NULL, R.left, R.top, w, h, SWP_SHOWWINDOW);
        }

        CFG = TempCFG;
        WriteConfigFile(&CFG);
    }
}

#endif