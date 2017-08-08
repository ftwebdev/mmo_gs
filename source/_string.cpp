#include "string.h"

	char string::tolower(const char c) const {
		if (c > 64 && c < 91) {
			return (c + 32);
		}
		return c;
	}
	bool string::_match(const char* needle, const size_t offset, const size_t nlen) const {
		if ((offset + nlen) > len_) { return false; }
		size_t count_ = 0;
		for (size_t i = 0; i < nlen; i++) {
			if (string_[i] == needle[i]) {
				++count_;
			}
			else {
				break;
			}
		}
		return (count_ == nlen ? true : false);
	}
	bool string::_match(const char* needle, const size_t offset) const {
		size_t nlen = get_len(needle);
		size_t count_ = 0;
		if ((offset + nlen) > len_)
			return false;
		return _match(needle, offset, nlen);
	}
	bool string::_match(const string &needle, const size_t offset) const {
		return _match(needle.string_, offset, needle.len_);
	}
	void string::__clear() {
		if (reffs_ != NULL && reffs_->dec()) {
			if (string_ != NULL) {
				delete[] string_;
				string_ = NULL;
			}
			delete reffs_;
			reffs_ = NULL;
			len_ = 0;
		}
	}
	void string::__copy(const string &src) {
		len_ = src.len_;
		string_ = src.string_;
		reffs_ = src.reffs_;
		if (reffs_ != NULL) {
			reffs_->inc();
		}
	}
	void string::__clone(const string &src) {
		if (len_ = src.len_) {
			if (string_ = copy(src.string_, src.len_)) {
				reffs_ = new refcounter;
				if (reffs_ != NULL) {}
				else {
					delete[] string_;
					len_ = 0;
				}
			}
			else {
				len_ = 0;
			}
		}

	}
	/* Structors */
	/// default
	string::string()
		: len_(0)
		, string_(NULL)
		, reffs_(NULL)
	{}
	/// from char
	string::string(const char c)
		: len_(1)
		, string_(NULL)
		, reffs_(NULL)
	{
		if (string_ = new char[2]) {
			if (reffs_ = new refcounter) {
				string_[0] = c;
				string_[1] = '\0';
			}
			else {
				delete[] string_;
				len_ = 0;
			}
		}
		else {
			len_ = 0;
		}
	}
	/// from char*
	string::string(const char* cstring)
		: len_(0)
		, string_(NULL)
		, reffs_(NULL)
	{
		if (cstring != NULL) {
			if (len_ = get_len(cstring)) {
				if (string_ = copy(cstring, len_)) {
					if (reffs_ = new refcounter);
					else {
						delete[] string_;
						len_ = 0;
					}
				}
				else {
					len_ = 0;
				}
			}
		}
	}
	/// copy
	string::string(const string &src)
		: len_(0)
		, string_(NULL)
		, reffs_(NULL)
	{
		__clone(src);
	}
	string::string(size_t i)
		: len_(i)
		, string_(NULL)
		, reffs_(NULL)
	{
		if (len_) {
			if (reffs_ = new refcounter) {
				if (string_ = new char[len_ + 1]) {
					for (size_t i = 0; i < len_; i++) {
						string_[i] = ' ';
					}
					string_[len_] = '\0';
				}
				else {
					delete reffs_;
					len_ = 0;
				}
			}
			else {
				len_ = 0;
			}
		}
	}
	///  destructor
	string::~string() {
		__clear();
	}

	/* modifiers */
	bool string::__takeover(char* buffer, const size_t size) {
		__clear();
		if (reffs_ = new refcounter) {
			len_ = size;
			string_ = buffer;
			return true;
		}
		return false;
	}
	char* string::__release() {
		char* t = NULL;
		if (reffs_->dec()) {
			t = string_;
		}
		else {
			t = this->copy(string_, len_);
		}
		string_ = NULL;
		reffs_ = NULL;
		len_ = 0;
		return t;
	}
	/// reverse
	void string::_reverse() {
		if (string_ != NULL) {
			size_t i, npos;
			for (i = 0; i < len_; i++) {
				char t = string_[i];
				npos = len_ - (i + 1);
				string_[i] = string_[npos];
				string_[npos] = t;
			}
		}
	}
	string string::reverse() const {
		string t(*this);
		t._reverse();
		return t;
	}
	/// to lower
	void string::__tolower() {
		for (size_t i = 0; i < len_; i++) {
			if (string_[i] > 64 && string_[i] < 91) {
				string_[i] += 32;
			}
		}
	}
	string string::tolower() const {
		string temp(*this);
		temp.__tolower();
		return temp;
	}
	/// to upper
	void string::__toupper() {
		for (size_t i = 0; i < len_; i++) {
			if (string_[i] > 96 && string_[i] < 123) {
				string_[i] -= 32;
			}
		}
	}
	string string::toupper() const {
		string temp(*this);
		temp.__toupper();
		return temp;
	}
	/// append char
	void string::append(const char c) {
		if (c == '\0') return;
		char* buffer = new char[len_ + 2];
		for (size_t i = 0; i < len_; i++)
			buffer[i] = string_[i];
		buffer[len_++] = c;
		buffer[len_] = '\0';
		if (len_ > 1)
			delete[] string_;
		string_ = buffer;
	}
	void string::operator+=(const char c) {
		append(c);
	}
	/// append cstring
	void string::append(const char* source) {
		size_t srclen = get_len(source);
		size_t nlen = len_ + srclen;
		char* buffer = new char[nlen + 1];
		buffer[nlen] = '\0';
		size_t i = 0;
		size_t j = 0;
		for (; i < nlen; i++) {
			buffer[i] = (i < len_ ? string_[i] : source[j++]);
		}

		if (string_)
			delete[] string_;
		len_ = nlen;
		string_ = buffer;
	}
	void string::operator+=(const char* source) {
		append(source);
	}
	/// append string
	void string::append(const string &source) {
		if (this != &source) {
			append(source.string_);
		}
		return;
	}
	void string::operator+=(const string &source) {
		append(source.string_);
	}
	/// trims heading and trailing spaces
	bool string::is_space(const char c) const {
		return (c < 33 || c > 126);
	}
	void string::_trim() {
		if (!len_) return;
		size_t headof = 0, tailof = 0;
		for (size_t i = 0; i < len_; i++) {
			if (isspace(string_[i]));
			else {
				headof = i;
				break;
			}
		}
		for (size_t i = len_ - 1; i > headof; i--) {
			if (isspace(string_[i]));
			else {
				tailof = i + 1;
				break;
			}
		}
		size_t nsize = tailof - headof;
		if (nsize > 0) {
			char* buffer = new char[nsize + 1];
			if (buffer != NULL) {
				for (size_t i = 0; i < nsize; i++) {
					buffer[i] = string_[i + headof];
				}
				buffer[nsize] = 0;
				__clear();
				reffs_ = new refcounter;
				if (reffs_) {
					len_ = nsize;
					string_ = buffer;
					return;
				}
			}
			throw("string::_trim() 'Out of free memory...'\n");
		}
	}
	string string::trim() const {
		string t(*this);
		t._trim();
		return t;
	}
	/// cuts all chars from string
	void string::_trim(const char c) {
		size_t chars = count(c);
		if (chars) {
			size_t nsize = len_ - chars;
			if (nsize) {
				char* temp = new char[nsize + 1];
				for (size_t i = 0, j = 0; i < len_; i++) {
					if (string_[i] != c) {
						temp[j++] = string_[i];
					}
				}
				temp[nsize] = '\0';
				delete[] string_;
				string_ = temp;
				len_ = nsize;
			}
			else {
				if (string_ != NULL) {
					delete[] string_;
					string_ = NULL;
				}
				len_ = 0;
			}
		}
		return;
	}
	string string::trim(const char c) const {
		string temp(*this);
		temp._trim(c);
		return temp;
	}
	/// cuts all subs from string
	void string::_trim(const char* sub) {
		if (!len_) return;
		size_t len = get_len(sub);
		long int pos, offset = 0;
		string temp;
		while ((pos = find(sub, offset)) != -1) {
			temp.append(substr(offset, pos - offset));
			offset = pos + len;
		}
		len_ = get_len(string_);
		string_ = copy(temp.string_, len_);
	}
	void string::_trim(const string &sub) {
		_trim(sub.string_);
	}
	string string::trim(const char* sub) const {
		string temp(*this);
		temp._trim(sub);
		return temp;
	}
	string string::trim(const string &sub) const {
		string temp(*this);
		temp._trim(sub);
		return temp;
	}
	// to numeric
	bool string::_is_digit(const char c) const {
		return (c > 47 && c < 58);
	}
	/********************************************
	*	TODO: Mus do this two functions
	*	trhows char* if cant parse
	********************************************/
	long int string::_find_first_digit() const {
		int result = -1;
		size_t i;
		for (i = 0; i < len_; i++) {
			if (_is_digit(string_[i])) {
				result = i;
				break;
			}
		}
		return 0;
	}
	long long int string::_parse_int() const {
		string t(*this);
		long int pos = _find_first_digit();
		long long int res = 0;
		if (pos != -1) {
			//count digits
			size_t start = (unsigned long long)pos;
			size_t end;
			for (end = start; end < len_; ++end) {
				if (!_is_digit(string_[end]))
					break;
			}
			size_t n = 1;
			for (size_t i = 0; i < end; i++) {
				pos = end - i - 1;
				char digit = string_[pos] - '0';
				res += digit * l2c4::pow(10, i);
			}
			if (start > 0 && string_[start - 1] == '-')
				res /= (-1);
		}
		else {
			error err_(2, "������ �� �������� ����� �����...");
			err_.thr_class = "string";
			err_.thr_func = "_parse_int";
			throw("-nan-");
		}
		return res;
	}
	double string::_parse_float() const {
		return 0.0;
	}
	/*******************************************/

	/* search */
	/// find a character in stored string
	long int string::find(const char needle, const size_t offset) const {
		if (needle != '\0' && offset <= len_)
			for (size_t i = offset; i < len_; i++) {
				if (string_[i] != needle);
				else
					return i;
			}
		return -1;
	}
	/// find a cstring in stored string
	long int string::find(const char* needle, const size_t _offset) const {
		size_t nsize = get_len(needle);
		long int offset = find(needle[0], _offset);
		if ((offset != -1) && ((_offset + nsize) <= len_)) {
			while (offset != -1 && offset < len_) {
				size_t i = 0;
				for (; i < nsize; i++) {
					if (string_[offset + i] == needle[i]) {
					}
					else {
						break;
					}
				}
				if (i == nsize)
					return offset;
				offset = find(needle[0], offset + i);
			}
		}
		return -1;
	}
	/// case insensitive character
	long int string::find_i(const char needle, const size_t offset) const {
		string a = tolower();
		char needle_ = tolower(needle);
		return a.find(needle);
	}
	/// case insensitive cstring
	long int string::find_i(const char* needle, const size_t _offset) const {
		string a = tolower(), b(needle);
		b.__tolower();
		return a.find(b.string_);
	}
	/// last entry of char
	long int string::rfind(const char c) const {
		long int pos = -1;
		for (size_t i = len_; i > 0; i--) {
			if (string_[i - 1] == c) {
				pos = i - 1;
				break;
			}
		}
		return pos;
	}
	/// last entry of*
	long int string::rfind(const char* needle) const {
		long int pos = -1;
		size_t nlen = get_len(needle);
		for (size_t i = len_ - nlen; i > 0; i--) {
			if (string_[i] == needle[0]) {
				if (_match(needle, i - 1, nlen))
					return (i - 1);
			}
		}
		return pos;
	}
	long int string::rfind(const string &needle) const {
		long int pos = -1;
		char* nstring = (char*)needle.c_str();
		for (size_t i = len_ - needle.len_; i > 0; i--) {
			if (string_[i] == nstring[0]) {
				if (_match(needle.c_str(), i - 1, needle.len_))
					return (i - 1);
			}
		}
		return pos;
	}
	long int string::rfind_i(const char c) const {
		char temp_c = c;
		string t(*this);
		t.__tolower();
		return t.rfind(temp_c);
	}
	long int string::rfind_i(const char* needle) const {
		string temp(*this);
		string _needle(needle);
		temp.__tolower();
		_needle.__tolower();
		return temp.rfind(_needle);
	}
	long int string::rfind_i(const string &needle) const {
		string t(*this);
		string _needle(needle);
		t.__tolower();
		_needle.__tolower();
		return t.rfind(_needle);
	}

	/// counts all entries of char
	size_t string::count(const char c, size_t offset, bool ci) const {
		size_t count_ = 0;
		for (size_t i = offset; i < len_; i++) {
			if (string_[i] != c);
			else
				++count_;
		}
		return count_;
	}
	/// counts all entries of string
	size_t string::count(const char* needle, const size_t offset, const  bool ci) const {
		size_t count_ = 0;
		size_t nlen = get_len(needle);
		long int pos;
		for (size_t offset_ = offset; pos = find(needle, offset_), pos != (-1); ) {
			++count_;
			offset_ = pos + nlen;
		}
		return count_;
	}
	/// caseinsensitive count
	size_t string::count_i(const char c, size_t offset, bool ci) const {
		size_t count_ = 0;
		for (size_t i = offset; i < len_; i++) {
			if (tolower(string_[i]) != tolower(c));
			else
				++count_;
		}
		return count_;
	}
	size_t string::count_i(const char* needle, const size_t offset, const  bool ci) const {
		size_t count_ = 0;
		size_t nlen = get_len(needle);
		long int pos;
		for (size_t offset_ = offset; pos = find_i(needle, offset_), pos != (-1); ) {
			++count_;
			offset_ = pos + nlen;
		}
		return count_;
	}

	/* copy makers */
	/// copy char
	void string::copy(const char c) {
		__clear();
		len_ = 1;
		string_ = new char[2];
		string_[0] = c;
		string_[1] = '\0';
	}
	/// other way to make a copy
	void string::copy(const string &other) {
		if (this != &other) {
			__clear();
			__clone(other);
		}
	}
	/// more copies
	string string::copy() const {
		return string(*this);
	}

	/* compare */
	/// string 2 string
	bool string::compare(const string &other) const {
		if (len_ == other.len_) {
			for (size_t i = 0; i < len_; i++) {
				if (string_[i] != other.string_[i])
					return false;
			}
			return true;
		}
		return false;
	}
	/// string 2 cstring
	bool string::compare(const char* &str) {
		string temp(str);
		return compare(temp);
	}
	// case insensitive
	/// string to string
	bool string::compare_i(const string &other) const {
		string a = tolower(), b = other.tolower();
		return a.compare(b);
	}
	/// string to cstring
	bool string::compare_i(const char* &str) {
		string a = tolower(), b(str);
		b.tolower();
		return a.compare(b);
	}
	/// equal
	bool string::operator == (const string &other) const {
		return compare(other);
	}
	/// not equal
	bool string::operator!=(const string &other) const {
		return !compare(other);
	}

	/* Getters */
	/// get length
	size_t string::length() const {
		return len_;
	}
	const char* string::c_str() const {
		return string_;
	}
	/// part of string
	string string::substr(const long int offset, const long int _count) const {
		size_t start, end;
		start = (offset < 0 ? len_ + offset : offset);
		end = (_count > 0 ? offset + _count : len_ + _count);
		long int count_ = end - start;
		if (count_ > 0) {
			char* buffer = new char[count_ + 1];
			buffer[count_] = '\0';
			for (size_t i = start; i < end; i++) {
				buffer[i - start] = string_[i];
			}
			string temp(buffer);
			delete[] buffer;
			return temp;
		}
		else {
			return string();
		}
	}
	/// splits the string to a list
	void string::split(splitstr &strings, const char* needle, const size_t offset, const size_t limit) const {
		size_t limit_ = (limit > 1 ? limit - 1 : limit);
		size_t entries;
		entries = this->count(needle, offset, false);
		size_t rounds;
		size_t nlen = get_len(needle);
		if (limit) {
			rounds = (entries > limit ? limit : entries);
		}
		else {
			rounds = entries;
		}
		strings.size = rounds + 1;
		strings.data = (string*)malloc(sizeof(*this) * (rounds + 1));
		size_t offset_ = offset;
		long int pos;
		for (size_t i = 0; pos = find(needle, offset_), pos != (-1); i++) {
			new (strings.data + i) string(pos == offset_ ? string() : substr(offset_, pos - offset_));
			offset_ = pos + nlen;
		}
		new (strings.data + rounds) string(substr(offset_));
	}

	/* operators */
	string::operator char*() const {
		if (string_)
			return string_;
		return "";
	}
	/*/// string as bool
	string::operator bool() {
	return (len_ ? true : false);
	}*/
	const char string::operator[](const size_t i) const {
		if (i <= len_) {
			return string_[i];
		}
		return '\0';
	}
	char& string::operator[](const size_t i) {
		if (i <= len_) {
			return string_[i];
		}
	}
	bool string::operator==(const string &other) {
		return strcomp(string_, other.string_);
	}
	bool string::operator!=(const string &other) {
		return !strcomp(string_, other.string_);
	}
	bool string::operator==(const char* cstr) {
		return strcomp(string_, cstr);
	}
	bool string::operator!=(const char* cstr) {
		return !strcomp(string_, cstr);
	}
	void string::operator=(const string &src) {
		if (this != &src) {
			__clear();
			__copy(src);
		}
	}
	void string::operator=(const char* cstring) {
		if (string_ != cstring) {
			__clear();
			if (cstring != NULL && cstring != "") {
				len_ = get_len(cstring);
				string_ = copy(cstring, len_);
			}
		}
	}
	void string::operator=(const char c) {
		copy(c);
	}
	string string::operator+(const char c) const {
		string temp(*this);
		temp.append(c);
		return temp;
	}
	string string::operator+(const char* str) const {
		string temp(*this);
		temp.append(str);
		return temp;
	}
	string string::operator+(const string &other) const {
		string temp(*this);
		temp.append(other);
		return temp;
	}

	/* ostream */
	/// std::cerr
	std::basic_ostream<char>& string::operator>>(std::basic_ostream<char> &stream) const {
		if (string_ != NULL) {
			stream << string_;
		}
		return stream;
	}
	/// FILE*
	void string::operator>>(FILE* const pFile) const {
		if (string_) {
			for (size_t i = 0; i < len_; i++) {
				fputc(string_[i], pFile);
			}
		}
	}
	/* istream */
	bool string::get_line(FILE* const pFile, long int offset) {
		if (pFile != NULL) {
			__clear();
			string_ = getline(pFile);
			if (string_ != NULL) {
				len_ = get_len(string_);
				if (len_ > 0) {
					reffs_ = new refcounter;
					if (reffs_) {
						return true;
					}
					delete[] string_;
				}
			}
		}
		return false;
	}
	bool string::get_Word(FILE* pFile, long int offset) {
		if (feof(pFile)) {
			return false;
		}
		char c = -1;
		if (offset == -1) {
			offset = ftell(pFile);
		}
		fseek(pFile, 0, SEEK_END);
		size_t filesize = ftell(pFile);
		fseek(pFile, offset, SEEK_SET);
		bool wflag = false;
		size_t wcount = 0;
		size_t wlen = 0;
		size_t word_offset;
		char* buffer = new char[STRING_BUFF_MAXSIZE];
		for (size_t i = offset; i < filesize; i++) {
			c = fgetc(pFile);
			if (!is_space(c)) {
				buffer[wlen++] = c;
				if (wflag) {
				}
				else {
					word_offset = ftell(pFile) - 1;
					++wcount;
					wflag = true;
				}
			}
			else {
				if (wflag) {
					buffer[wlen] = '\0';
					len_ = wlen;
					string_ = copy(buffer, len_);

					wlen = 0;
					break;
				}
				wflag = false;
			}
		}
		delete[] buffer;
		return (c == EOF ? false : true);
	}
	/// FILE*
	void string::operator<<(FILE* pFile) {
		get_Word(pFile);
	}

	/*
	*	end of class string
	*/

	splitstr::splitstr()
		: size(0)
		, data(NULL)
	{}
	splitstr::~splitstr() {
		if (data != NULL) {
			for (size_t i = 0; i < size; i++) {
				(data + i)->~string();
			}
			free(data);
		}
	}

	FILE* operator<<(FILE* const pFile, const string &str) {
		str >> pFile;
		return pFile;
	}
	FILE* operator>>(FILE* const pFile, string &str) {
		str.get_Word(pFile);
		return pFile;
	}
	string operator+(const char* cstring, string b) { return (string(cstring) + b); }

	std::basic_ostream<char>& operator<<(std::basic_ostream<char> &stream, const string &str) {
		if (str.c_str() != NULL) {
			stream << str.c_str();
		}
		return stream;
	}

