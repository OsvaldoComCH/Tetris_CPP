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
        HFONT DefFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

        void TetrisBlt(HDC DestDC, HDC SrcDC, RECT * TransferArea)
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