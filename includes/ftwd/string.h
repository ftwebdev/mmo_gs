#pragma once
#include "global.h"

char num2char(byte _Byte) {
	return (_Byte % 10 + '0');
}
char byte2hex(byte _Byte) {
	byte t = _Byte % 0x10;
	return (char)(t += (t > 9 ? ('a' - 10) : '0'));
}


/**
* @class: string
* @version: 1.0
* @author: felexxx67@gmail.com
*/

#define STRING_BUFF_MAXSIZE 4096

	class string;

	struct splitstr {
		size_t size;
		string* data;
		splitstr();
		~splitstr();
	};

	class string {
		BaseDynamicBuffer<char> _string;
		char tolower(const char c) const;
		bool _match(const char* needle, const size_t offset, const size_t nlen) const;
		bool _match(const char* needle, const size_t offset) const;
		bool _match(const string &needle, const size_t offset) const;
		void __clear();
		void __copy(const string &src);
		void __clone(const string &src);
	public:
		/* Structors */
		/// default
		string();
		/// from char
		explicit string(const char c);
		/// from char*
		explicit string(const char* cstring);
		explicit string(size_t i);
		/// copy
		string(const string &src);

		///  destructor
		virtual ~string();

		/* modifiers */
		bool __takeover(char*, const size_t);
		char* __release();
		/// reverse
		void _reverse();
		string reverse() const;
		/// to lower
		void __tolower();
		string tolower() const;
		/// to upper
		void __toupper();
		string toupper() const;
		/// append char
		void append(const char c);
		void operator+=(const char c);
		/// append cstring
		void append(const char* source);
		void operator+=(const char* source);
		/// append string
		void append(const string &source);
		void operator+=(const string &source);
		/// trims heading and trailing spaces
		bool is_space(const char c) const;
		void _trim();
		string trim() const;
		/// cuts all chars from string
		void _trim(const char c);
		string trim(const char c) const;
		/// cuts all subs from string
		void _trim(const char* sub);
		void _trim(const string &sub);
		string trim(const char* sub) const;
		string trim(const string &sub) const;
		bool _is_digit(const char c) const;
		long int _find_first_digit() const;
		long long int _parse_int() const;
		double _parse_float() const;
		/* search */
		/// find a character in stored string
		long int find(const char needle, const size_t offset = 0) const;
		/// find a cstring in stored string
		long int find(const char* needle, const size_t _offset = 0) const;
		/// case insensitive character
		long int find_i(const char needle, const size_t offset = 0) const;
		/// case insensitive cstring
		long int find_i(const char* needle, const size_t _offset = 0) const;
		/// last entry of char
		long int rfind(const char c) const;
		/// last entry of cstring
		long int rfind(const char* needle) const;
		long int rfind(const string &needle) const;
		long int rfind_i(const char c) const;
		long int rfind_i(const char* needle) const;
		long int rfind_i(const string &needle) const;
		/// counts all entries of char
		size_t count(const char c, size_t offset = 0, bool ci = false) const;
		/// case insensitive count
		size_t count_i(const char c, size_t offset = 0, bool ci = false) const;
		/// counts all entries of string
		size_t count(const char* needle, const size_t offset = 0, const  bool ci = false) const;
		size_t count_i(const char* needle, const size_t offset = 0, const  bool ci = false) const;
		/* copy makers */
		/// from char
		void copy(const char c);
		/// other way to make a copy
		void copy(const string &other);
		/// more copies
		string copy() const;

		/* compare */
		/// string 2 string
		bool compare(const string &other) const;
		/// string to cstring
		bool compare(const char* &str);
		// case insensitive
		/// string to string
		bool compare_i(const string &other) const;
		/// string to cstring
		bool compare_i(const char* &str);
		/// equal
		bool operator == (const string &other) const;
		/// not equal
		bool operator!=(const string &other) const;

		/* Getters */
		/// get length
		size_t length() const;
		const char* c_str() const;
		/// part of string
		string substr(const long int offset, const long int _count = 0) const;
		/// splits the string to a list
		void split(splitstr &strings, const char* needle, const size_t offset = 0, const size_t limit = 0) const;
		/* operators */
		operator char*() const;
		/// string as bool
		//operator bool();
		const char operator[](const size_t i) const;
		char& operator[](const size_t i);
		bool operator==(const string &other);
		bool operator!=(const string &other);
		bool operator==(const char* cstr);
		bool operator!=(const char* cstr);
		void operator=(const string &src);
		void operator=(const char* cstring);
		void operator=(const char c);
		string operator+(const char c) const;
		string operator+(const char* str) const;
		string operator+(const string &other) const;

		/* ostream */
		/// std::cerr
		std::basic_ostream<char>& operator>>(std::basic_ostream<char> &stream) const;
		/// FILE*
		void operator>>(FILE* const pFile) const;
		/* istream */
		bool get_line(FILE* pFile, long int offset = -1);
		bool get_Word(FILE* pFile, long int offset = -1);
		/// FILE*
		void operator<<(FILE* pFile);
	};
	/*
	*	end of class string
	*/
	FILE* operator<<(FILE* const pFile, const string &str);
	FILE* operator>>(FILE* const pFile, string &str);
	string operator+(const char* cstring, string b);
