#include "ftwd/encoding.h"
namespace ftwd {
	namespace enc {
		_encoding _unknown;
		UTF8 _utf8;
		UCS2BE _ucs2be;
		UCS2LE _ucs2le;
		CP1251 _cp1251;
		CP866 _cp866;

		_encoding * const encs[] = {
			&_unknown,
			&_utf8,
			&_ucs2be,
			&_ucs2le,
			&_cp1251,
			&_cp866
		};
	}
	const enc::_encoding *const *const Encodings = enc::encs;

	errcodes encoding::convert(enctypes _Curr, enctypes _Tgt, byte* &_SrcStr, size_t &TotalBytes, bool addMarker) {
		byte* result = nullptr;
		size_t btsz = TotalBytes;
		if (_Curr <= EncLatinOnly) {
			size_t nsize = Encodings[_Curr]->countEncCharBufferSize(_SrcStr, TotalBytes);
			chlib::EncChar* src = new chlib::EncChar[nsize + 1];
			src[nsize] = chlib::CharLib[_Tgt][0];
			size_t offset = 0;
			for (size_t i = 0; i < nsize; i++) {
				src[i] = (chlib::convertByType(_Tgt, _Curr, (enc::encs[_Curr]->encodedToEncChar(_SrcStr, offset)._char)));
				btsz += src[i].size();
			}
			btsz += src[nsize].size();
			result = new byte[btsz];
			size_t rstpos = 0;
			for (size_t i = 0; i <= nsize; i++) {
				src[i].toString((char*)result, rstpos);
			}
			delete[] _SrcStr;
			_SrcStr = result;
			TotalBytes = btsz - src[nsize].size();
		}
		return (ConvErrEncodingUndefined);
	}
	namespace enc {
		const qword EncMarkers[] = {
			0,				/* EncUnknown */
			0xbfbbef,		/* EncUTF8 */
			0xfffe, 0xfeff, /* EncUCS2BE, EncUCS2LE */
			0, 0, 0, 0		/* others */
		};

		const byte EncMarkerSize[] = {
			0,			/* EncUnknown */
			3,			/* EncUTF8 */
			2, 2,		/* EncUCS2BE, EncUCS2LE */
			0, 0, 0, 0	/* others */
		};

		/* class _encoding */
		chlib::EncChar _encoding::ansiToEncChar(const char _Src) const {
			return chlib::CharLib[_type][_Src];
		}
		chlib::EncChar _encoding::encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const {
			if (_Source) {
				return (chlib::CharLib[_type][_Source[_SrcOffset++]]);
			}
			throw(exception(globErrNullPtr, "Arg0", "encodedToChar", "_encoding"));
		}
		size_t _encoding::countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const {
			if (_Buffer) {
				return _BuffSize;
			}
			throw(exception(globErrNullPtr, "Arg0", "countEncCharBufferSize", "_encoding"));
		}
		errcodes _encoding::addBOM(FILE* pFile) const {
			if (pFile) {
				if (EncMarkerSize[_type]) {
					if (fwrite(&EncMarkers[_type], EncMarkerSize[_type], 1, pFile))
						return Success;
					throw(exception(globErrWrittingFile, "Arg0", "addBOM", "_encoding"));
				}
			}
			throw(exception(globErrNullFilePtr, "Arg0", "addBOM", "_encoding"));
		}
		_encoding::_encoding()
			: _type(EncUnknown)
		{}
		_encoding::_encoding(const enctypes _type)
			: _type(_type)
		{}
		_encoding::~_encoding() {
			_type = EncUnknown;
		}
		_encoding& _encoding::operator=(const _encoding &_Src) {
			_type = _Src._type;
			return *this;
		}
		_encoding& _encoding::operator=(const enctypes _EncType) {
			_type = _EncType;
			return *this;
		}
		
		/* utf8 */
		UTF8::UTF8(): _encoding(EncUTF8) {}
		UTF8::~UTF8() {}
		chlib::EncChar UTF8::encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const {
			chlib::EncChar EncChr;
			if (_Source) {
				byte mask = 0x80;
				if (!(_Source[_SrcOffset] & mask)) {
					EncChr._ETS = (1 | (EncUTF8 << 4));
					EncChr.lPos = _Source[_SrcOffset++];
					EncChr._char = _Source[_SrcOffset++];
				}
				else {
					byte bCnt = 0;
					while ((_Source[_SrcOffset] & mask) && (bCnt < 8)) {
						mask >>= 1;
						++bCnt;
					}
					byte limit = (bCnt < chlib::EncCharMaxEncSize ? bCnt : (byte)chlib::EncCharMaxEncSize);
					const byte* const srcBuff = &_Source[_SrcOffset];
					EncChr._ETS = (limit | (EncUTF8 << 4));
					byte* reff = (byte*)EncChr._char;
					for (byte i = 0; i < limit; i++) {
						reff[i] = srcBuff[i];
					}
					_SrcOffset += bCnt;
				}
			}
			else {
				throw(exception(globErrNullPtr, "Arg0", "encodedToEncChar", "UTF8"));
			}
			return EncChr;
		}
		size_t UTF8::countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const {
			size_t _Result = 0;
			if (_Buffer && _BuffSize) {
				byte* _Src = (byte*)_Buffer;
				for (size_t i = 0; i < _BuffSize; i++) {
					bool berr = false;
					if ((byte)0x80 & _Src[i]) {
						byte mask_s = (byte)0x40;
						byte chSize = 1;
						while (mask_s & _Src[i]) {
							if ((_Src[i + chSize] ^ (byte)0b11000000) < (byte)0x80)
								chSize++;
							else {
								berr = true;
								break;
							}
							mask_s >>= 1;
						}
						i += (chSize - 1);
						if (!berr) {
							++_Result;
						}
					}
					else {
						++_Result;
					}
				}
			}
			else if (!_Buffer) {
				throw(exception(globErrNullPtr, "Arg0", "countEncCharBufferSize", "UTF8"));
			}
			return _Result;
		}

		/* ucs2be */
		UCS2BE::UCS2BE(): _encoding(EncUCS2BE) {}
		UCS2BE::~UCS2BE() {}
		chlib::EncChar UCS2BE::encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const {
			if (_Source) {
				chlib::EncChar EChr;
				EChr._ETS = (((byte)EncUCS2BE << 4) | (byte)2);
				byte* reff = (byte*)&EChr._char;
				reff[0] = _Source[_SrcOffset++];
				reff[1] = _Source[_SrcOffset++];
				return EChr;
			}
			throw(exception(globErrNullPtr, "Arg0", "encodedToEncChar", "UCS2BE"));
		}
		size_t UCS2BE::countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const {
			return _BuffSize / 2;
		}

		/* ucs2le */
		UCS2LE::UCS2LE() : _encoding(EncUCS2LE) {}
		UCS2LE::~UCS2LE() {}
		chlib::EncChar UCS2LE::encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const {
			if (_Source) {
				chlib::EncChar EChr;
				EChr._ETS = (((byte)EncUCS2BE << 4) | (byte)2);
				byte* reff = (byte*)&EChr._char;
				reff[0] = _Source[_SrcOffset++];
				reff[1] = _Source[_SrcOffset++];
				return EChr;
			}
			throw(exception(globErrNullPtr, "Arg0", "encodedToEncChar", "UCS2LE"));
		}
		size_t UCS2LE::countEncCharBufferSize(const void* _Buffer, const size_t _BuffSize) const {
			return _BuffSize / 2;
		}

		/* cp1251 */
		CP1251::CP1251(): _encoding(EncCP1251) {}
		CP1251::~CP1251() {}

		/* cp866 */
		CP866::CP866(): _encoding(EncCP866) {}
		CP866::~CP866() {}
		chlib::EncChar CP866::encodedToEncChar(const byte* const _Source, size_t &_SrcOffset) const {
			if (_Source) {
				chlib::EncChar eChr;
				eChr._ETS = (((byte)EncCP866 << 4) | (byte)1);
				eChr._char = _Source[_SrcOffset++];
				return eChr;
			}
			throw(exception(globErrNullPtr, "Arg0", "encodedToChar", "CP866"));
		}
	};
};
