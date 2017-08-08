#pragma once

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#ifdef GLOBALDLL_EXPORT
#define GLOBALDLL_API __declspec(dllexport)
#elif _WIN32
#define GLOBALDLL_API __declspec(dllimport)
#endif // !EXCEPTIONDLL_EXPORT
#else
#define GLOBALDLL_API

#endif // _WIN32
// Global typedef
#include "types.h"
// All enums used in current project enums
#include "enums.h"
// exceptions
#include "exception.h"
#include "tpl/global.h"
#include "tpl/BaseDynamicBuffer.h"

namespace ftwd {
	/************************ Global enums, flags, types and constants ************************/
	GLOBALDLL_API struct dlGlobal {
		static const byte DSlash;	// '//'
		static const byte Sharp;	// '#'
		static const byte CStyle;	// '/* */'
		static const byte CrlBrace;	// {}
		static const byte SqrBrace;	// []
		static const byte RndBrace;	// ()
		static const byte CrnBrace;	// <>
	};

    extern EXCEPTIONDLL_API char const * const ErrMsgs[];
	void GLOBALDLL_API _exit(const int code);
	void GLOBALDLL_API _exit(const exception &e);
    /********************************* Global Functions *********************************/
    bool GLOBALDLL_API write2buff(void* const _Dest, size_t &_DestOffset, void const * const _Source, size_t const _BytesCount);
	// Buffer supose to be 'globUTCTimeSize' length(including trailing 0)
    errcodes GLOBALDLL_API utctime(char* const buffer, const time_t _time = time(0), const char sep = '-');
    size_t GLOBALDLL_API fsize(FILE* pFile);
    size_t GLOBALDLL_API fleft(FILE* pFile);
    void GLOBALDLL_API _exit(const int);
    void GLOBALDLL_API _exit(const ftwd::exception &e);

	/* Default String Functions */
	GLOBALDLL_API char tolower(char const _C);
	GLOBALDLL_API char toupper(char const _C);
	
	/* this two functions modify the given strings */
	GLOBALDLL_API char* tolower(char* const _Src);
	GLOBALDLL_API char* toupper(char* const _Src);

	GLOBALDLL_API long long strpos(const char* const _Src, const char _Tpl, const size_t _Offset = 0, const size_t _SrcLen = 0);
	GLOBALDLL_API long long firstof(const char* const _Src, const char* const _Tpl, const size_t _Offset = 0, const size_t _SrcLen = 0);
	// find position of closest to the '_SrcOff'{0} next not escaped with '_Esc'{'\'} character
	GLOBALDLL_API long long strpos_ue(const char* const _Src, const size_t _SrcLen, const char const _Tpl, const size_t _SrcOff = 0, const char _Esc = '\\');
	// cuts given char from string
	GLOBALDLL_API errcodes cutChar(char* const _Src, size_t &_SrcLen, const char _Tpl, const size_t _Offset = 0, const size_t _Limit = 0);
	GLOBALDLL_API errcodes cutNQChar(char* const _Src, const char _Tpl, const char* const _qTypes = "\"'`");
	GLOBALDLL_API errcodes cutNQChar(char* const _Src, const char* _Tpl, const char* const _qTypes);
	GLOBALDLL_API errcodes cutChar(char* const _Src, size_t &_SrcLen, const char* const _Tpl, const size_t _Offset = 0, const size_t _Limit = 0);
	GLOBALDLL_API errcodes cutSpaces(char* const _Src, size_t &_SrcLen, const char* _Spaces = " \t\r\n\v");
	GLOBALDLL_API errcodes cutNQSpaces(char* const _Src, size_t &_SrcLen, const char* _Quotes = "'\"", const char* _Spaces = " \t\r\n\v");
	// 
	GLOBALDLL_API errcodes strreverse(const char* _Src);
	/*	do not use this three functions if yuo have a multi comment file
		it may cause some problems in places like in next comment */
		/* // */
	/*	or in other cases like next comment */
	// /* ...\n#.... */
	GLOBALDLL_API errcodes delCommDSlash(byte* _buffer, size_t &_BuffSize);
	GLOBALDLL_API errcodes delCommSharp(byte* _buffer, size_t &_BuffSize);
	GLOBALDLL_API errcodes delCommCStyle(byte* _buffer, size_t &_BuffSize);
	GLOBALDLL_API errcodes delComments(byte* _buffer, size_t &_BuffSize, const byte cflags = (dlGlobal::DSlash | dlGlobal::CStyle | dlGlobal::Sharp));

	/****************** classes ****************/
    /* reference counter */
	GLOBALDLL_API class refcounter {
		size_t reffs;	// refference count
    public:
		refcounter();	// constructor
		~refcounter();	// destructor
		size_t inc();	// returns preincremented refference count
		size_t dec();	// returns predecremented refference count or 0 // throws: ftwd::exception(globErrBadReffCount)
    };

	class string {
		BaseDynamicBuffer<char> _cstr;
	public:
		string() {
			char* str = new char[1];
			*str = 0;
			_cstr.takeover(str, 1);
		}
		explicit string(const char* const _Src, const size_t _SrcSize = 0) {
			if (_Src) {
				size_t sz = (_SrcSize ? _SrcSize : strlen(_Src)) + 1;
				char* src = new char[sz];
				for (size_t i = 0; i < sz; i++)
					src[i] = _Src[i];
				_cstr.takeover(src, sz);
			}
		}
		explicit string(size_t _Length) {
			char* str = new char[_Length + 1];
			str[_Length] = 0;
			_cstr.takeover(str, _Length + 1);
		}
		string(string &_Src) {
			_cstr = _Src._cstr;
		}
		string(const string &_Src) {
			_cstr.clone(_Src._cstr);
		}
		~string() { }
		// taken string must be allocated using 'new'
		string& takeover(char* _Src, size_t _SrcLen) {
			if (_Src && _SrcLen) {
				_cstr.takeover(_Src, _SrcLen);
			}
		}
		char* release() {
			return _cstr.release();
		}

		// modifiers
		string& _reverse() {
			strreverse(_cstr.data());
			return *this;
		}
		string reverse() const {
			string temp(*this);
			return temp._reverse();
		}
		string& __tolower() {
			ftwd::tolower(_cstr.data());
			return *this;
		}
		string tolower() const {
			string temp(*this);
			return temp.__tolower();
		}
		string& __toupper() {
			ftwd::toupper(_cstr.data());
			return *this;
		}
		string toupper() const {
			string temp(*this);
			return temp.__toupper();
		}

		const size_t length() {
			return (_cstr.isEmpty() ? 0 : _cstr.size() - 1);
		}

		operator char*() {
			return _cstr.data();
		}
	};
}
