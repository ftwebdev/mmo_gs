#include "ftwd/EncChar.h"

namespace ftwd {
    namespace chlib {
        EncChar::EncChar(): lPos(0), _ETS(0), _char(0) {}
        EncChar::EncChar(byte const * const _Array, size_t &pos, byte const _Libpos) {
            parse(_Array, pos, _Libpos);
        }
		byte* EncChar::dataref() const {
			return (byte*)&_char;
		}
        EncChar& EncChar::clear() {
			lPos = _ETS = 0;
			_char = 0;
            return *this;
        }
        EncChar& EncChar::parse(byte const * const _Array, size_t &pos, byte const _Libpos) {
			lPos = _Libpos;
			_ETS = _Array[pos++];
			_char = 0;
			byte size = _ETS & 0xf;
			byte* reff = (byte*)&_char;
            for (byte i = 0; i < size; i++)
                 reff[i] = _Array[pos++];
            return *this;
        }
        enctypes EncChar::type() const {
            return (enctypes)(_ETS >> 4);
        }
        byte EncChar::size() const {
            return (_ETS & 0x0f);
        }
        char* const EncChar::toString(char * const _Dest, size_t &_DestPos) const {
            if(_Dest) {
                char* const tgt = &_Dest[_DestPos];
                const char* const src = (char*)&_char;
                byte size = _ETS & 0xf;
                for(byte i = 0; i < size; i++) {
                    tgt[i] = src[i];
                    ++_DestPos;
                }
            }
            return _Dest;
        }
        EncChar& EncChar::operator=(EncChar const &src){
			lPos = src.lPos;
			_ETS = src._ETS;
			_char = src._char;
            return *this;
        }
        FILE* EncChar::operator>>(FILE* const Cpp) const {
			if (Cpp) {
				char buff[7];
				fputs("{ ", Cpp);
				const byte* arr = &lPos;
				for (byte i = 0; i < EncCharTSize; i++) {
					fputs(int2hex(buff, arr[i], true, 1), Cpp);
					fputs(((i < (EncCharTSize - 1)) ? ", " : " "), Cpp);
				}
				fputc('}', Cpp);
			}
			else {
				throw(exception(globErrNullFilePtr, "Arg0", "operator>>(FILE* const)", "ftwd::chlib::EncChar"));
			}
			return Cpp;
        }
        std::basic_ostream<char>& EncChar::operator>>(std::basic_ostream<char> &stream) const {
            byte _size = (_ETS & 0xf);
            const byte* _arr = (byte*)&_char;
            for(byte i = 0; i < _size; i++)
                stream << _arr[i];
            return stream;
        }
        bool EncChar::operator==(EncChar const &src) const {
            return (_char == src._char);
        }
        bool EncChar::operator!=(EncChar const &src) const {
			return (_char != src._char);
        }
        EncChar::operator unsigned short() const {
            return (unsigned short)_char;
        }
        EncChar::operator unsigned long() const {
            return _char;
        }
    }
}