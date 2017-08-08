#pragma once
#ifdef _WIN32
	#ifdef EXCEPTIONDLL_EXPORT
		#define EXCEPTIONDLL_API __declspec(dllexport)
	#elif _WIN32
		#define EXCEPTIONDLL_API __declspec(dllimport)
	#endif // !EXCEPTIONDLL_EXPORT
#else
	#define EXCEPTIONDLL_API

#endif // _WIN32

#include <iostream>
#include <cstdio>
#include <ctime>
#include "ftwd/types.h"
#include "ftwd/errcodes.h"

namespace ftwd {
    /* Exception default parameters */
    enum ExceptionDefaults {
        ExcDefltMsgSize = 256,
        ExcDefltClassNameSize = 50,
        ExcDefltMemberNameSize = 50,
        ExcDeltThrTimeSize = 20
    };

    class EXCEPTIONDLL_API exception {
    protected:
        errcodes _code;
        char msg[ExcDefltMsgSize];
        char thrmember[ExcDefltClassNameSize];
        char thrclass[ExcDefltMemberNameSize];
        char thrtime[ExcDeltThrTimeSize];
		char* __utctime(char* const _Buffer, const time_t t, const char _Separator);
		void __copy(char* const _TargetString, const char* const _SourceString, size_t const _SizeLimit);
    public:
		exception(errcodes const code = Success, const char* const msg = nullptr, const char* const thrfunc = nullptr, const char* const thrclass = nullptr);
        virtual ~exception();
        virtual std::basic_ostream<char>& operator>>(std::basic_ostream<char> &stream) const;
        virtual FILE* operator>>(FILE* const fout) const;
        virtual char const * const what() const;
		errcodes const code() const;
    };

	EXCEPTIONDLL_API std::basic_ostream<char>& operator<<(std::basic_ostream<char> &stream, exception &e);
	EXCEPTIONDLL_API FILE* operator<<(FILE* stream, exception &e);

};


