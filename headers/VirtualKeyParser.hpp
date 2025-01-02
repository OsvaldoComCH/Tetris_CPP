#ifndef VIRTUALKEYPARSER
#define VIRTUALKEYPARSER

#include <Windows.h>
#include <string>

int GetVKFromString(const std::string * Str)
{
    using namespace std;
    if(Str->rfind("NUMPAD", 0) == 0)
    {
        string Sub = Str->substr(6);
        if(Sub.compare("0") == 0)
        {
            return VK_NUMPAD0;
        }else
        if(Sub.compare("1") == 0)
        {
            return VK_NUMPAD1;
        }else
        if(Sub.compare("2") == 0)
        {
            return VK_NUMPAD2;
        }else
        if(Sub.compare("3") == 0)
        {
            return VK_NUMPAD3;
        }else
        if(Sub.compare("4") == 0)
        {
            return VK_NUMPAD4;
        }else
        if(Sub.compare("5") == 0)
        {
            return VK_NUMPAD5;
        }else
        if(Sub.compare("6") == 0)
        {
            return VK_NUMPAD6;
        }else
        if(Sub.compare("7") == 0)
        {
            return VK_NUMPAD7;
        }else
        if(Sub.compare("8") == 0)
        {
            return VK_NUMPAD8;
        }else
        if(Sub.compare("9") == 0)
        {
            return VK_NUMPAD9;
        }
    }else
    if(Str->compare("UP") == 0)
    {
        return VK_UP;
    }else
    if(Str->compare("DOWN") == 0)
    {
        return VK_DOWN;
    }else
    if(Str->compare("LEFT") == 0)
    {
        return VK_LEFT;
    }else
    if(Str->compare("RIGHT") == 0)
    {
        return VK_RIGHT;
    }else
    if(Str->compare("ENTER") == 0)
    {
        return VK_RETURN;
    }else
    if(Str->compare("SPACE") == 0)
    {
        return VK_SPACE;
    }else
    if(Str->compare("TAB") == 0)
    {
        return VK_TAB;
    }else
    if(Str->compare("BACK") == 0)
    {
        return VK_BACK;
    }else
    if(Str->compare("SHIFT") == 0)
    {
        return VK_SHIFT;
    }else
    {
        short Ret = VkKeyScanEx(Str->c_str()[0], GetKeyboardLayout(0));
        return *((unsigned char *) &Ret);
    }

    return -1;
}

int GetVKName(int Vk, std::string * Str)
{
    if(Vk == VK_RETURN)
    {
        Str->assign("ENTER");
    }else
    if(Vk == VK_TAB)
    {
        Str->assign("TAB");
    }else
    if(Vk == VK_BACK)
    {
        Str->assign("BACK");
    }else
    if(Vk == VK_SHIFT)
    {
        Str->assign("SHIFT");
    }else
    if(Vk == VK_LEFT)
    {
        Str->assign("LEFT");
    }else
    if(Vk == VK_UP)
    {
        Str->assign("UP");
    }else
    if(Vk == VK_RIGHT)
    {
        Str->assign("RIGHT");
    }else
    if(Vk == VK_DOWN)
    {
        Str->assign("DOWN");
    }else
    if(Vk == VK_SPACE)
    {
        Str->assign("SPACE");
    }else
    if(Vk >= VK_NUMPAD0 && Vk <= VK_NUMPAD9)
    {
        char Text[8];
        sprintf(Text, "NUMPAD%i", Vk - VK_NUMPAD0);
        Str->assign(Text);
    }else
    {
        int Key = MapVirtualKeyEx(Vk, MAPVK_VK_TO_CHAR, GetKeyboardLayout(0));
        if(Key)
        {
            Str->assign((char *) &Key);
        }else
        {
            return 1;
        }
    }
    return 0;
}

#endif