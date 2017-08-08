#pragma once

#include "ftwd/EncChar.h"

namespace ftwd {
    enum eCharLib {
        ChLibUnknown,
        ChLibUTF8,
        ChLibUCS2BE,
        ChLibUCS2LE,
        ChLibCP1251,
        ChLibCP866,
        ChLibTypes,
        ChLibCharsPerType = 0x100
    };

    namespace chlib {
        
        
        extern const EncChar
            *const *const CharLib,
            *const LibUnknown,
            *const LibUTF8,
            *const LibUCS2BE,
            *const LibUCS2LE,
            *const LibCP1251,
            *const LibCP866
        ;
        // much costable by time and resources function that search characters in lib
        static EncChar convert(const eCharLib _TypeOut, const EncChar &_SrcChar);
        // get position of char and convert to other type
        static EncChar convertByType(const enctypes _TypeOut, const enctypes _TypeIn, const dword iChar);
        // get char from type and position
        static EncChar convertByPos(const enctypes _TypeOut, const byte _LPos);
    }
}

