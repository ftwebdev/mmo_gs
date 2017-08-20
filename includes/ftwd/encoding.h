#pragma once

#include <cstdlib>
#include <iostream>

#include "ftwd/enctypes.h"
#include "ftwd/CharLib.h"
#include "tpl/BaseDynamicBuffer.h"
#include "tpl/global.h"

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
		static errcodes CHARLIBDLL_API convert(enctypes _Current, enctypes _Target, byte* &_SourceString, size_t &TotalBytes, bool addMarker = false);
	};
	
	extern const qword CHARLIBDLL_API EncMarkers[];
	extern const byte CHARLIBDLL_API EncMarkersSize[];
	namespace enc {
		class _encoding {
			friend encoding;
			enctypes _type;
			_encoding(const _encoding &Src);
		protected:
			CHARLIBDLL_API virtual chlib::EncChar ansiToEncChar(const char _Src) const final;
			CHARLIBDLL_API virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			CHARLIBDLL_API virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
			CHARLIBDLL_API virtual errcodes addBOM(FILE* pFile) const final;
		public:
			CHARLIBDLL_API virtual _encoding& operator=(const _encoding &_Src) final;
			CHARLIBDLL_API virtual _encoding& operator=(const enctypes _EncType) final;
			CHARLIBDLL_API _encoding();
			CHARLIBDLL_API _encoding(const enctypes _type);
			CHARLIBDLL_API virtual ~_encoding();
		};

		class UTF8 : public _encoding {
			friend encoding;
			UTF8(UTF8 const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			CHARLIBDLL_API UTF8();
			CHARLIBDLL_API ~UTF8();
		};

		class UCS2BE : public _encoding {
			friend encoding;
			UCS2BE(UCS2BE const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			CHARLIBDLL_API UCS2BE();
			CHARLIBDLL_API ~UCS2BE();
		};

		class UCS2LE : public _encoding {
			friend encoding;
			UCS2LE(UCS2LE const &_Src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
			virtual size_t countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const;
		public:
			CHARLIBDLL_API UCS2LE();
			CHARLIBDLL_API ~UCS2LE();
		};
		class CP1251 : public _encoding {
			friend encoding;
		public:
			CHARLIBDLL_API CP1251();
			CHARLIBDLL_API ~CP1251();
		};
		class CP866 : public _encoding {
			friend encoding;
			CP866(const CP866 &src);
			virtual chlib::EncChar encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const;
		public:
			CHARLIBDLL_API CP866();
			CHARLIBDLL_API ~CP866();
		};
	};
	CHARLIBDLL_API extern const enc::_encoding *const *const Encodings;
	//std::basic_ostream<char>& CHARLIBDLL_API operator<<(std::basic_ostream<char>&, const char* const);
};
