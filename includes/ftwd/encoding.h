#pragma once

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
	#ifdef CHARLIBDLL_EXPORT
		#define CHARLIBDLL_API __declspec(dllexport)
	#elif _WIN32
		#define CHARLIBDLL_API __declspec(dllexport)
	#endif // !CHARLIBDLL_EXPORT
#else
	#define CHARLIBDLL_API
#endif // !_WIN32

#include "ftwd/enctypes.h"
#include "ftwd/CharLib.h"
#include "tpl/BaseDynamicBuffer.h"
#include "tpl/glob.h"
//#include "global.h"

/**
* @version: 1.1
* @author: felexxx67@gmail.com
*/

namespace ftwd {
	namespace enc {

	#ifdef _WIN32
		enctypes SysIOSCharset = EncCP866;
	#else
		enctypes SysIOSCharset = EncUTF8;
	#endif
	}

	class encoding {
	public:
		errcodes CHARLIBDLL_API convert(enctypes _Current, enctypes _Target, byte* &_SourceString, size_t &TotalBytes, bool addMarker = false) const;
	};
	
	extern const qword CHARLIBDLL_API EncMarkers[];
	extern const byte CHARLIBDLL_API EncMarkersSize[];
	namespace enc {
		class _encoding {
			friend encoding;
			enctypes _type;
			_encoding(const encoding &Src);
		protected:
			virtual chlib::EncChar ansiToEncChar(const char _Src) const final;
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
			virtual errcodes addBOM(FILE* pFile) const final;
		public:
			virtual _encoding& operator=(const _encoding &_Src) final;
			virtual _encoding& operator=(const enctypes _EncType) final;
			explicit _encoding(const enctypes _type = EncUnknown);
			virtual ~_encoding();
		};

		class UTF8 : public _encoding {
			friend encoding;
			UTF8(UTF8 const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			UTF8();
			~UTF8();
		};

		class UCS2BE : public _encoding {
			friend encoding;
			UCS2BE(UCS2BE const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			UCS2BE();
			~UCS2BE();
		};

		class UCS2LE : public _encoding {
			friend encoding;
			UCS2LE(UCS2LE const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			UCS2LE();
			~UCS2LE();
		};
		class CP1251 : public _encoding {
			friend encoding;
		public:
			CP1251();
			~CP1251();
		};
		class CP866 : public _encoding {
			friend encoding;
			CP866(const CP866 &src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
		public:
			CP866();
			~CP866();
		};
	};
	std::basic_ostream<char>& CHARLIBDLL_API operator<<(std::basic_ostream<char>&, const char* const);
};
