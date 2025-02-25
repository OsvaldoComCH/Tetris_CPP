#ifndef TETRISCONSTANTS
#define TETRISCONSTANTS

#include "Config.hpp"
#include <Windows.h>
#include <chrono>

typedef signed char int8;
typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_milli;

namespace Tetris
{
    typedef struct Point
    {
        int8 x, y;

        int8& operator[] (int index)
        {
            if(index){return y;}
            return x;
        }
        int8& operator[] (int8 index)
        {
            if(index){return y;}
            return x;
        }
    } Point;

    Config CFG;
    Config TempCFG;
    HWND hwnd;

    static constexpr Point SpinTable[4] = {{0,3},{2,3},{0,1},{2,1}};

    namespace Render
    {
        HFONT DefFont;
        HFONT BigFont;
        HFONT StatsFont;
        HFONT TimerFont;
        HPEN BkgdPen;
        HPEN PiecePen;

        static constexpr COLORREF PieceColors[8] = {
            RGB(64,64,64),
            RGB(16,192,192),
            RGB(192,16,192),
            RGB(192,192,16),
            RGB(192,96,16),
            RGB(32,32,192),
            RGB(192,32,32),
            RGB(32,192,32),
        };
        static constexpr COLORREF ShadowColors[7] = {
            RGB(64,96,96),
            RGB(96,64,96),
            RGB(96,96,64),
            RGB(128,96,64),
            RGB(72,72,112),
            RGB(112,72,72),
            RGB(72,112,72)
        };
        
        static HPEN ShadowPens[7];

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

        void InitResources()
        {
            DefFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

            BigFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

            TimerFont = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
            
            StatsFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);

            PiecePen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, Color::Black);
            BkgdPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, Color::Gray);

            for(int i = 0; i < 7; ++i)
            {
                ShadowPens[i] = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, PieceColors[i + 1]);
            }
        }

        void FreeResources()
        {
            DeleteObject(DefFont);
            DeleteObject(BigFont);
            DeleteObject(TimerFont);
            DeleteObject(StatsFont);
            DeleteObject(PiecePen);
            DeleteObject(BkgdPen);
            for(int i = 0; i < 7; ++i)
            {
                DeleteObject(ShadowPens[i]);
            }
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