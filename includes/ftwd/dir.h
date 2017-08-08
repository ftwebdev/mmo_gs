#pragma once

#include "tpl/BaseDynamicBuffer.h"

namespace ftwd {
	enum eDirectory {
		DirMaxFileNameSize = 256,
		DirMaxTempFileNameSize = 256
	};
	struct ffblk {
		long long ff_reserved;
		size_t ff_fsize;
		unsigned long ff_attrib;
		unsigned short ff_ftime;
		unsigned short ff_fdate;
		char ff_name[DirMaxFileNameSize];
	};
	class Directory {
		BaseDynamicBuffer<char> _fullPath;
		BaseDynamicBuffer<char> _path;
		char* _dname;
	public:
		Directory& chdir(const char* _Path);
		Directory& close();
		Directory& open(const char* const _Path);
		Directory& rewind();
		errcodes findfirst(const char* const _fName, ffblk &_Buffer, int _Atrib);
		errcodes findnext(ffblk &_Buffer);
		static errcodes getcwd(char* &_Buffer, size_t &_BuffLength);
		static errcodes mkdir(const char* const _DirName);
		static errcodes mktemp(char _Buffer[DirMaxTempFileNameSize]);
		static errcodes rmdir(const char* const _DirName);
	};
}