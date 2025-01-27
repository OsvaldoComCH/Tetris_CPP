#ifndef TETRISCONSTANTS
#define TETRISCONSTANTS

#include "Config.hpp"
#include <Windows.h>

typedef signed char int8;

namespace Tetris
{
    Config CFG;
    Config TempCFG;
    HWND hwnd;

    namespace Render
    {
        static const HFONT DefFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
        static const HFONT BigFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

        static constexpr COLORREF PieceColors[8] = {
            RGB(64,64,64),
            RGB(0,224,224),
            RGB(224,0,224),
            RGB(224,224,0),
            RGB(224,112,0),
            RGB(0,0,224),
            RGB(224,0,0),
            RGB(0,224,0),
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
            DarkGray=PieceColors[0],
            Cyan=PieceColors[1],
            Purple=PieceColors[2],
            Yellow=PieceColors[3],
            Orange=PieceColors[4],
            Blue=PieceColors[5],
            Red=PieceColors[6],
            Green=PieceColors[7],
            Gray=RGB(128,128,128),
            White=RGB(255,255,255),
            Black=RGB(0,0,0),
            LightGray=RGB(192,192,192)
        };
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