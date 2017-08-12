#pragma once
/*
Global templates
*/

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include "ftwd/types.h"
#include "ftwd/exception.h"

/* template class used as 'nullptr' */
class _null {
public:
	template<typename T>
	operator T*() {
		return 0;
	}
};
_null null;

namespace ftwd {
	
	/* namespace used for not secure functions */
	namespace global_ns { };
    
	/* strlen
		returns count of elements before first 0 Element */
	namespace global_ns {
		template<typename T>
		size_t strlen(const T* const _String) {
			size_t pos = 0;
			while (_String[pos])
				++pos;
			return pos;
		}
	}
    template<typename T>
    size_t strlen(const T * const _String) {
        size_t pos = 0;
        if (_String) {
			while (_String[pos])
				++pos;
        }
		else {
			throw(exception(globErrNullPtr, NULL, "strlen"));
		}
		return pos;
    }
	namespace global_ns {
		template<typename T>
		T* dcopy(const T* const _Src, const size_t _Len) {
			T* buff = new T[_Len + 1];
			for (size_t i = 0; i < _Len; i++)
				buff[i] = _Src[i];
			buff[_Len] = 0;
			return buff;
		}
	};
	template<typename T>
	T* dcopy(const T* const _Src, const size_t _Len = 0) {
		T* buff = nullptr;
		if (_Src) {
			size_t len = (_Len ? _Len : global_ns::strlen(_Src));
			T* buff = new T[_Len + 1];
			for (size_t i = 0; i < _Len; i++)
				buff[i] = _Src[i];
			buff[_Len] = 0;
		}
		return buff;
	}
	/* zalloc
		returns pointer to byte array filled with 0 */
	namespace global_ns {
		template<typename T>
		T* zalloc(size_t _ElemCnt) {
			size_t tSz = sizeof(T) * _ElemCnt;
			T* _array = new T[_ElemCnt];
			byte* temp = (byte*)_array;
			while (tSz)
				temp[--tSz] = 0;
			return _array;
		}
	}
    template<typename T>	
    T* zalloc(size_t _ElementsCount) {
        if (_ElementsCount) {
            size_t fullsize = sizeof(T) * _ElementsCount;
            byte* t = new T[fullsize];
            for (size_t i = 0; i < fullsize; i++)
                t[i] = 0;
            return t;
        }
        else {
            throw(exception(globErrNullBytesAlloc));
        }
        return nullptr;
    }

	/* val2buff
		writes given value to given buffer */
	namespace global_ns {
		template<typename T>
		bool val2buff(void* const _Dest, size_t &_DestOff, T const _Source, size_t BytesCnt = sizeof(T)) {
			byte* buff = (byte*)_Dest;
			byte* Src = (byte*)&_Source;
			for (size_t i = 0; i < BytesCnt; i++) {
				buff[_DestOff++] = Src[i];
			}
			return true;
		}
	}
    template<typename T>	// writes value to given buffer, return boolean rtue or false
    bool val2buff(void* const _Dest, size_t &_DestOff, T const _Source) {
        if (_Dest) {
            byte* buff = (byte*)_Dest;
            size_t SrcSize = sizeof(_Source);
            byte* Src = (byte*)&_Source;
            for (size_t i = 0; i < SrcSize; i++) {
                buff[_DestOff++] = Src[i];
            }
        }
        else {
            throw(exception(globErrNullPtr, "Agr0", "write2buff<T>"));
        }
        return true;
    }
	
	/*	fload
	*	loads full file into given buffer
	*	'_Dest' will be allocated using 'new' there will be the loaded file
	*	returns size of loaded file in count of given template size (filesize / sizeof(T))
	*	throws ftwd::exception	*/
    template<typename T>	
    size_t fload(T* &_Dest, const char * const _Path) {
        size_t SrcSize = 0;
        if (_Path) {
            size_t ElemSize = sizeof(T);
            FILE* _SrcFile = fopen(_Path, "rb");
            if (_SrcFile) {
                // getting size of source file
                fseek(_SrcFile, 0, SEEK_END);
                SrcSize = ftell(_SrcFile);
                fseek(_SrcFile, 0, SEEK_SET);

                _Dest = (T*)new byte[SrcSize + ElemSize];
                if (!fread(_Dest, SrcSize, 1, _SrcFile)) {
                    throw(exception(globErrReadingFile, _Path, "fload"));
                }
                fclose(_SrcFile);
                byte* temp = (byte*)(&_Dest[SrcSize]);
                for (size_t i = 0; i < ElemSize; i++)
                    temp[i] = 0;
                SrcSize /= sizeof(T);
            }
            else {
                throw(exception(globErrOpenFile, _Path, "fload"));
            }
        }
        else {
            throw(exception(globErrNullPtr, "Arg1", "fload"));
        }
        return SrcSize;
    }
    template<typename T>	// writes value to a string in hex format
    const char* int2hex(char* _Dest, T const &_Int, bool const _Header = false, size_t _BytesToConvert = 0) {
        char size = sizeof(T);
        if (size < 9) {
            byte* src = (byte*)&_Int;
            byte* tgt = (byte*)_Dest;
            unsigned long long temp = (unsigned long long)_Int;
            size_t tgtPos = 0;
            if (_Header) {
                tgt[tgtPos++] = '0', tgt[tgtPos++] = 'x';
            }
            char i = 0;
            while (++i, temp /= 0x100, temp > 0);
            if (_BytesToConvert && _BytesToConvert <= sizeof(T)) {
                i = sizeof(T);
            }
            for (; i > 0;) {
                --i;
                byte b1 = src[i] / 0x10, b2 = src[i] % 0x10;
                b1 += (b1 > 9 ? ('a' - 10) : '0');
                b2 += (b2 > 9 ? ('a' - 10) : '0');
                tgt[tgtPos++] = b1, tgt[tgtPos++] = b2;
            }
            tgt[tgtPos] = '\0';
        }
        else {
            byte i = 0;
            if (_Header) {
                _Dest[i++] = '0';
                _Dest[i++] = 'x';
            }
            _Dest[i++] = '0';
            _Dest[i++] = '0';
            _Dest[i] = '\0';
        }
        return _Dest;
    }

    template<typename T>
    static T* strcopy(const T* string, const size_t len = 0) {
        T* copy = nullptr;
        if (string) {
            size_t sz = (len ? len : strlen(string));
            copy = new T[sz + 1];
            for (size_t i = 0; i < sz; i++)
                copy[i] = string[i];
            copy[sz] = 0;
        }
        return copy;
    }
    template<typename T>
    static T* strcopy(T* const copy, const T* src, const size_t len = 0) {
        if (src && copy) {
            if(len) {
                for (size_t i = 0; i < len; i++) {
                    copy[i] = src[i];
                }
                copy[len] = 0;
            }
            else {
                size_t pos = 0;
                while(copy[pos] = src[pos], src[pos++]);
            }
        }
        else if(!src){
            copy[0] = 0;
        }
        else {
            throw(exception(globErrNullPtr, "T* const, T const * const, size_t const", "strcopy<T>"));
        }
        return copy;
    }
    template<typename T>
    static errcodes fread(T* _TargetBuffer, const size_t _ElementsCount, FILE* const _FilePointer) {
        if (_FilePointer) {
            size_t esize = sizeof(T);
            if (fread(_TargetBuffer, esize * _ElementsCount, 1, _FilePointer)) {
                return Success;
            }
            throw(exception(globErrReadingFile, NULL, "_fread", "glob"));
        }
        throw(exception(globErrNullFilePtr, NULL, "_fread", "glob"));
    }
    namespace global_ns {
        template<typename T>
        errcodes memcopy(T* _Dest, const T* const _Source, const size_t _ElCnt) {
            for(size_t i = 0; i < _ElCnt; i++) {
                _Dest[i] = _Source[i];
            }
            return Success;
        }
    }
    template<typename T1, typename T2>
    static errcodes memcopy(T1* _TargetBuff, T2* const _SourceBuff, size_t const _SourceSize) {
        if (_TargetBuff) {
            if (_SourceBuff) {
                if (_SourceSize) {
                    size_t totalBytes = _SourceSize * sizeof(T2);
                    byte* src = (byte*)_SourceBuff;
                    byte* tgt = (byte*)_TargetBuff;
                    for (size_t i = 0; i < totalBytes; i++) {
                        tgt[i] = src[i];
                    }
                    return Success;
                }
                throw(exception(globErrNullBytesAlloc, "Arg2", "memcopy", "glob"));
            }
            throw(exception(globErrNullPtr, "Arg1", "memcopy", "glob"));
        }
        throw(exception(globErrNullPtr, "Arg0", "memcopy", "glob"));
    }

    /* type compare */
    // dummy container
    template<typename T>
    struct __typetest {
        __typetest() {}
        virtual ~__typetest() {}
    };
    // one argument compare: typecom<type1>(type2 const &Arg);
	template<typename T>
	bool typecmp(const size_t _TypeId) {
		__typetest<T> t, *ptr = &t;
		return (*((size_t*)ptr) == _TypeId);
	}
    template<typename T1, typename T2>
    bool typecmp(T2 const &_Type2) {
        __typetest<T1> _t1, *ptr1 = &_t1;
        __typetest<T2> _t2, *ptr2 = &_t2;
        return *((size_t*)ptr1) == *((size_t*)ptr2);
    }
    // two arguments compare: typecmp(type1 const &Arg1, type2 const &Arg2);
    template<typename T1, typename T2>
    bool typecmp(T1 const &_Type1, T2 const &_Type2) {
        __typetest<T1> _t1, *ptr1 = &_t1;
        __typetest<T2> _t2, *ptr2 = &_t2;
        return *((size_t*)ptr1) == *((size_t*)ptr2);
    }
    template<typename T>
    bool isInt(T &_Var) {
        bool _Result = false;
        if (typecmp<char>(_Var) || typecmp<byte>(_Var)
            || typecmp<short>(_Var) || typecmp<word>(_Var)
            || typecmp<long>(_Var) || typecmp<dword>(_Var)
            || typecmp<long long>(_Var) || typecmp<qword>(_Var)
            )
            _Result = true;
        return _Result;
    }
    template<typename T>
    bool isNat(T &_Var){
        return (typecmp<double>(_Var) || typecmp<float>(_Var));
    }
    /* End of typecmp */
	
	/* type to string */
	namespace global_ns {
		template<typename T>
		const char* const int2str(char* const _Dest, const T _Value, size_t _Prec = 0) {
			size_t digits = 0;
			T tVal = _Value;
			do {
				++digits;
			} while (tVal /= 10);
			if (digits < _Prec) digits = _Prec;
			char* dest = _Dest;
			tVal = _Value;
			if (tVal < 0) {
				tVal = -tVal;
				*(dest++) = '-';
			}
			dest[digits] = 0;
			while (digits) {
				dest[--digits] = (tVal % 10) + '0';
				tVal /= 10;
			}
			return _Dest;
		}
		const char* const dbl2str(char* const _Dest, const double _Value, size_t _Prec = 0) {
			char mask[] = { '%', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			byte mskPos = 1;
			if (_Prec) {
				mask[mskPos++] = '.';
				mask[mskPos++] = ((_Prec % 100) / 10) + '0';
				mask[mskPos++] = (_Prec % 10) + '0';
			}
			mask[mskPos++] = 'f';
			mask[mskPos] = 0;
			sprintf(_Dest, mask, _Value);
			return _Dest;
		}
	}
    template<typename T>
    const char * const type2str(char* _Dest, T const &_Value, size_t _Prec = 0) {
        if (_Dest) {
            global_ns::int2str(_Dest, _Value, _Prec);
        }
        else {
            throw(exception(globErrInvalidValueType, nullptr, "etype2str<T>"));
        }
        return _Dest;
    }
	template<>
	const char * const type2str<double>(char* _Dest, const double &_Value, size_t _Prec) {
		if (_Dest) {
			global_ns::dbl2str(_Dest, _Value, _Prec);
		}
		else {
			throw(exception(globErrInvalidValueType, nullptr, "etype2str<T>"));
		}
		return _Dest;
	}
	template<>
	const char * const type2str<float>(char* _Dest, const float &_Value, size_t _Prec) {
		if (_Dest) {
			global_ns::dbl2str(_Dest, _Value, _Prec);
		}
		else {
			throw(exception(globErrInvalidValueType, nullptr, "etype2str<T>"));
		}
		return _Dest;
	}
	
	/* cstrings deep compare */
	namespace global_ns {
		template<typename T>
		bool strcomp(T const * const _Str1, T const * const _Str2, size_t const _Length) {
			for (size_t i = 0; i < _Length; i++)
				if (_Str1[i] == _Str2[i]);
				else return false;
			return true;
		}
	};
	template<typename T>
    bool strcomp(T const * const _Str1, T const * const _Str2, size_t const _Length = 0) {
        if(_Str1 != _Str2) {
			if (_Str1 && _Str2) {
				size_t l1, l2;
				if (!_Length) {
					l1 = global_ns::strlen(_Str1);
					l2 = global_ns::strlen(_Str2);
				}
				else {
					l1 = l2 = _Length;
				}
				return global_ns::strcomp(_Str1, _Str2, (l1 < l2 ? l1 : l2));
			}
			return false;
        }
        return true;
    }
	
	/* counts chars in given string */
	namespace global_ns {
		template<typename T>
		size_t strcnt(const T* const _Src, const size_t _SrcOff, const T _Char) {
			size_t result = 0;
			size_t srcPos = _SrcOff;
			while (_Src[srcPos]) {
				if(_Src[srcPos++] != _Char) { }
				else { ++result; }
			}
			return result;
		}
		template<typename T>
		size_t strcnt(const T* const _Src, const T* const _Sub, const size_t _SubLen, const size_t _SrcOff, const size_t _Limit) {
			size_t count = 0;
			size_t srcPos = _SrcOff;
			while (srcPos < _Limit) {
				if (_Src[srcPos] != _Sub[0]) { ++srcPos; }
				else {
					size_t subPos = 1;
					const T* temp = _Src + srcPos;
					for (; subPos < _SubLen; subPos++) {
						if (_Sub[subPos] != temp[subPos]) { break; }
					}
					if (subPos != _SubLen) { ++srcPos; }
					else { ++count; srcPos += subPos; }
				}
			}
			return count;
		}
	}
	template<typename T>
	size_t strcnt(const T* const _Src, const T _C, size_t _SrcOff = 0) {
		size_t count = 0;
		if (_Src) {
			size_t i = _SrcOff;
			while (_Src[i])
				if (_Src[i++] != _C);
				else
					++count;
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "strcnt<T>", __FILE__));
		}
		return count;
	}
	template<typename T>
	size_t strcnt(const T* const _Src, const T* const _Sub, size_t _SrcOff = 0) {
		size_t count = 0;
		if (_Src && _Sub) {
			size_t srcLen = strlen(_Src);
			size_t subLen = strlen(_Sub);
			if (subLen < srcLen) {
				return global_ns::strcnt(_Src, _Sub, subLen, _SrcOff, (srcLen - subLen));
			}
		}
		else {
			throw(exception(globErrNullPtr, "Arg0", "strcnt<T>", __FILE__));
		}
		return count;
	}

    /* strpos */
    namespace global_ns {
        template<typename T>
        long long strpos(const T* const _Source, const size_t _SrcSize, const T _Char, const size_t _SrcOff) {
            for (size_t srcPos = 0; srcPos < _SrcSize; srcPos++)
                if (_Source[srcPos] != _Char);
                else return srcPos;
            return -1;
        }
        template<typename T>
        long long strpos(const T* const _Source, const size_t _SrcSize, const size_t _SrcOff, const T* const _Tpl, const size_t _TplSize) {
            size_t limit = _SrcSize - _TplSize;
            for (size_t srcPos = 0; srcPos < limit; srcPos++)
                if (_Source[srcPos] != _Tpl[0]);
                else {
                    size_t tplPos = 1;
                    const T* const temp = _Source + srcPos;
                    for (; tplPos < _TplSize; tplPos++)
                        if (temp[tplPos] != _Tpl[tplPos]) { break; }
                    if (tplPos != _TplSize);
                    else return srcPos;
                }
            return -1;
        }
    }
    template<typename T>
    long long strpos(const T* const _Src, const T _Value, const size_t _SrcOff = 0, size_t _SrcLen = 0) {
        if (_Src) {
            size_t len = (_SrcLen ? _SrcLen : strlen(_Src));
            for (size_t pos = _SrcOff; pos < len; pos++) {
                if (_Src[pos] != _Value) { }
                else { return pos; }
            }
        }
        else {
            throw(exception(globErrNullPtr, "_Src", "strpos", __FILE__));
        }
        return 0;
    }
	template<typename T>
	long long strpos(const T* const _Src, const T* const _Tpl, const size_t _SrcOff = 0) {
		if (_Src && _Tpl) {
			size_t srcLen = strlen(_Src);
			size_t tplLen = strlen(_Tpl);
			size_t limit = srcLen - tplLen;
			for (size_t srcPos = _SrcOff; srcPos < srcLen; srcPos++) {
				if (_Src[srcPos] != _Tpl[0]);
				else {
					size_t tplPos = 1;
					const T* temp = _Src + srcPos;
					for (; tplPos < tplLen; tplPos++) {
						if (_Tpl[tplPos] != temp[tplPos]) break;
					}
					if (tplPos != tplLen);
					else {
						return srcPos;
					}
				}
			}
		}
		else {
			throw(exception(globErrNullPtr, "_Src", "strpos", __FILE__));
		}
		return -1;
	}

}