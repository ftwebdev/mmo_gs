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

namespace ftwd {
	enum errcodes {
		/* Global */
		Success,
		globErrNullPtr,
		globErrNullFilePtr,
		globErrBadReffCount,
		globErrOpenFile,
		globErrReadingFile,
		globErrWrittingFile,
		globErrBadAlloc,
		globErrNullBytesAlloc,
		globInvFileName,
		globErrArrayOutOfRange,
		globErrInvalidValueType,
		globErrBadArgs,

		/* encoding */
		EncErrUTF8NoLibfile,
		EncErrUTF8BadLibfile,
		EncErrInvalidUTF8Char,
		EncErrUCS2NoLibfile,
		EncErrUCS2BadLibfile,
		EncErrInvalidUCS2Char,

		/* converter */
		ConvErrEncodingUndefined,
		ConvDefinedBomUTF8,
		ConvDefinedBomUCS2BE,
		ConvDefinedBomUCS2LE,
		ConvNoBOM,

		UnknownError,
		errcodesSizeOffset
	};

	extern EXCEPTIONDLL_API const char* const ErrMsgs[];
}

