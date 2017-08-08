#pragma once

#include "global.h"
#include "encoding.h"

/***********************************************
	TODO:
	class convexc : public glob::exception

***********************************************/


#ifdef _WIN32
	#define ConvDefaultFileEnc EncCP1251
	#define ConvDefaultConsoleEnc EncCP866
#else 
	#define ConvDefaultFileEnc EncUTF8
	#define ConvDefaultConsoleEnc EncUTF8
#endif // !_WIN32



namespace fconv {
	
	class convexc : public exception {
	public:
		convexc(errcodes code = Success, const char* msg = NULL, const char* member = NULL);
		~convexc();
		virtual std::basic_ostream<char>& operator>>(std::basic_ostream<char> &stream) const;
		virtual FILE* operator>>(FILE* const fout) const;
		virtual char const * const what();
	};
	std::basic_ostream<char>& operator<<(std::basic_ostream<char> &stream, convexc &e);
	FILE* operator<<(FILE* stream, convexc &e);

	struct finfo {
		size_t sz;
		glob::BaseDynamicBuffer<char> fname;
		finfo();
		virtual ~finfo();
		virtual void clear();
	};

	struct encstat {
		size_t ansibytes = 0;
		size_t utf8bytes = 0;
		size_t utf8badbytes = 0;
		size_t pairdiff1 = 0;
		size_t pairdiff2 = 0;
		size_t nullbytes = 0;
		encstat();
		virtual ~encstat();
		void clear();
	};
	
	class converter {
		glob::BaseDynamicBuffer<char> ansibuff;
		glob::BaseDynamicBuffer<byte> inputbuff;
		finfo inputfinfo;
		encstat charstat;
		encoding** encs;
		enctype encin;
		errcodes lerrno;
		
		converter& operator=(const converter &src);
		converter(const converter &src);
		void _throw(const char* msg, const char* member) const;
	public:
		converter();
		~converter();
		enctype inputenc() const;
		char* release();
		errcodes lasterror() const;
		errcodes load(const char* fname);
		errcodes takeover(const char* _string, const size_t size);
		errcodes savebufferas(const char* filename, enctype encoding = ConvDefaultFileEnc);
		errcodes convertfile(const char* filename, enctype encoding = ConvDefaultFileEnc);
		void* convert(void* buffer, enctype in, enctype out);
		char* load2string(const char* fname);
	private:
		errcodes init();
		enctype chkFileBOM(FILE* const pFile = NULL);
		bool analyzeEnc();
		encoding* newEncoding(enctype const type);
		errcodes open(const char* fname);
		bool analyzeCP866();
		bool chkUtf8bytes();
		bool chkUtf8byte(const byte* src, size_t &offset);
		bool chkUcs2bytes();
		size_t chkBOM(const char* src);
		void clear();
		// enc tests
		float diffbytestest(const size_t n1, const size_t n2);
		float simcounttest(const size_t n1, const size_t n2);
		float simbytestest(const size_t* bytes, const size_t* simmcount);
		float simover6test(const size_t* simmcount, const size_t* over6);
		
	};
};