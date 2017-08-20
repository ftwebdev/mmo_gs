#pragma once
#include "ftwd/global.h"
#include "ftwd/enctypes.h"

#ifdef _WIN32
	#ifdef CHARLIBDLL_EXPORT
		#define CHARLIBDLL_API __declspec(dllexport)
	#elif _WIN32
		#define CHARLIBDLL_API __declspec(dllexport)
	#endif // !CHARLIBDLL_EXPORT
#else
	#define CHARLIBDLL_API
#endif // !_WIN32

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
			CHARLIBDLL_API EncChar();
			CHARLIBDLL_API EncChar(byte const * const _Array, size_t &pos, byte const _Libpos);
			CHARLIBDLL_API byte* dataref() const;
			CHARLIBDLL_API EncChar& clear();
			CHARLIBDLL_API EncChar& parse(byte const * const _Array, size_t &pos, byte const _Libpos);
			CHARLIBDLL_API enctypes type() const;
			CHARLIBDLL_API byte size() const;
			CHARLIBDLL_API char* const toString(char * const _Dest, size_t &_DestPos) const;
			CHARLIBDLL_API EncChar& operator=(EncChar const &_Other);
			CHARLIBDLL_API FILE* operator>>(FILE* const Cpp) const;
			CHARLIBDLL_API std::basic_ostream<char>& operator>>(std::basic_ostream<char> &stream) const;
			CHARLIBDLL_API bool operator==(EncChar const &_Other) const;
			CHARLIBDLL_API bool operator!=(EncChar const &_Other) const;
			CHARLIBDLL_API operator unsigned short() const;
			CHARLIBDLL_API operator unsigned long() const;
		};
	}
}

