#include <Memory.h>#include <Resources.h>#include "font routines.h"#include "graphics toolbox library.h"#define segmentLoaderTrashesResLoadState#ifdef segmentLoaderTrashesResLoadState    #define SETRESLOAD(load)    SetResLoad(load)#else    #define SETRESLOAD(load)#endiflong AddResFileFonts(short resFileID, gxFont** fonts){    long i, count;    long validFonts = 0;    char resLoadState = *(char*)0xa5e;    short oldResFile = CurResFile();    UseResFile(resFileID);    count = Count1Resources('sfnt');    if (fonts)        SetHandleSize((Handle)fonts, count * sizeof(gxFont));    SetResLoad(false);    for (i = 1; i <= count; i++)    {   Handle sfnt;        SETRESLOAD(false);        sfnt = Get1IndResource('sfnt', i);        if (sfnt && !ResError())        {   gxFont fontID = GXNewFont(gxResourceFontStorage, sfnt, 0);            if (fonts)                (*fonts)[validFonts] = fontID;            validFonts++;        }    }    SetResLoad(resLoadState);    if (fonts && validFonts != count)        SetHandleSize((Handle)fonts, validFonts * sizeof(gxFont));    UseResFile(oldResFile);    return validFonts;}