#define EXCEPTIONDLL_EXPORT
#include "ftwd/exception.h"
namespace ftwd {
    char* exception::__utctime(char* const buff, const time_t t, const char sep) {
        tm* now = gmtime(&t);
        buff[2] = buff[5] = sep;
        buff[10] = ' ';
        buff[13] = buff[16] = ':';
        buff[19] = 0;
        now->tm_mon++;
        now->tm_year += 1900;
        for (char i = 1; i >= 0; i--) {
            buff[i] = now->tm_mday % 10 + '0';
            now->tm_mday /= 10;
        }
        for (char i = 4; i > 2; i--) {
            buff[i] = now->tm_mon % 10 + '0';
            now->tm_mon /= 10;
        }
        for (char i = 9; i > 5; i--) {
            buff[i] = now->tm_year % 10 + '0';
            now->tm_year /= 10;
        }
        buff[11] = now->tm_hour / 10 + '0';
        buff[12] = now->tm_hour % 10 + '0';

        buff[14] = now->tm_min / 10 + '0';
        buff[15] = now->tm_min % 10 + '0';

        buff[17] = now->tm_sec / 10 + '0';
        buff[18] = now->tm_sec % 10 + '0';
        return buff;
    }
    void exception::__copy(char* const _TgtString, const char* const _SrcString, const size_t _TgtLength) {
        if(_TgtString && _SrcString) {
            size_t maxLength = _TgtLength - 1;
            size_t i = 0;
            while (_SrcString[i] && i < maxLength) {
                _TgtString[i] = _SrcString[i];
                ++i;
            }
            _TgtString[i] = 0;
        }
        else {
            _TgtString[0] = 0;
        }
    }
    exception::exception(errcodes const code, const char* const msg, const char* const thrfunc, const char* const thrclass)
        : _code(code < UnknownError ? code : UnknownError)
    {
        __copy(this->msg, msg, ExcDefltMsgSize);
        __copy(this->thrmember, thrfunc, ExcDefltMemberNameSize);
        __copy(this->thrclass, thrclass, ExcDefltClassNameSize);
        __utctime(thrtime, time(0), '-');
    }
    exception::~exception() {}
    char const * const exception::what() const {
        return ftwd::ErrMsgs[_code];
    }
    errcodes const exception::code() const {
        return _code;
    }
    std::basic_ostream<char>& exception::operator>>(std::basic_ostream<char> &stream) const {
        stream << thrtime << "\tException: " << ErrMsgs[_code] << "\n";
        if (msg[0])
            stream << "Message: " << msg << "\n";
        if (thrclass[0])
            stream << thrclass << "::";
        if(thrmember[0])
            stream << thrmember << "()\n";
        stream << "\n";
        FILE* excLog = fopen("ftwd-exc.log", "a");
        return stream;
    }
    FILE* exception::operator>>(FILE* const fout) const {
        fputs(thrtime, fout);
        fputs("\tException: ", fout);
        fputs(ErrMsgs[_code], fout);
        fputs("\n", fout);
        if (msg[0]) {
            fputs(msg, fout);
            fputs("\n", fout);
        }
        if (thrclass[0]) {
            fputs(thrclass, fout);
            fputs("::", fout);
        }
        if (thrmember[0]) {
            fputs(thrmember, fout);
            fputs("()\n", fout);
        }
        fputs("\n", fout);
        return fout;
    }
    std::basic_ostream<char>& operator<<(std::basic_ostream<char> &stream, exception &e) {
        e >> stream;
        return stream;
    }
    FILE* operator<<(FILE* fout, exception &e) {
        e >> fout;
        return fout;
    }
};
