#include <iostream>
#include <cstdio>
#include "types.h"
#include "exception.h"
#include "global.h"
#include "EncChar.h"

#ifdef _WIN32
	#define _SBX_CFG_ "win-sbx.cfg"
#else
	#define _SBX_CFG_ "x-sbx.cfg"
#endif // _WIN32

template<typename T>
T* clrArr(T const *const _Array, size_t const _Start, size_t const _End) {
	if (_Array) {
		size_t start = sizeof(T) * _Start;
		size_t end = sizeof(T) * _End;
		byte* buff = (byte*)_Array;
		for (size_t i = start; i < end; i++)
			buff[i] = 0;
	}
	return _Array;
}
enum enctype {
	EncUnknown,
	EncUTF8,
	EncUCS2BE,
	EncUCS2LE,
	EncCP1251,
	EncCP866,
	EncLatinOnly,
	EncEndOffset
};

const char* _LibFiles[] = {
	"",
	"lib_utf8.bin",
	"lib_ucs2be.bin",
	"lib_ucs2le.bin",
	"lib_cp1251.bin",
	"lib_cp866.bin"
};

struct ios {
	static const size_t
		flDfltBlkSz = 4096,
		flDfltNmSz = 255
	;
	static const byte
		bin = 0b1,
		in = 0b10,
		out = 0b100,
		app = 0b1000
	;
};

class file {
	FILE* _file;
	char _fname[1024];
	char _mode[10];
	file(file const &src);
	file& operator=(file const &src);
public:
	file(const char* const _Path, const char * const _Mode)
		: _file(fopen(_Path, _Mode))
	{
		strcopy(_fname, _Path);
		strcopy(_mode, _Mode);
	}
	~file() {
		fflush(_file);
		fclose(_file);
	}
	size_t size() const {
		return fsize(_file);
	}
	file& operator<<(const char * const _string) {
		if (_string) {
			size_t _size = strlen((byte*)_string);
			fputs(_string, _file);
		}
		return *this;
	}
	file& operator<<(const char _C) {
		fputc(_C, _file);
		return *this;
	}
	operator FILE*() const {
		return _file;
	}
};

int genCpp() {
	byte* iBuff;
	// loading libpath
	size_t iSize = fload(iBuff, _SBX_CFG_);
	// adding 256 bytes to length for filenames
	size_t nsize = iSize + 256;
	byte* _LibDir = new byte[nsize + 1];
	// copy the lib path to buffer
	strcopy(_LibDir, iBuff);
	// deleting iBuffer for other use
	delete[] iBuff;
	// now we will write to iBuffer file data, iBuffer is a tail part of _LibDir
	iBuff = &_LibDir[iSize];
	std::cout << "Enter destinatioin file name: ";
	std::cin >> iBuff;
	file fout((char*)iBuff, "w");
	fout << "#include \"EncChar.h\"\n\nunsigned char _ChLib[6][0x100][6] = {\n";
	EncChar tC;
	for (size_t i = 0; i < EncLatinOnly; i++) {
		if(!i) {
			fout << "\n\t// Unknown\n\t{";
			for (size_t j = 0; j < 0x100; j++) {
				if (j)
					fout << ", ";
				if(!(j % 3))
					fout << "\n\t\t";
				tC.clear();
				tC._char[EncCharLPos] = j;
				tC._char[EncCharETS] = 1;
				tC._char[EncCharData] = (j < 0x80 ? (byte)j : '?');
				tC >> fout;
			}
		}
		else {
			strcopy((char*)iBuff, _LibFiles[i]);
			std::cout << "Processing: " << _LibDir << "\n";
			byte* LoadedLib;
			size_t tSize = fload(LoadedLib, (char*)_LibDir);
			size_t tPos = 0;
			fout << "\n\t// " << _LibFiles[i] << "\n\t{";
			for (size_t j = 0; j < 0x100; j++) {
				if (j)
					fout << ", ";
				if(!(j % 3))
					fout << "\n\t\t";
				if(j < 0x80) {
					tC.clear();
					tC._char[EncCharLPos] = j;
					tC._char[EncCharETS] ^= (i << 4) + 1;
					if(i == 2) {
						++tC._char[EncCharETS];
						tC._char[EncCharData + 1] = j;
					}
					else if (i == 3) {
						++tC._char[EncCharETS];
						tC._char[EncCharData] = j;
					}
					else {
						tC._char[EncCharData] = j;
					}
				}
				else {
					tC.parse(LoadedLib, tPos, j);
				}
				tC >> fout;
			}
			delete[] LoadedLib;
		}
		fout << ((i < (EncLatinOnly - 1)) ? "\n\t},\n" : "\n\t}\n");
	}
	fout << "};\n\nconst EncChar"
		<< "\n\t*const *const CharLib = (EncChar**)_ChLib,"
		<< "\n\t*const _LibUnknown = (EncChar*)_ChLib[0],"
		<< "\n\t*const _LibUTF8 = (EncChar*)_ChLib[1],"
		<< "\n\t*const _LibUCS2BE = (EncChar*)_ChLib[2],"
		<< "\n\t*const _LibUCS2LE = (EncChar*)_ChLib[3],"
		<< "\n\t*const _LibCP1251 = (EncChar*)_ChLib[4],"
		<< "\n\t*const _LibCP866 = (EncChar*)_ChLib[5]\n;";
	return 0;
}

int genBinLib() {
	byte* iBuff;
	// loading libpath
	size_t iSize = fload(iBuff, _SBX_CFG_);
	// adding 256 bytes to length for filenames
	size_t nsize = iSize + 256;
	byte* _LibDir = new byte[nsize + 1];
	// copy the lib path to buffer
	strcopy(_LibDir, iBuff);
	// deleting iBuffer for other use
	delete[] iBuff;
	// now we will write to iBuffer file data, iBuffer is a tail part of _LibDir
	iBuff = &_LibDir[iSize];
	std::cout << "Enter destinatioin file name: ";
	std::cin >> iBuff;
	file fout((char*)iBuff, "w");
	EncChar tC;
	for (size_t i = 0; i < EncLatinOnly; i++) {
		if(!i) {
			for (size_t j = 0; j < 0x100; j++) {
				fout << (byte)j << (byte)1 << (j < 0x80 ? (byte)j : '?') << '\0' << '\0' << '\0';
			}
		}
		else {
			strcopy((char*)iBuff, _LibFiles[i]);
			std::cout << "Processing: " << _LibDir << "\n";
			byte* LoadedLib;
			size_t tSize = fload(LoadedLib, (char*)_LibDir);
			size_t tPos = 0;
			for (size_t j = 0; j < 0x100; j++) {
				tC.clear();
				if(j < 0x80) {
					tC._char[EncCharLPos] = j;
					tC._char[EncCharETS] ^= (i << 4) + 1;
					if(i == 2) {
						++tC._char[EncCharETS];
						tC._char[EncCharData + 1] = j;
					}
					else if (i == 3) {
						++tC._char[EncCharETS];
						tC._char[EncCharData] = j;
					}
					else {
						tC._char[EncCharData] = j;
					}
				}
				else {
					tC.parse(LoadedLib, tPos, j);
				}
				fout << (byte)j << (byte)tC.size();
				byte* _t = &tC._char[EncCharData];
				for(byte i = 0; i < 4; i++){
					fout << _t[i];
				}
			}
			delete[] LoadedLib;
		}

	}
	return 0;
}

int printBin() {
	char buff[256];
	char _strBuff[17];
	std::cout << "Enter target file name: ";
	std::cin >> buff;
	std::cout << "\n";
	byte* fin;
	size_t insz = fload(fin, buff);
	for (size_t i = 0; i < insz; i++)
	return 0;
}


int main() {
	try{
		int choise;
		std::cout << "Enter 1 to generate cpp code and 2 for binary byte array: ";
		std::cin >> choise;
		switch(choise) {
			case(1):
				genCpp();
				break;
			case(2):
				genBinLib();
				break;
			case(3):
				printBin();
			default: exit(0);
		}
	}
	catch(exception e) {
		e >> std::cerr;
	}
	
#ifdef _WIN32
	system("pause");
#endif // _WIN32
	return 0;
}