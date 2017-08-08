#include "encoding.h"

/*
cout function for current platform
parameters:
(const char* cp1251, std::basic_ostream<char>& stream = std::cout)
*/
std::basic_ostream<char>& cout(const char* cp1251, std::basic_ostream<char> &stream) {
#ifdef _WIN32
	stream << ansi2cp866(cp1251);
#else
	UTF8 utf;
	char* t = (char*)utf.encode(cp1251, glob::strlen(cp1251));
	stream << t;
	delete[] t;
#endif
	return stream;
}

/*
This two variables can be modified for your needs, that's why i've placed it in global space,
all you have to remember - the order must be the same
*/
// EncLibraries paths
char* EncLibraries[] = {
	"bin/utf8.bin",
	"bin/ucs2.bin",
	"bin/_utf8.bin",
	"bin/_ucs2be.bin",
	"bin/_ucs2le.bin"
};
byte** LibUTF8::bin = NULL;
byte* LibUTF8::buffer = NULL;
size_t LibUTF8::reffs = 0;

LibUTF8::LibUTF8(const char* libpath) {
	init(libpath);
}
LibUTF8::LibUTF8(const LibUTF8 &src) {
	++reffs;
}
LibUTF8::~LibUTF8() {
	if (reffs != 0 && (--reffs) == 0) {
		if (bin) {
			delete[] bin;
		}
		bin = 0;
		if (buffer) {
			delete[] buffer;
		}
		buffer = 0;
		reffs = 0;
	}
}
bool LibUTF8::write2buffer(byte* _Dest, size_t &_BuffOffset, const char _CharCP1251) const {
	if (_Dest) {
		if (!_CharCP1251 & 0x80) {
			_Dest[_BuffOffset++] = _CharCP1251;
		}
		else {
			byte LibPos = (byte)_CharCP1251 - (byte)0x80;
			byte tgtSize = bin[LibPos][0];
			for (size_t i = 1; i <= tgtSize; i++) {
				_Dest[_BuffOffset++] = bin[LibPos][i];
			}
		}
		return true;
	}
	throw(exception(globErrNullPtr, "Arg0", "write2buffer", "LibUTF8"));
}
bool LibUTF8::read2cp1251(char &_Dest, byte const * const _Utf8, size_t &_Utf8Offset) const {
	if (_Utf8) {
		if (_Utf8[_Utf8Offset] < 0x80) {
			_Dest = (char)_Utf8[_Utf8Offset++];
			return true;
		}
		else {
			for (size_t i = 0; i < EncDefaultLibsize; i++) {
				if (_Utf8[_Utf8Offset] == bin[i][1]) {
					size_t j = 1;
					while ((_Utf8[_Utf8Offset + j]) == (byte)bin[i][1 + j]) {
						++j;
					}
					if (j == bin[i][0]) {
						_Utf8Offset += bin[i][0];
						_Dest = (char)(i + 0x80);
						return true;
					}
				}
			}
		}
		_Dest = '?';
		return false;
	}
	throw(exception(globErrNullPtr, "Arg1", "read2cp1251", "LibUTF8"));
}
byte LibUTF8::countUtf8ChSize(byte const _Utf8FirstByte) const {
	byte mask = 0x40;
	byte _ChSize = 1;
	while (mask & _Utf8FirstByte) {
		++_ChSize;
		mask >>= 1;
	}
	return _ChSize;
}
char const * const LibUTF8::operator[] (char const &_CharCP1251) const {
	char _result[9];
	if ((byte)_CharCP1251 < 0x80) {
		_result[0] = _CharCP1251;
		_result[1] = '\0';
	}
	else {
		byte LibPos = (byte)_CharCP1251 - 0x80;
		for (size_t i = 0; i <= bin[LibPos][0]; i++) {
			_result[i] = (char)bin[LibPos][i + 1];
		}
		_result[bin[LibPos][0]] = 0;
	}
	return _result;
}
char const * const LibUTF8::operator[] (byte const * _CharUtf8) const {
	byte _ChCp1251[2] = { (byte)'?', (byte)'\0' };
	if (_CharUtf8[0] & 0x80) {
		byte _CharSize = countUtf8ChSize(_CharUtf8[0]);
		for (size_t i = 0; i < EncDefaultLibsize; i++) {
			if (bin[i][0] == _CharSize) {
				size_t j;
				for (j = 0; j < bin[i][0]; j++) {
					if (bin[i][j + 1] == _CharUtf8[j]) {

					}
					else {
						break;
					}
				}
				if (bin[i][0] == j) {
					_ChCp1251[0] = i + 0x80;
					break;
				}
			}
		}
	}
	else {
		_ChCp1251[0] = (byte)_CharUtf8;
	}
	return (char*)_ChCp1251;
}
size_t LibUTF8::strlen_cp1251(byte const * const _utf8String) const {
	size_t cp1251StringSize = 0;
	if (_utf8String) {
		size_t totalUtf8Length = glob::strlen(_utf8String);
		size_t i = 0;
		byte mask = 0x80;
		while (i <  totalUtf8Length) {
			++cp1251StringSize;
			i += countUtf8ChSize(_utf8String[i]);
		}
	}
	return cp1251StringSize;
}
size_t LibUTF8::strlen_utf8(char const * const _cp1251String) const {
	size_t ResultLength = 0;
	if (_cp1251String) {
		size_t SrcLength = strlen(_cp1251String);
		for (size_t i = 0; i < SrcLength; i++) {
			if ((byte)_cp1251String[i] < 0x80) {
				++ResultLength;
			}
			else {
				ResultLength += bin[(byte)_cp1251String[i] - 0x80][0];
			}
		}
	}
	return ResultLength;
}
size_t LibUTF8::parseLib(byte const * const _Buffer, size_t const _Size) {
	size_t i = 0;
	size_t libpos = 0;
	for (i = 0; i < _Size; i++) {
		if (testFirstByte(_Buffer[i + 1], _Buffer[i]) && testOtherBytes(&_Buffer[i])) {
			bin[libpos++] = (byte*)(_Buffer + i);
			i += _Buffer[i];
		}
		else {
			throw(exception(EncErrUTF8BadLibfile, NULL, "parseLib", "LibUTF8"));
		}
	}
	return i;
}
bool LibUTF8::testFirstByte(byte const &_byte, byte const &_CharSize) const {
	byte test1 = 0, test2 = 0;
	switch (_CharSize) {
	case 2:
		test1 = 0b11011111;
		test2 = 0b00100000;
		break;
	case 3:
		test1 = 0b11101111;
		test2 = 0b00010000;
		break;
	case 4:
		test1 = 0b11110111;
		test2 = 0b00001000;
		break;
	case 5:
		test1 = 0b11111011;
		test2 = 0b00000100;
		break;
	case 6:
		test1 = 0b11111101;
		test2 = 0b00000010;
		break;
	case 7:
		test1 = 0b11111110;
		test2 = 0b00000001;
		break;
	case 8:
		test1 = 0b11111111;
		test2 = 0b00000000;
		break;
	default:
		return false;
	}
	if(_CharSize < 8 && (test1 ^ _byte) < test2) {
		return true;
	}
	else if (_CharSize == 8 && !(test1 ^ _byte)) {
		return true;
	}
	return false;
}
bool LibUTF8::testOtherBytes(byte const* _Chars) const {
	byte count = _Chars[0];
	for(size_t i = 2; i <= count; i++) {
		if (((byte)0x80 ^ _Chars[i]) >= (byte)0x40) {
			return false;
		}
	}
	return true;
}
void* LibUTF8::encode(char const * const _Cp1251, size_t _SrcSize) const {
	byte* _EncodedString = NULL;
	if (_SrcSize && _Cp1251) {
		size_t EncStringLen = strlen_utf8(_Cp1251);
		byte* EncString = new byte[EncStringLen + 1];
		size_t EncPos = 0;
		for (size_t i = 0; i < _SrcSize; i++) {
			write2buffer(EncString, EncPos, _Cp1251[i]);
		}
		EncString[EncPos] = 0;
	}
	return _EncodedString;
}
char* LibUTF8::decode(void * const _Utf8, size_t _SrcSize) const {
	char* _DecResult = NULL;
	if (_SrcSize && _Utf8) {
		_DecResult = new char[_SrcSize];
		byte* _EncString = (byte*)_Utf8;
		size_t decPos = 0;
		for (size_t i = 0; i < _SrcSize;) {
			if ((_EncString[i] ^ 0b11000000) < 0x40) {
				double _dEncChar = enc::Utf8Lib._UtfChar2double(&_EncString[i]);
				char* _EncChar = (char*)&_dEncChar;
				for (byte j = 0; (j < 8 && _EncChar[j]); j++) {
					++i;
				}
				_DecResult[decPos++] = enc::Utf8Lib[_dEncChar];
			}
			else if (!(_EncString[i] & 0x80)) {
				_DecResult[decPos++] = _EncString[i++];
			}
		}
	}
	return _DecResult;
}
errcodes LibUTF8::init(const char* const libpath) {
	if (!bin && !buffer && !reffs) {
		if (libpath) {
			try {
				bin = new byte*[EncDefaultLibsize];
				for (size_t i = 0; i < EncDefaultLibsize; i++) {
					bin[i] = NULL;
				}
				size_t LibFileSize = fload(buffer, libpath);
				if (LibFileSize != parseLib(buffer, LibFileSize)) {
					throw(exception(EncErrUTF8BadLibfile, "Wrong Libsize", "LibUTF8", "LibUTF8"));
				}
			}
			catch (std::exception e) {
				throw(exception(UnknownError, e.what(), "LibUTF8", "LibUTF8"));
			}
		}
	}
	else {
		++reffs;
	}
}
LibUTF8& LibUTF8::operator=(LibUTF8 const &src) {
	++reffs;
}

namespace enc {
	LibUTF8 utf8;
};

size_t EncMarkers[] = {
	// unknown
	0,
	// UTF8
	0xbfbbef,
	// UCS2BE
	0xfffe,
	// UCS2LE
	0xfeff,
	// others
	0, 0, 0, 0 };
size_t EncMarkerSize[] = {
	// unknown
	0,
	// UTF8
	3,
	// UCS2
	2, 2,
	// others
	0, 0, 0, 0 };

// simple convertion functions for CP866
char ansi2cp866(const char c_) {
	uchar c = (uchar)c_;
	uchar _c = '?';
	if (c > 0xbf && c < 0xf0) {
		_c = c - 0x40;
	}
	else if (c > 0xef) {
		_c = c - 0x10;
	}
	else if (c == 0xa8)
		_c = 0xf0;
	else if (c == 0xb8)
		_c = 0xf1;
	else {
		_c = c;
	}
	return (char)_c;
}
char cp866toansi(const char c_) {
	uchar c = (uchar)c_;
	uchar _c = 0;
	if (c > 0x7f && c < 0xb0) {
		_c = c + 0x40;
	}
	else if (c > 0xdf && c < 0xf0) {
		_c = c + 0x10;
	}
	else if (c == 0xf0)
		_c = 0xa8;
	else if (c == 0xf1)
		_c = 0xb8;
	else {
		_c = c;
	}
	return (char)_c;
}
char* ansi2cp866(const char* string_) {
	size_t len = glob::strlen(string_);
	uchar* src = (uchar*)string_;
	uchar* buffer = new uchar[len + 1];
	buffer[len] = 0;
	for (size_t i = 0; i < len; i++) {
		if (src[i] > 0xbf && src[i] < 0xf0) {
			buffer[i] = src[i] - 0x40;
		}
		else if (src[i] > 0xef) {
			buffer[i] = src[i] - 0x10;
		}
		else if (src[i] == 0xa8)
			buffer[i] = 0xf0;
		else if (src[i] == 0xb8)
			buffer[i] = 0xf1;
		else {
			buffer[i] = src[i];
		}
	}
	return (char*)buffer;
}
char* cp866toansi(const char* string_) {
	size_t len = glob::strlen(string_);
	uchar* src = (uchar*)string_;
	uchar* buffer = new uchar[len + 1];
	buffer[len] = 0;
	for (size_t i = 0; i < len; i++) {
		if (src[i] > 0x7f && src[i] < 0xb0) {
			buffer[i] = src[i] + 0x40;
		}
		else if (src[i] > 0xdf && src[i] < 0xf0) {
			buffer[i] = src[i] + 0x10;
		}
		else if (src[i] == 0xf0)
			buffer[i] = 0xa8;
		else if (src[i] == 0xf1)
			buffer[i] = 0xb8;
		else {
			buffer[i] = src[i];
		}
	}
	return (char*)buffer;
}
char* ansi2cp866(char* const string_) {
	size_t len = glob::strlen(string_);
	for (size_t i = 0; i < len; i++) {
		string_[i] = ansi2cp866(string_[i]);
	}
	return string_;
}
char* cp866toansi(char* const string_) {
	size_t len = glob::strlen<char>(string_);
	for (size_t i = 0; i < len; i++)
		string_[i] = cp866toansi(string_[i]);
	return string_;
}

// abstract class encoding
encoding::encoding(enctype _type)
	: _type(_type)
	, stat(Success)
{}
encoding::~encoding() {}
void* encoding::encode(const char* buffer, const size_t size) const {
	char* result = new char[size + 1];
	result[size] = '\0';
	for (size_t i = 0; i < size; i++) {
		result[i] = buffer[i];
	}
	return result;
}
char* encoding::decode(void* const _buffer, const size_t size) const {
	char* buffer = (char*)_buffer;
	char* result = new char[size + 1];
	result[size] = '\0';
	for (size_t i = 0; i < size; i++) {
		result[i] = buffer[i];
	}
	return result;
}
errcodes encoding::addBOM(FILE* pFile) const {
	if (pFile) {
		size_t bom = EncMarkers[_type];
		char* t = (char*)&bom;
		rewind(pFile);
		for (size_t i = 0; i < EncMarkerSize[_type]; i++) {
			fputc(t[i], pFile);
		}
		return Success;
	}
	return globErrNullFilePtr;
}
errcodes encoding::state() const {
	return stat;
}
enctype encoding::type() const {
	return _type;
}

// cp1251
CP1251::CP1251()
	: encoding(EncCP1251)
{}
CP1251::~CP1251() {}

// CP866
CP866::CP866()
	: encoding(EncCP866)
{}
CP866::~CP866() {}
void* CP866::encode(const char* buffer, const size_t size) const {
	char* result = new char[size + 1];
	result[size] = '\0';
	for (size_t i = 0; i < size; i++) {
		result[i] = ansi2cp866(buffer[i]);
	}
	return result;
}
char* CP866::decode(void* const _buffer, const size_t size) const {
	char* buffer = (char*)_buffer;
	char* result = new char[size + 1];
	result[size] = '\0';
	for (size_t i = 0; i < size; i++) {
		result[i] = cp866toansi(buffer[i]);
	}
	return result;
}

// UTF8
unsigned char* UTF8::lib = NULL;
size_t UTF8::libsize = 0;
size_t UTF8::reffs = 0;
UTF8::UTF8(const char* libpath)
	: encoding(EncUTF8)
{
	stat = init(libpath);
}
errcodes UTF8::init(const char* libpath) {
	if (!lib && !libsize && !reffs) {
		FILE* libfile = fopen(libpath, "rb");
		if (!libfile) {
			throw(exception(EncErrUTF8NoLibfile, libpath, "init", "UTF8"));
		}
		libsize = fsize(libfile);
		if (!libsize) {
			throw(exception(EncErrUTF8BadLibfile, libpath, "init", "UTF8"));
		}
		lib = new byte[libsize];
		for (size_t i = 0; i < libsize; i++)
			lib[i] = (byte)fgetc(libfile);
	}
	++reffs;
	return Success;
}
UTF8::~UTF8() {
	if (lib && !(--reffs)) {
		delete[] lib;
		lib = NULL;
		libsize = reffs = 0;
	}

}
void* UTF8::encode(const char* buffer, const size_t size) const {
	size_t nlen = countUtfLength(buffer);
	char* t = new char[nlen + 1];
	size_t respos = 0;
	for (size_t i = 0; i < size; i++) {
		if (!(buffer[i] & 0x80)) {
			t[respos++] = buffer[i];
		}
		else {
			respos += write2buffer(buffer[i], t, respos);
		}
	}
	char* result = new char[respos + 1];
	for (size_t i = 0; i < respos; i++)
		result[i] = t[i];
	result[respos] = '\0';
	delete[] t;
	return result;
}
char* UTF8::decode(void* const buffer, const size_t size) const {
	size_t resultLen = countAnsiLength((char*)buffer), srcPos = 0, resPos = 0, mask = 0x80;
	char* src = (char*)buffer;
	char* result = new char[resultLen + 1];
	for (srcPos = 0; srcPos < size; srcPos++) {
		if (src[srcPos] & mask) {
			srcPos += (decode(&result[resPos++], &src[srcPos]) - 1);
		}
		else {
			result[resPos++] = src[srcPos];
		}
	}
	result[resultLen] = '\0';
	return result;
}
size_t UTF8::decode(char* tgt, const char* buffer) const {
	size_t charSize = bytesInUtfChar(*buffer);
	for (size_t i = 0; i < libsize; i++) {
		if (*buffer == (char)lib[i]) {
			size_t n;
			for (n = 1; n < charSize; n++)
				if ((unsigned char)buffer[n] != lib[i + n])
					break;
			if (n == charSize) {
				*tgt = (char)lib[i - 2];
				return charSize;
			}
		}
	}
	*tgt = '?';
	return charSize;
}
size_t UTF8::countUtfLength(const char* _string) const {
	size_t i = 0, n = 0;
	while (_string[i] != '\0') {
		++n;
		if (_string[i] & 0x80) {
			n += 6;
		}
		i++;
	}
	return n;
}
size_t UTF8::countAnsiLength(const char* const _string) const {
	size_t len = glob::strlen(_string), n = 0, mask = 0x80;
	for (size_t i = 0; i < len; i++) {
		++n;
		if (_string[i] & mask) {
			i += (bytesInUtfChar(_string[i]) - 1);
		}
	}
	return n;
}
size_t UTF8::bytesInUtfChar(const char c) const {
	size_t bytes = 0, mask = 0x80;
	while (++bytes, mask >>= 1, mask & c);
	return bytes;
}
size_t UTF8::write2buffer(char c, char* buffer, size_t offset) const {
	size_t libpos = 0, mask = 0x80, n = 0;
	for (; libpos < libsize - 1; libpos++) {
		if (c == (char)lib[libpos] && !lib[libpos + 1]) {
			libpos += 2;
			char l = (char)lib[libpos];
			while (l & mask) {
				buffer[offset + (n++)] = (char)lib[libpos++];
				mask >>= 1;
			}
			return n;
		}
	}
	buffer[offset] = '?';
	return 1;
}

// UCS2
UCS2::ucs2libblock* UCS2::lib = NULL;
size_t UCS2::libsize = 0;
size_t UCS2::reffs = 0;
UCS2::UCS2(unsigned short ByteOrder, const char* libpath)
	: encoding((ByteOrder == EncMarkers[EncUCS2LE] ? EncUCS2LE : EncUCS2BE))
{
	stat = init(libpath);
}
UCS2::~UCS2() {
	if (lib && !(--reffs)) {
		delete[] lib;
		lib = NULL;
		reffs = libsize = 0;
	}
}
void* UCS2::encode(const char* buffer, const size_t size) const {
	wchar* result = new wchar[size + 1];
	for (size_t i = 0; i < size; i++) {
		encode(result[i], buffer[i]);
	}
	result[size] = 0;
	return result;
}
char* UCS2::decode(void* const buffer, const size_t size) const {
 	wchar* src = (wchar*)buffer;
	size_t sz = (size ? size : glob::strlen(src));
	char* tgt = new char[sz + 1];
	for (size_t i = 0; i < sz; i++) {
		decode(tgt[i], src[i]);
	}
	tgt[sz] = 0;
	return tgt;
}
errcodes UCS2::init(const char* libpath) {
	if (!lib && !libsize && !reffs) {
		FILE* libfile = fopen(libpath, "rb");
		if (!libfile) {
			throw(exception(EncErrUCS2NoLibfile, libpath, "init", "UCS2"));
		}
		libsize = fsize(libfile);
		if (!libsize) {
			throw(exception(EncErrUCS2BadLibfile, libpath, "init", "UCS2"));
		}
		byte* temp = new byte[libsize];
		glob::fread(temp, libsize, libfile);
		libsize /= 3;
		lib = (ucs2libblock*)temp;
	}
	++reffs;
	return Success;
}
errcodes UCS2::encode(wchar &unicode, const char &ansi) const {
	byte* uni = (byte*)&unicode;
	if ((byte)ansi < 0x80) {
		uni[0] = (_type == EncUCS2LE ? (byte)ansi : 0);
		uni[1] = (_type == EncUCS2LE ? 0 : (byte)ansi);
		return Success;
	}
	for (size_t i = 0; i < libsize; i++) {
		if (ansi == lib[i].ansi) {
			uni[0] = (_type == EncUCS2LE ? lib[i].b2 : lib[i].b1);
			uni[1] = (_type == EncUCS2LE ? lib[i].b1 : lib[i].b2);
			return Success;
		}
	}
	uni[0] = (_type == EncUCS2LE ? 0x3f : 0);
	uni[1] = (_type == EncUCS2LE ? 0 : 0x3f);
	return EncErrInvalidUCS2Char;
}
errcodes UCS2::decode(char &ansi, const wchar &unicode) const {
	byte* uni = (byte*)&unicode;
	byte b1 = (_type == EncUCS2LE ? uni[0] : uni[1]);
	byte b2 = (_type == EncUCS2LE ? uni[1] : uni[0]);
	if (b1 == 0 && _type == EncUCS2BE) { ansi = b2; return Success; }
	else if (b2 == 0 && _type == EncUCS2LE) { ansi = b1; return Success; }
	else if (!b1 || !b2) {
		ansi = '?';
		return EncErrInvalidUCS2Char;
	}
	for (size_t i = 0; i < libsize; i++) {
		if (b1 == lib[i].b1 && b2 == lib[i].b2) {
			ansi = lib[i].ansi;
			return Success;
		}
	}
	ansi = '?';
	return EncErrInvalidUCS2Char;
}