#include "ftwd/CharLib.h"

/* charlibs */
#include "libUTF8.h"
#include "libUTF8.h"
#include "libUCS2BE.h"
#include "libUCS2LE.h"
#include "libCP1251.h"
#include "libCP866.h"

namespace ftwd {
	namespace chlib {


		const EncChar *const _CharLib[EncLatinOnly] = {
			(EncChar*)lib_unknown::lib,
			(EncChar*)lib_utf8::lib,
			(EncChar*)lib_ucs2be::lib,
			(EncChar*)lib_ucs2le::lib,
			(EncChar*)lib_cp1251::lib,
			(EncChar*)lib_cp866::lib,
		};

		const EncChar *const *const CharLib = _CharLib,
			*const LibUnknown = _CharLib[0],
			*const LibUTF8 = _CharLib[1],
			*const LibUCS2BE = _CharLib[2],
			*const LibUCS2LE = _CharLib[3],
			*const LibCP1251 = _CharLib[4],
			*const LibCP866 = _CharLib[5]
		;

		const EncChar static &searchUnknown(dword _EncChar) {
			byte typeStart = (_EncChar > 0xff ? EncUTF8 : EncCP1251), typeEnd = (_EncChar > 0xff ? EncCP1251 : EncLatinOnly);
			for (byte type = typeStart; type < typeEnd; type++) {
				for (word _char = 0; _char < ChLibCharsPerType; _char++) {
					if (CharLib[type][_char]._char == _EncChar) {
						return CharLib[type][_char];
					}
				}
			}
			return CharLib[EncUnknown][(byte)'?'];
		}
		const EncChar static &getByType(const enctypes _TypeOut, const enctypes _TypeIn, const dword iChar) {
			for (word i = 0; i < ChLibCharsPerType; i++)
				if (iChar == CharLib[_TypeIn][i]._char && CharLib[_TypeIn][i].lPos == i)
					return CharLib[_TypeOut][i];
		}
		const EncChar static &getByPos(const enctypes _TypeOut, const byte _LPos) {
			return CharLib[_TypeOut][_LPos];
		}
		// much costable by time and resources function that search characters in lib
		EncChar convert(const enctypes _TypeOut, const EncChar &_SrcChar) {
			enctypes type = _SrcChar.type();
			size_t lpos = 0;
			byte flag = 0;
			if (!_SrcChar.lPos && type == EncUnknown) {
				for (size_t i = EncUTF8; i < EncLatinOnly; i++) {
					for (size_t j = 0; j < ChLibCharsPerType; j++) {
						if (chlib::CharLib[i][j]._char != _SrcChar._char);
						else {
							type = chlib::CharLib[i][j].type();
							lpos = chlib::CharLib[i][j].lPos;
							flag = 1;
							break;
						}
					}
					if (type == EncUnknown);
					else { break; }
				}
			}
			else if (!_SrcChar.lPos) {
				return convertByType(_TypeOut, _SrcChar.type(), _SrcChar._char);
			}
			else {
				return convertByPos(_TypeOut, _SrcChar.lPos);
			}
			return chlib::CharLib[type][(flag ? lpos : '?')];
		}
		// get position of char and convert to other type
		EncChar convertByType(const enctypes _TypeOut, const enctypes _TypeIn, const dword iChar) {
			size_t lpos = 0;
			byte flag = 0;
			for (; lpos < ChLibCharsPerType; lpos++) {
				if (chlib::CharLib[_TypeIn][lpos]._char != iChar);
				else {
					flag = 1;
					break;
				}
			}
			return chlib::CharLib[_TypeOut][(flag ? lpos : '?')];
		}
		// get char from type and position
		EncChar convertByPos(const enctypes _TypeOut, const byte _LPos) {
			return chlib::CharLib[_TypeOut][_LPos];
		}
	};

};
