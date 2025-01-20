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
            GetWindowRect(hwnd, &R);
            int w = 160 * TempCFG.WindowSize;
            int h = 120 * TempCFG.WindowSize;
            R.right = R.left + w;
            R.bottom = R.top + h;

            AdjustWindowRectEx(&R, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), false, 0);
            SetWindowPos(hwnd, NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_SHOWWINDOW);
        }
    }
}

#endif