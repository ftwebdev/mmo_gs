#define GLOBALDLL_EXPORT

#include "ftwd/global.h"
#include "tpl/global.h"

namespace ftwd {
	/************************ Global enums, flags, types and constants ************************/
	/* a definition of class dlGlobal */
	
	// '//'
	const byte dlGlobal::DSlash = 0b0001;
	// '#'
	const byte dlGlobal::Sharp = 0b0010;
	// '/* */'
	const byte dlGlobal::CStyle = 0b0100;
	
	// bracket types
	static const char* BracketTypes[] = { "{}", "[]", "()", "<>" };
	
	// {}
	const byte dlGlobal::CrlBrace = 0b0001;
	// []
	const byte dlGlobal::SqrBrace = 0b0010;
	// ()
	const byte dlGlobal::RndBrace = 0b0100;
	// <>
	const byte dlGlobal::CrnBrace = 0b1000;
	
	/* end of class dlGlobal */
	
	/************************ Global functions and classes realisation ************************/

	void _exit(const int code) {
	#ifdef _WIN32
		system("pause");
	#endif // _WIN32
		exit(code);
	}
	void _exit(const exception &e) {
		e >> std::cerr;
	#ifdef _WIN32
		system("pause");
	#endif // _WIN32
		exit(e.code());
	}
	// not safe funcions
	namespace global_ns {
		// check: _Dest != nullptr, _Source != nullptr, (_DestOffset + _BytesCount) < strlen(_Dest)
		bool write2buff(void* const _Dest, size_t &_DestOff, void const * const _Source, size_t const _BytesCount) {
			byte* Dest = (byte*)_Dest;
			byte* Src = (byte*)_Source;
			for (size_t i = 0; i < _BytesCount; i++) {
				Dest[_DestOff++] = Src[i];
			}
			return true;
		}
		// strlen(buffer) > 19
		errcodes utctime(char* const buff, const time_t t, const char sep) {
			tm* now = gmtime(&t);
			buff[2] = buff[5] = sep;
			buff[10] = ' ';
			buff[13] = buff[16] = ':';
			buff[19] = 0;
			now->tm_mon++;
			now->tm_year += 1900;
			for (char i = 1; i >= 0; i--) {
				buff[i] = now->tm_mday % 10 + '0';
				now->tm_mday /= 10;
			}
			for (char i = 4; i > 2; i--) {
				buff[i] = now->tm_mon % 10 + '0';
				now->tm_mon /= 10;
			}
			for (char i = 9; i > 5; i--) {
				buff[i] = now->tm_year % 10 + '0';
				now->tm_year /= 10;
			}
			buff[11] = now->tm_hour / 10 + '0';
			buff[12] = now->tm_hour % 10 + '0';

			buff[14] = now->tm_min / 10 + '0';
			buff[15] = now->tm_min % 10 + '0';

			buff[17] = now->tm_sec / 10 + '0';
			buff[18] = now->tm_sec % 10 + '0';
			return Success;
		}
		// pFile != nullptr
		size_t fsize(FILE* pFile) {
			size_t size = 0;
			size_t pos = ftell(pFile);
			fseek(pFile, (long)0, SEEK_END);
			size = ftell(pFile);
			fseek(pFile, (long)pos, SEEK_SET);
			return size;
		}
		// pFile != nullptr
		size_t fleft(FILE* pFile) {
			size_t size = 0;
			size_t pos = ftell(pFile);
			fseek(pFile, (long)0, SEEK_END);
			size = ftell(pFile) - pos;
			fseek(pFile, (long)pos, SEEK_SET);
			return size;
		}
		// _Src != nullptr
		long long strpos(const char* const _Src, const char _Tpl, const size_t _Offset, const size_t _SrcLen) {
			for (size_t i = _Offset; i < _SrcLen; i++) {
				if (_Src[i] == _Tpl)
					return i;
			}
			return -1;
		}
		// _Src != _Sub != nullptr
		long long strpos(const char* const _Src, const char* const _Sub, const size_t _Offset, const size_t _SrcLen, const size_t _SubLen) {
			for (size_t srcPos = 0; srcPos < _SrcLen; srcPos++) {
				if(_Src[srcPos] != _Sub[0]) { }
				else {
					size_t subPos = 1;
					const char* const temp = &_Src[srcPos];
					for (; subPos < _SubLen; subPos++) {
						if (_Sub[subPos] != temp[subPos]) { break; }
						else { }
					}
					if(subPos != _SubLen) { }
					else { return srcPos; }
				}
			}
		}
		// _Src != nullptr
		long long strpos_ue(const char* const _Src, const size_t _SrcLen, const char const _Tpl, const size_t _SrcOff, const char _Esc = '\\') {
			if (_Src[_SrcOff] != _Tpl) {
				char prev = _Src[_SrcOff];
				for (size_t srcPos = _SrcOff + 1; srcPos < _SrcLen; srcPos++) {
					if (_Src[srcPos] == _Tpl && prev != '\\') {
						return srcPos;
					}
					prev = _Src[srcPos];
				}
			}
			else {
				return _SrcOff;
			}
			return -1;
		}
		// _Src != nullptr, _Tpl != nullptr
		long long firstof(const char* const _Src, const char* const _Tpl, const size_t _Offset, const size_t _SrcLen) {
			size_t sLen = (_SrcLen ? _SrcLen : strlen(_Src));
			size_t tLen = strlen(_Tpl);
			for (size_t i = _Offset; i < sLen; i++) {
				for (size_t j = 0; j < tLen; j++) {
					if (_Src[i] == _Tpl[j])
						return i;
				}
			}
			return -1;
		}
		// _Src != _Tpl != nullptr
		long long firstof_ue(const char* const _Src, const size_t _SrcLen, const char* const _Tpl, const size_t _SrcOff, const char _Esc = '\\') {
			size_t tplSize = strlen(_Tpl);
			for (size_t tplPos = 0; tplPos < tplSize; tplPos++) {
				if (_Src[_SrcOff] != _Tpl[tplPos]) { }
				else { return 0; }
			}
			for (size_t srcPos = _SrcOff + 1; srcPos < _SrcLen; srcPos++) {
				for (size_t tplPos = 0; tplPos < tplSize; tplPos++) {
					if(_Tpl[tplPos] != _Src[srcPos]) { }
					else if (_Src[srcPos] != '\\') { return srcPos; }
				}
			}
			return -1;
		}
		// _Src != nullptr
		errcodes cutChar(char* const _Src, size_t &_SrcLen, const char _Tpl, const size_t _Offset, const size_t _Limit) {
			char* tgt = _Src;
			size_t tgtPos, srcPos;
			tgtPos = srcPos = _Offset;
			for ( ; srcPos < _Limit; srcPos++) {
				if (_Src[srcPos] != _Tpl) {
					(tgtPos < srcPos ? (tgt[tgtPos++] = _Src[srcPos]) : (++tgtPos));
				}
			}
			if (srcPos < _SrcLen) {
				for (; srcPos < _SrcLen; srcPos++) {
					(tgtPos < srcPos ? (tgt[tgtPos++] = _Src[srcPos]) : (++tgtPos));
				}
			}
			tgt[tgtPos] = 0;
			_SrcLen = tgtPos;
			return Success;
		}
		// _Src != nullptr, _qTypes != nullptr
		errcodes cutNQChar(const char* const _Src, const char _Tpl, const char* const _qTypes) {
			size_t srcLen = strlen(_Src);
			size_t qtsLen = strlen(_qTypes);
			char* const Target = (char* const)_Src;
			size_t tgtPos = 0;
			char qtType = 0;
			size_t qtPos = srcLen;
			// continue
			for (size_t srcPos = 0; srcPos < srcLen; srcPos++) {
				/*qPos = _SrcLen;*/
				long long next = 0;
				/* getting pos of next not escaped quote */
				qtPos = srcLen - 1;
				// if not in quoteblock
				if (!qtType) {
					if ((next = firstof_ue(_Src, srcLen, _qTypes, srcPos)) != (-1)) {
						qtPos = next;
						qtType = _Src[qtPos];
					}
					else {
						qtType = _qTypes[0];
					}
				}
				else {
					next = strpos_ue(_Src, srcLen, qtType, srcPos);
					if (next != -1) {
						qtPos = next;
					}
					qtType = 0;
				}
				/* now we've found the next quote */

				// if we are in quote
				if (!qtType) {
					// just copy the source to buffer including the quote
					while (srcPos < qtPos) {
						Target[tgtPos++] = _Src[srcPos++];
					}
					Target[tgtPos++] = _Src[srcPos];
				}
				// not in quote
				else {
					// copy to buffer with skipping not needed chars
					while (srcPos < qtPos) {
						if (_Src[srcPos] != _Tpl) {
							Target[tgtPos++] = _Src[srcPos];
						}
						++srcPos;
					}
					// copy the quote
					Target[tgtPos++] = _Src[srcPos];
				}
			}
			Target[tgtPos] = '\0';
			return Success;
		}
		// _buffer != nullptr, _BuffSize <= strlen(_buffer)
		errcodes delCommDSlash(byte* _buffer, size_t &_BuffSize) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (incom && i && _buffer[i - 1] == '\n') incom = false;
				if (i && _buffer[i] == '/' && _buffer[i - 1] != '\\' && _buffer[i + 1] == '/')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '/' && _buffer[i] == '/') {
					incom = true;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize && _buffer[i] != '\n') i++;
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
			return Success;
		}
		// _buffer != nullptr, _BuffSize <= strlen(_buffer)
		errcodes delCommSharp(byte* _buffer, size_t &_BuffSize) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (incom && i && _buffer[i - 1] == '\n') incom = false;
				if (i && _buffer[i] == '#' && _buffer[i - 1] != '\\')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '#') {
					incom = true;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize && _buffer[i] != '\n') i++;
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
			return Success;
		}
		// _buffer != nullptr, _BuffSize <= strlen(_buffer)
		errcodes delCommCStyle(byte* _buffer, size_t &_BuffSize) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (i && _buffer[i] == '/' && _buffer[i - 1] != '\\' && _buffer[i + 1] == '*')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '/' && _buffer[i] == '/') {
					incom = true;
					i += 2;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize) {
						if (_buffer[i] == '*' && _buffer[i + 1] == '/' && _buffer[i - 1] != '\\') {
							++i;
							incom = false;
							break;
						}
						++i;
					}
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
			return Success;
		}
	}
	// safe section
	char tolower(char const _C) {
		byte c = (byte)_C;
		if ((c > 0x40 && c < 0x5b) || (c > 0xbf && c < 0xe0))
			return (char)(c + 0x20);
		else if (c == 0xa8)
			return (char)0xb8;
		return _C;
	}
	char* tolower(char* const _Src) {
		if (_Src) {
			byte* src = (byte*)_Src;
			size_t srcLen = strlen(_Src);
			for (size_t srcPos = 0; srcPos < srcLen; srcPos++) {
				if ((src[srcPos] > 0x40 && src[srcPos] < 0x5b) || (src[srcPos] > 0xbf && src[srcPos] < 0xe0))
					src[srcPos] += 0x20;
				else if (src[srcPos] == 0xa8)
					src[srcPos] = 0xb8;
			}
		}
		return _Src;
	}
	char toupper(char const _C) {
		byte c = (byte)_C;
		if ((c > 0x60 && c < 0x7b) || c > 0xdf)
			return (char)(c - 0x20);
		else if (c == 0xb8)
			return (char)0xa8;
		return _C;
	}
	char* toupper(char* const _Src) {
		if (_Src) {
			byte* src = (byte*)_Src;
			size_t srcLen = strlen(_Src);
			for (size_t srcPos = 0; srcPos < srcLen; srcPos++) {
				if ((src[srcPos] > 0x60 && src[srcPos] < 0x7b) || (src[srcPos] > 0xdf))
					src[srcPos] -= 0x20;
				else if (src[srcPos] == 0xb8)
					src[srcPos] = 0xa8;
			}
		}
		return _Src;
	}
	char num2digit(const byte _c) {
		return (_c % 10 + '0');
	}
	
	bool write2buff(void* const _Dest, size_t &_DestOff, void const * const _Source, size_t const _BytesCount) {
		bool _Result = false;
		if (_Dest && _Source) {
			return global_ns::write2buff(_Dest, _DestOff, _Source, _BytesCount);
		}
		else {
			throw(exception(globErrNullPtr, (_Dest ? "Arg2" : "Arg0"), "write2buff"));
		}
		return _Result;
	}
	errcodes utctime(char* const buff , const time_t t, const char sep) {
		if (buff) {
			return global_ns::utctime(buff, t, sep);
		}
		return (globErrNullPtr);
	}

	size_t fsize(FILE* pFile) {
		size_t size = 0;
		if (pFile) {
			size_t pos = ftell(pFile);
			fseek(pFile, (long)0, SEEK_END);
			size = ftell(pFile);
			fseek(pFile, (long)pos, SEEK_SET);
		}
		else {
			throw(exception(globErrNullFilePtr, nullptr, "fsize"));
		}
		return size;
	}
	size_t fleft(FILE* pFile) {
		size_t size = 0;
		if (pFile) {
			size_t pos = ftell(pFile);
			fseek(pFile, (long)0, SEEK_END);
			size = ftell(pFile) - pos;
			fseek(pFile, (long)pos, SEEK_SET);
		}
		else {
			throw(exception(globErrNullFilePtr, nullptr, "fleft"));
		}
		return size;
	}

	
	long long strpos(const char* const _Src, const char _Tpl, const size_t _Offset, const size_t _SrcLen) {
		if (_Src) {
			size_t len = (_SrcLen ? _SrcLen : strlen(_Src));
			if (_Offset < len) {
				for (size_t i = _Offset; i < len; i++) {
					if (_Src[i] == _Tpl)
						return i;
				}
			}
			else {
				throw(exception(globErrArrayOutOfRange, "_Offset >= _SrcLen", "strpos", __FILE__));
			}
		}
		else {
			throw(exception(globErrNullPtr, "_Src", "strpos", __FILE__));
		}
		return -1;
	}
	long long firstof(const char* const _Src, const char* const _Tpl, const size_t _Offset, const size_t _SrcLen) {
		if (_Src && _Tpl) {
			size_t sLen = (_SrcLen ? _SrcLen : strlen(_Src));
			if (_Offset < sLen) {
				size_t tLen = strlen(_Tpl);
				for (size_t i = _Offset; i < sLen; i++) {
					for (size_t j = 0; j < tLen; j++) {
						if (_Src[i] == _Tpl[j])
							return i;
					}
				}
			}
			else {
				throw(exception(globErrArrayOutOfRange, "_Offset >= _SrcLen", "strpos", __FILE__));
			}
		}
		else {
			throw(exception(globErrNullPtr, (_Tpl ? "_Src" : "_Tpl"), "strpos", __FILE__));
		}
		return -1;
	}
	// find position of closest to the '_SrcOff' next not escaped with '_Esc' character
	long long strpos_ue(const char* const _Src, const size_t _SrcLen, const char const _Tpl, const size_t _SrcOff, const char _Esc) {
		if (_Src) {
			if (_Src[_SrcOff] != _Tpl) {
				char prev = _Src[_SrcOff];
				for (size_t srcPos = _SrcOff + 1; srcPos < _SrcLen; srcPos++) {
					if (_Src[srcPos] == _Tpl && prev != '\\') {
						return srcPos;
					}
					prev = _Src[srcPos];
				}
			}
			else {
				return _SrcOff;
			}
		}
		else {
			throw(exception(globErrNullPtr, (_Src ? "Arg1" : "Arg0"), "findFirstOf", __FILE__));
		}
		return -1;
	}
	
	/* class refcounter */
	refcounter::refcounter()
		: reffs(1)
	{}
	refcounter::~refcounter() {
		reffs = 0;
	}
	size_t refcounter::inc() {
		return ++reffs;
	}
	size_t refcounter::dec() {
		if (reffs > 0) {
			return --reffs;
		}
		else {
			throw(exception(globErrBadReffCount, nullptr, "dec", "refcounter"));
		}
	}
	
	errcodes cutChar(char* const _Src, size_t &_SrcLen, const char _Tpl, const size_t _Offset, const size_t _Limit) {
		if (_Src) {
			return global_ns::cutChar(_Src, _SrcLen, _Tpl, _Offset, _Limit);
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "cutChar", __FILE__));
		}
		return Success;
	}
	errcodes cutNQChar(char* const _Src, const char _Tpl, const char* const _qTypes) {
		if (_Src && _qTypes) {
			return global_ns::cutNQChar(_Src, _Tpl, _qTypes);
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "cutChar", __FILE__));
		}
		return Success;
	}
	errcodes cutNQChar(char* const _Src, const char* _Tpl, const char* const _qTypes) {
		if (_Src && _Tpl && _qTypes) {
			size_t chsSize = strlen(_Tpl);
			for (size_t i = 0; i < chsSize; i++) {
				global_ns::cutNQChar(_Src, _Tpl[i], _qTypes);
			}
		}
		else {
			throw(exception(globErrNullPtr, (_Src ? "Arg1" : "Arg0"), "cutChar", __FILE__));
		}
		return Success;
	}
	errcodes cutChar(char* const _Src, size_t &_SrcLen, const char* const _Tpl, const size_t _Offset, const size_t _Limit) {
		if (_Src && _Tpl) {
			size_t tplSize = strlen(_Tpl);
			for (size_t i = 0; i < tplSize; i++) {
				cutChar(_Src, _SrcLen, _Tpl[i], _Offset, _Limit);
			}
		}
		else {
			throw(exception(globErrNullPtr, (_Tpl ? "Arg0" : "Arg2"), "cutChar", __FILE__));
		}
		return Success;
	}
	errcodes cutSpaces(char* const _Src, size_t &_SrcLen, const char* _Spaces) {
		if (_Src && _Spaces) {
			size_t spLen = strlen(_Spaces);
			for(size_t i = 0; i < spLen; i++)
			global_ns::cutChar(_Src, _SrcLen, _Spaces[i], 0, _SrcLen);
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "cutChar", __FILE__));
		}
		return Success;
	}
	errcodes cutNQSpaces(char* const _Src, size_t &_SrcLen, const char* _Quotes, const char* _Spaces) {
		if (_Src && _SrcLen && _Quotes && _Spaces) {
			size_t spLen = strlen(_Spaces);
			for (size_t i = 0; i < spLen; i++) {
				global_ns::cutNQChar(_Src, _Spaces[i], _Quotes);
			}
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "cutNQSpaces", __FILE__));
		}
		return Success;
	}

	errcodes strreverse(char* _Src) {
		if (_Src) {
			size_t srcLen = strlen(_Src);
			if (srcLen > 0) {
				size_t endOffset = srcLen - 1;
				char temp;
				for (size_t i = 0; i < srcLen; i++) {
					temp = _Src[i];
					_Src[srcLen] = _Src[endOffset - i];
					_Src[endOffset - i] = temp;
				}
			}
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "strreverse", __FILE__));
		}
		return Success;
	}
	
	errcodes delCommDSlash(byte* _buffer, size_t &_BuffSize) {
		if (_buffer) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (incom && i && _buffer[i - 1] == '\n') incom = false;
				if (i && _buffer[i] == '/' && _buffer[i - 1] != '\\' && _buffer[i + 1] == '/')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '/' && _buffer[i] == '/') {
					incom = true;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize && _buffer[i] != '\n') i++;
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
		}
		else {
			throw(exception(globErrNullPtr, "_buffer", "delCommDslash", "global"));
		}
		return Success;
	}
	errcodes delCommSharp(byte* _buffer, size_t &_BuffSize) {
		if (_buffer) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (incom && i && _buffer[i - 1] == '\n') incom = false;
				if (i && _buffer[i] == '#' && _buffer[i - 1] != '\\')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '#') {
					incom = true;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize && _buffer[i] != '\n') i++;
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
		}
		else {
			throw(exception(globErrNullPtr, "_buffer", "delCommSharp", "global"));
		}
		return Success;
	}
	errcodes delCommCStyle(byte* _buffer, size_t &_BuffSize) {
		if (_buffer) {
			bool incom = false;
			byte* tgt = _buffer;
			size_t tgtPos = 0;
			for (size_t i = 0; i < _BuffSize; i++) {
				if (i && _buffer[i] == '/' && _buffer[i - 1] != '\\' && _buffer[i + 1] == '*')
					incom = true;
				else if (i);
				else if (!i && _buffer[i] == '/' && _buffer[i] == '/') {
					incom = true;
					i += 2;
				}
				if (!incom) {
					if (tgtPos) {
						tgt[tgtPos] = _buffer[i];
					}
					++tgtPos;
				}
				else {
					while (i < _BuffSize) {
						if (_buffer[i] == '*' && _buffer[i + 1] == '/' && _buffer[i - 1] != '\\') {
							++i;
							incom = false;
							break;
						}
						++i;
					}
				}
			}
			tgt[tgtPos] = '\0';
			_BuffSize = tgtPos;
		}
		else {
			throw(exception(globErrNullPtr, "_buffer", "delCommDslash", "global"));
		}
		return Success;
	}
	errcodes delComments(byte* _buffer, size_t &_BuffSize, const byte cflags) {
		// CStyle multistring /* */
		if (cflags & dlGlobal::CStyle) {
			delCommCStyle(_buffer, _BuffSize);
			return Success;
		}
		// double slash '//'
		if (cflags & dlGlobal::DSlash)
			delCommDSlash(_buffer, _BuffSize);
		// sharp '#'
		if (cflags & dlGlobal::Sharp)
			delCommSharp(_buffer, _BuffSize);
		return Success;
	}

}; /* end of namespace ftwd */
