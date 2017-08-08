#pragma once

#include "ftwd/enctypes.h"
#include "tpl/glob.h"

namespace ftwd {
	namespace chlib {
		enum eEncChar {
			EncCharLPos = 0,
			EncCharETS = 1,
			EncCharData = 2,
			EncCharMaxEncSize = 4,
			EncCharTSize = 6
		};

		struct EncChar {
			byte lPos;
			byte _ETS;
			dword _char;
			EncChar();
			EncChar(byte const * const _Array, size_t &pos, byte const _Libpos);
			byte* dataref() const;
			EncChar& clear();
			EncChar& parse(byte const * const _Array, size_t &pos, byte const _Libpos);
			enctypes type() const;
			byte size() const;
			char* const toString(char * const _Dest, size_t &_DestPos) const;
			EncChar& operator=(EncChar const &_Other);
			FILE* operator>>(FILE* const Cpp) const;
			std::basic_ostream<char>& operator>>(std::basic_ostream<char> &stream) const;
			bool operator==(EncChar const &_Other) const;
			bool operator!=(EncChar const &_Other) const;
			operator unsigned short() const;
			operator unsigned long() const;
		};
	}
}

