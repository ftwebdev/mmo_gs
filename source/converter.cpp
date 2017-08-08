#include "converter.h"

/*******************************************
	TODO:
	float converter::simbytes(const size_t* bytes, const size_t* simcount);

*******************************************/

namespace fconv {

	/* convexc */
	convexc::convexc(errcodes code, const char* msg, const char* member)
		: exception(code, msg, member, "fconv::converter")
	{}
	convexc::~convexc() {}
	char const * const convexc::what() {
		return ErrMsgs[_code];
	}
	std::basic_ostream<char>& convexc::operator>>(std::basic_ostream<char> &stream) const {
		stream << thrtime << "\tException: " << ErrMsgs[_code] << "\n";
		if (msg)
			stream << "Message: " << msg << "\n";
		stream << thrclass;
		if (thrmember)
			stream << "::" << thrmember << "()\n";
		else
			stream << "\n";
		stream << "\n";
		return stream;
	}
	FILE* convexc::operator>>(FILE* const fout) const {
		if (fout) {
			fputs(thrtime, fout);
			fputs("\tException: ", fout);
			fputs(ErrMsgs[_code], fout);
			fputs("\n", fout);
			if (msg) {
				fputs(msg, fout);
				fputs("\n", fout);
			}
			fputs(thrclass, fout);
			if (thrmember) {
				fputs("::", fout);
				fputs(thrmember, fout);
				fputs("()\n", fout);
			}
			fputs("\n", fout);
		}
		return fout;
	}
	std::basic_ostream<char>& operator<<(std::basic_ostream<char> &stream, convexc &e) {
		e >> stream;
		return stream;
	}
	FILE* operator<<(FILE* fout, convexc &e) {
		e >> fout;
		return fout;
	}

	/* encstat */
	encstat::encstat()
		: ansibytes(0)
		, utf8bytes(0)
		, utf8badbytes(0)
		, pairdiff1(0)
		, pairdiff2(0)
		, nullbytes(0)
	{}
	encstat::~encstat() {
		clear();
	}
	void encstat::clear() {
		ansibytes = utf8bytes = utf8badbytes = pairdiff1 = pairdiff2 = nullbytes = 0;
	}
	
	
	finfo::finfo()
		: sz(0)
		, fname()
	{}
	finfo::~finfo() {}
	void finfo::clear() {
		sz = 0;
		fname.clear();
	}


	converter::converter()
		: ansibuff()
		, inputbuff()
		, inputfinfo()
		, charstat()
		, encs(NULL)
		, encin(EncUnknown)
		, lerrno(Success)
	{
		lerrno = init();
	}
	void converter::clear() {
		if (encs) {
			delete[] encs;
			encs = NULL;
		}
		ansibuff.clear();
		inputbuff.clear();
		inputfinfo.clear();
		charstat.clear();
		encs = NULL;
		encin = EncUnknown;
		lerrno = Success;
	}
	converter::~converter() {
		clear();
	}
	void converter::_throw(const char* msg, const char* member) const {
		throw(convexc(lerrno, msg, member));
	}
	char* converter::release() {
		char* t = (char*)ansibuff.release();
		clear();
		return t;
	}
	enctype converter::inputenc() const {
		return encin;
	}
	errcodes converter::lasterror() const {
		return lerrno;
	}
	errcodes converter::load(const char* fname) {
		open(fname);
		return lerrno;
	}
	errcodes converter::takeover(const char* _string, const size_t size) {
		if (_string) {
			clear();
			ansibuff.takeover(_string, size);
			lerrno = Success;
		}
		else {
			lerrno = globErrNullPtr;
		}
		return lerrno;
	}
	errcodes converter::savebufferas(const char* filename, enctype encoding) {	
		if (filename) {
			char* t = (char*)encs[encoding]->encode((char*)inputbuff.data(), inputbuff.size() - 1);
			size_t elemsize = ((encoding == EncUCS2BE || encoding == EncUCS2LE) ? 2 : 1);
			size_t len = ((encoding == EncUCS2BE || encoding == EncUCS2LE) ? strlen((unsigned short*)t) : strlen(t));
			FILE* fout = fopen(filename, "wb");
			if (fout) {
				encs[encoding]->addBOM(fout);
				size_t written = fwrite(t, elemsize, len, fout);
				if (written == len) {
					lerrno = Success;
				}
				else {
					lerrno = globErrWrittingFile;
				}
				fclose(fout);
			}
			else {
				lerrno = globErrWrittingFile;
			}
		}
		else {
			lerrno = globErrNullPtr;
		}
		return lerrno;
	}
	errcodes converter::convertfile(const char* filename, enctype encoding) {
		clear();
		lerrno = open(filename);
		if (encin != EncUnknown) {
			lerrno = ConvErrEncodingUndefined;
			return lerrno;
		}
		lerrno = savebufferas(filename, encoding);
		return lerrno;
	}
	char* converter::load2string(const char* fname) {
		clear();
		char* t = NULL;
		open(fname);
		if (encin != EncUnknown) {
			t = release();
		}
		return t;
	}
	void* converter::convert(void* buffer, enctype in, enctype out) {
		void* result = NULL;
		size_t len = 0;
		if (in == EncUCS2BE || in == EncUCS2LE) {
			len = strlen((unsigned short*)buffer);
		}
		else {
			len = strlen((char*)buffer);
		}
		return result;
	}
	// private
	errcodes converter::init() {
		encs = new encoding*[EncEndOffset];
		for (size_t i = 0; i < EncEndOffset; i++) {
			encs[i] = newEncoding((enctype)i);
		}
		return Success;
	}
	encoding* converter::newEncoding(enctype const type) {
		switch (type) {
		case(EncUTF8):
			return new UTF8;
		case(EncUCS2BE):
		case(EncUCS2LE):
			return new UCS2(EncMarkers[type]);
		case(EncCP1251):
			return new CP1251;
		case(EncCP866):
			return new CP866;
		default:
			return new encoding;
		}
	}
	enctype converter::chkFileBOM(FILE* const pFile) {
		if (pFile) {
			encin = EncUnknown;
			size_t _bom = 0;
			byte* bom = (byte*)&_bom;
			_bom = fgetwc(pFile);
			if (_bom == EncMarkers[EncUCS2BE]) {
				lerrno = ConvDefinedBomUCS2BE;
				encin = EncUCS2BE;
			}
			else if (_bom == EncMarkers[EncUCS2LE]) {
				lerrno = ConvDefinedBomUCS2LE;
				encin = EncUCS2LE;
			}
			else {
				bom[2] = (byte)fgetc(pFile);
				if (_bom == EncMarkers[EncUTF8]) {
					lerrno = ConvDefinedBomUTF8;
					encin = EncUTF8;
				}
				else {
					rewind(pFile);
				}
			}
		}
		return encin;
	}
	errcodes converter::open(const char* fname) {
		lerrno = Success;
		if (fname) {
			FILE* srcfile = fopen(fname, "rb");
			if (srcfile) {
				inputfinfo.fname.copy(fname, strlen(fname) + 1);
				inputfinfo.sz = fsize(srcfile);
				chkFileBOM(srcfile);
				size_t insize = fleft(srcfile);
				byte* str = new byte[insize];
				glob::fread(str, insize, srcfile);
				fclose(srcfile);
				inputbuff.takeover((byte*)str, insize);
				if (analyzeEnc() != EncUnknown) {
					if (encin != EncLatinOnly) {
						char* converted = encs[encin]->decode(str, (encin == EncUCS2BE || encin == EncUCS2LE ? insize / 2 : insize));
						ansibuff.takeover(converted, strlen(converted));
					}
					else {
						char* t = new char[insize + 1];
						glob::memcopy(t, str, insize);
						ansibuff.takeover(t, insize + 1);
					}
					return lerrno;
				}
				throw(exception(ConvErrEncodingUndefined, fname, "open", "converter"));
			}
			throw(exception(globInvFileName, fname, "open", "converter"));
		}
		throw(exception(globErrNullPtr, "Arg0", "open", "converter"));
	}
	bool converter::analyzeEnc() {
		if (!chkUtf8bytes()) {
			if (!chkUcs2bytes()) {
				analyzeCP866();
			}
		}
		return (encin == EncUnknown ? false : true);
	}
	bool converter::analyzeCP866() {
		size_t cp866 = 0, cp1251 = 0, notlatin = 0;
		char* src = (char*)inputbuff.data();
		size_t len = inputbuff.size();
		for (size_t i = 0; i < len; i++) {
			unsigned char c = (unsigned char)src[i];
			if (c > 0xbf || c == 0xa8 || c == 0xb8) {
				++notlatin;
				++cp1251;
			}
			if (c > 0x79 && c < 0xb0 || c > 0xdf && c < 0xf2) {
				++notlatin;
				++cp866;
			}
		}
		if (notlatin) {
			if (cp866 || cp1251) {
				double w1 = (double)cp866 / (double)notlatin;
				double w2 = (double)cp1251 / (double)notlatin;
				encin = (w1 > w2 ? EncCP866 : EncCP1251);
			}
		}
		return (encin == EncUnknown ? false : true);
	}
	bool converter::chkUtf8bytes() {
		unsigned char mask = 0x80;
		byte* src = inputbuff.data();
		size_t len = inputbuff.size();
		for (size_t i = 0; i < len; i++) {
			if (!(src[i] & mask))
				(src[i] > 0 ? ++charstat.ansibytes : ++charstat.nullbytes);
			else if (0b11000000 & src[i] == 0b11000000) {
				(chkUtf8byte(src, i) ? ++charstat.utf8bytes : ++charstat.utf8badbytes);
			}
			else {
				++charstat.utf8badbytes;
			}
		}
		if (charstat.utf8bytes && !charstat.utf8badbytes) {
			encin = EncUTF8;
		}
		else if (charstat.ansibytes == len) {
			encin = EncLatinOnly;
		}
		return (encin ? true : false);
	}
	bool converter::chkUtf8byte(const byte* src, size_t &offset) {
		bool result = true;
		byte mask = 0x80;
		byte bytecount = 1;
		const byte* t = &src[offset];
		while (mask >>= 1, mask & *t) {
			if (t[bytecount] & 0b01000000 == 0b10000000) {
				++bytecount;
			}
			else {
				result = false;
				break;
			}
		}
		offset += (bytecount - 1);
		return result;
	}
	bool converter::chkUcs2bytes() {
		size_t
			similars[2] = { 0, 0 },
			simlength[2] = { 0, 0 },
			maxsimlength[2] = { 0, 0 },
			simover6[2] = { 0, 0 },
			diffbytes[2] = { 0, 0 }
		;
		byte* src = inputbuff.data();
		size_t len = inputbuff.size();
		char prev[2] = { src[0], src[1] };
		byte bsz = -1;
		bool* bytes[2];
		bytes[0] = new bool[bsz];
		bytes[1] = new bool[bsz];
		for (size_t i = 0; i < bsz; i++) {
			bytes[0][i] = bytes[1][i] = false;
		}
		for (size_t i = 2; i < len; i++) {
			unsigned char grp = i % 2;
			if (!bytes[grp][(unsigned char)src[i]]) {
				++diffbytes[grp];
				bytes[grp][(unsigned char)src[i]] = true;
			}
			if (src[i] == prev[grp]) {
				if (!(simlength[grp]++))
					++similars[grp];
				if (maxsimlength[grp] < simlength[grp])
					maxsimlength[grp] = simlength[grp];
				if (simlength[grp] == 7)
					++simover6[grp];
			}
			else {
				simlength[grp] = 0;
			}
			prev[grp] = src[i];
		}
		float test = 0;
		test += diffbytestest(diffbytes[0], diffbytes[1]);
		test += simcounttest(similars[0], similars[1]);
		test += simbytestest(diffbytes, similars);
		test += simover6test(similars, simover6);
		if (test)
			encin = (test > 0 ? EncUCS2BE : EncUCS2LE);
		return (encin ? true : false);
	}
	
	float converter::diffbytestest(const size_t n1, const size_t n2) {
		double weight = 0;
		if (n1 < 5)
			weight += 1;
		if (n2 < 5)
			weight -= 1;
		return weight;
	}
	float converter::simcounttest(const size_t n1, const size_t n2) {
		double total = n1 + n2;
		double sims1 = n1 / total, sims2 = n2 / total;
		float limit[2] = { 0.1, 0.9 };
		float weight = 0;
		if (sims1 < limit[0]) {
			weight -= 1;
			if (sims2 > limit[1])
				weight -= 1;
		}
		else if (sims2 < limit[0]) {
			weight += 1;
			if (sims1 > limit[1])
				weight += 1;
		}
		return weight;
	}
	// not finished
	float converter::simbytestest(const size_t* bytes, const size_t* simcount) {
		double n1 = (double)simcount[0];
		double n2 = (double)simcount[1];
		return 0;
	}
	//
	float converter::simover6test(const size_t* simmcount, const size_t* over6) {
		float weight = 0, t1 = (double)over6[0] / (double)simmcount[0], t2 = (double)over6[1] / (double)simmcount[1];
		float limit = 0.05;
		if (t1 < limit) {
			weight -= 1;
		}
		if (t2 < limit) {
			weight += 1;
		}
		return weight;
	}
	size_t converter::chkBOM(const char* src) {
		size_t bom = 0;
		char* t = (char*)&bom;
		t[0] = src[0];
		t[1] = src[1];
		if (bom == EncMarkers[EncUCS2BE]) {
			lerrno = ConvDefinedBomUCS2BE;
			return EncUCS2BE;
		}
		else if (bom == EncMarkers[EncUCS2LE]) {
			lerrno = ConvDefinedBomUCS2LE;
			return EncUCS2LE;
		}
		else {
			t[2] = src[2];
			if (bom == EncMarkers[EncUTF8]) {
				lerrno = ConvDefinedBomUTF8;
				return EncUTF8;
			}
		}
		lerrno = ConvNoBOM;
		return EncUnknown;
	}
	
};

