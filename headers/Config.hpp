#ifndef TETRISCONFIG
#define TETRISCONFIG

#include "VirtualKeyCodes.h"
#include "VirtualKeyParser.hpp"
#include <Windows.h>
#include <fstream>
#include <string>

typedef struct
{
    int Left;
    int Right;
    int HardDrop;
    int SoftDrop;
    int RCW;
    int RCCW;
    int Hold;
} TetrisControls;

typedef struct TetrisConfig
{
    unsigned char WindowSize;

    TetrisControls Controls;
} TetrisConfig;

static constexpr TetrisConfig DEFAULT_CONFIG = {4, {VK_LEFT, VK_RIGHT, VK_SPACE, VK_DOWN, VK_UP, VK_X, VK_C}};

void WriteConfigFile(const TetrisConfig * cfg)
{
    using namespace std;
    fstream File("./settings.cfg", fstream::out | fstream::trunc);
    string Key;
    if(!File.is_open()){return;}

    File << "[Window Settings]\n";
    File << "WindowSize=" << (int)cfg->WindowSize << "\n\n";
    File << "[Controls]\n";

    if(GetVKName(cfg->Controls.Left, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.Left, &Key);
    }
    File << "Left=" << Key << "\n";

    if(GetVKName(cfg->Controls.Right, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.Right, &Key);
    }
    File << "Right=" << Key << "\n";

    if(GetVKName(cfg->Controls.HardDrop, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.HardDrop, &Key);
    }
    File << "HardDrop=" << Key << "\n";

    if(GetVKName(cfg->Controls.SoftDrop, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.SoftDrop, &Key);
    }
    File << "SoftDrop=" << Key << "\n";

    if(GetVKName(cfg->Controls.RCW, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.RCW, &Key);
    }
    File << "RotateCW=" << Key << "\n";

    if(GetVKName(cfg->Controls.RCCW, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.RCCW, &Key);
    }
    File << "RotateCCW=" << Key << "\n";

    if(GetVKName(cfg->Controls.Hold, &Key))
    {
        GetVKName(DEFAULT_CONFIG.Controls.Hold, &Key);
    }
    File << "Hold=" << Key << "\n";
}

void ReadConfigFile(TetrisConfig * cfg)
{
    using namespace std;
    *cfg = DEFAULT_CONFIG;

    fstream File("./settings.cfg", fstream::in);
    if(File.is_open())
    {
        while(!File.eof())
        {
            string Line;
            getline(File, Line);
            if(Line.length() == 0 || Line.rfind("[", 0) == 0){continue;}

            int Divider = Line.find("=");
            if(Divider == -1){continue;}

            string Key = Line.substr(0, Divider);
            string Value = Line.substr(Divider + 1);
            if(Key.compare("WindowSize") == 0)
            {
                cfg->WindowSize = stoi(Value);
            }else
            {
                int Vk = GetVKFromString(&Value);
                if(Vk == -1){continue;}

                if(Key.compare("Left") == 0)
                {
                    cfg->Controls.Left = Vk;
                }else
                if(Key.compare("Right") == 0)
                {
                    cfg->Controls.Right = Vk;
                }else
                if(Key.compare("HardDrop") == 0)
                {
                    cfg->Controls.HardDrop = Vk;
                }else
                if(Key.compare("SoftDrop") == 0)
                {
                    cfg->Controls.SoftDrop = Vk;
                }else
                if(Key.compare("RotateCW") == 0)
                {
                    cfg->Controls.RCW = Vk;
                }else
                if(Key.compare("RotateCCW") == 0)
                {
                    cfg->Controls.RCCW = Vk;
                }else
                if(Key.compare("Hold") == 0)
                {
                    cfg->Controls.Hold = Vk;
                }
            }
        }
    }else
    {
        WriteConfigFile(cfg);
    }
}

#endif