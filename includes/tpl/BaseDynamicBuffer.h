#pragma once

/**
* @class: BaseDynamicBuffer
* @version: 1.0
* @author: felexxx67@gmail.com
*/

#include "ftwd/types.h"
#include "ftwd/exception.h"

namespace ftwd {
	enum eBaseDynamicBuffer {
		BaseDynBuffRCnt,
		BaseDynBuffLen,
		BaseDynBuffData,
		BaseDynBuffHSize
	};

	template<typename T>
	class BaseDynamicBuffer {
		typedef unsigned char byte;
	protected:
		size_t* _state;
	private:
		/* init block */
		BaseDynamicBuffer<T>& init() {
			if (!_state) {
				_state = new size_t[BaseDynBuffHSize];
				for (byte i = 0; i < BaseDynBuffHSize; i++) {
					_state[i] = 0;
				}
				inc();
			}
			return *this;
		}
		// buffer as other type
		template<typename U>
		BaseDynamicBuffer(BaseDynamicBuffer<U>& src)
			: _state(src._state)
		{
			if (!_state) {
				src.init();
				_state = src._state;
			}
			inc();
		}
	public:
		// default constructor
		BaseDynamicBuffer()
			: _state(NULL)
		{}
		// constructor
		BaseDynamicBuffer(const T* src, const size_t srclen)
			: _state(NULL)
		{
			copy(src, srclen);
		}
		// make a copy of instance with incrementing reffs count
		BaseDynamicBuffer(BaseDynamicBuffer<T>& src)
			: _state(src._state)
		{
			if (!_state) {
				src.init();
				_state = src._state;
			}
			inc();
		}

		// destructor
		virtual ~BaseDynamicBuffer() {
			if (_state != NULL && _state[BaseDynBuffRCnt] && !(--_state[BaseDynBuffRCnt])) {
				if (_state[BaseDynBuffData])
					delete[](byte*)_state[BaseDynBuffData];
				delete[] _state;
			}
			_state = NULL;
		}
		/*	*/

		/* getters block */
		virtual const size_t size() const {
			return (_state ? (_state[BaseDynBuffLen] / sizeof(T)) : 0);
		}
		virtual bool isEmpty() const {
			return (_state[BaseDynBuffData] == 0);
		}
		virtual T* data() const {
			if (_state) {
				return (T*)_state[BaseDynBuffData];
			}
			return NULL;
		}
		/*	*/

		/* setters block */
	private:
		size_t inc() {
			if (_state) {
				return ++_state[BaseDynBuffRCnt];
			}
			return 0;
		}
		size_t dec() {
			if (_state && _state[BaseDynBuffRCnt]) {
				return --_state[BaseDynBuffRCnt];
			}
			return 0;
		}
	public:
		// clear instance all except refference counter
		virtual BaseDynamicBuffer<T>& clear() {
			if (_state) {
				if (_state[BaseDynBuffData] != 0) {
					delete[](byte*)_state[BaseDynBuffData];
					_state[BaseDynBuffData] = 0;
				}
				_state[BaseDynBuffLen] = 0;
			}
			return *this;
		}
		// clone instance (deep copy)
		virtual BaseDynamicBuffer<T>& clone(const BaseDynamicBuffer<T>& src) {
			if (src._state != _state) {
				clear();
				if (!_state)
					init();
				if (src._state[BaseDynBuffData] && src._state[BaseDynBuffLen]) {
					_state[BaseDynBuffLen] = src._state[BaseDynBuffLen];
					byte* source = (byte*)src._state[BaseDynBuffData];
					byte* target = new byte[_state[BaseDynBuffLen]];
					for (size_t i = 0; i < _state[BaseDynBuffLen]; i++)
						target[i] = source[i];
					_state[BaseDynBuffData] = (size_t)target;
				}
			}
			return *this;
		}
		// copy instance easy copy makes a copy of instance for work with from more than one function
		virtual BaseDynamicBuffer<T>& copy(BaseDynamicBuffer<T>& src) {
			if (_state != src._state) {
				clear();
				if (!src._state) {
					src.init();
				}
				_state = src._state;
				inc();
			}
			return *this;
		}
		// make a copy of some buffer
		virtual BaseDynamicBuffer<T>& copy(const T* src, size_t len) {
			if (src != NULL) {
				(_state ? clear() : init());
				_state[BaseDynBuffData] = (size_t)new T[len];
				_state[BaseDynBuffLen] = (len * sizeof(T));
				byte* target = (byte*)_state[BaseDynBuffData];
				byte* source = (byte*)src;
				for (size_t i = 0; i < _state[BaseDynBuffLen]; i++) {
					target[i] = source[i];
				}
			}
			return *this;
		}
		// make a copy from other type
		template<typename U>
		BaseDynamicBuffer<U> getOtherType() {
			return BaseDynamicBuffer<U>(*this);
		}
		/* take over a dynamic buffer(taked buffer must be allocated
		by using 'new T[]', cose it'l be freed by using 'delete[]') */
		virtual BaseDynamicBuffer<T>& takeover(const T* src, size_t len) {
			T* t = (!_state ? NULL : (T*)_state[BaseDynBuffData]);
			if (src != t) {
				(!_state ? init() : clear());
				_state[BaseDynBuffData] = (size_t)src;
				_state[BaseDynBuffLen] = (len * sizeof(T));
			}
			return *this;
		}
		virtual T* release() {
			T* temp = NULL;
			if (_state) {
				temp = (T*)_state[BaseDynBuffData];
				_state[BaseDynBuffData] = 0;
				_state[BaseDynBuffLen] = 0;
			}
			return temp;
		}
		/*	*/

		/* opeartors */
		// copy of instance
		virtual BaseDynamicBuffer<T>& operator=(BaseDynamicBuffer<T>& src) {
			return copy(src);
		}
		// boolean cast
		virtual operator bool() const {
			return (_state && _state[BaseDynBuffData]);
		}
		// compare by addres
		virtual const bool operator==(const BaseDynamicBuffer<T>& other) const {
			return (_state == other._state);
		}
		virtual const bool operator!=(const BaseDynamicBuffer<T>& other) const {
			return _state != other._state;
		}
		virtual T const operator[] (const size_t n) const {
			size_t i = (n * sizeof(T));
			if (!isEmpty() && n < _state[BaseDynBuffLen]) {
				return ((T*)_state[BaseDynBuffData])[i];
			}
			throw(ftwd::exception(globErrArrayOutOfRange, NULL, "const T operator[]", "BaseDynamicBuffer<T>"));
		}
		virtual T& operator[] (const size_t n) {
			size_t i = (n * sizeof(T));
			if (!isEmpty() && n < _state[BaseDynBuffLen]) {
				return ((T*)_state[BaseDynBuffData])[i];
			}
			throw(ftwd::exception(globErrArrayOutOfRange, NULL, "operator[]", "BaseDynamicBuffer<T>"));
		}
		/*	*/

	#ifdef _debug_test_
		
		#ifndef _testmacros_h_
			#include "testmacros.h"
		#endif // !_testmacros_h_
		
		/**************************************************************************
		This section contains debug tests.
		To compile your programm with it place '#define _debug_test_'
		before including this file and then call the test by calling
		'BaseDynamicBuffer<char>::test();' in your testing functions
		***************************************************************************
			TODO: Yuo can make your own test functions, this is just an example
		**************************************************************************/
		typedef char TestType;

		static bool DefaultConstructorTest() {
			_debugstream << ("BaseDynamicBuffer()");
			BaseDynamicBuffer<TestType> test;
			return (test._state == NULL);
		}
		static bool ParametredConstructorTest() {
			_debugstream << "BaseDynamicBuffer(const T*, const size_t)";
			const char tstring[] = "Buffer test";
			size_t tlen = sizeof(tstring);
			BaseDynamicBuffer<TestType> test2(tstring, tlen);
			size_t i;
			for (i = 0; i < tlen; i++) {
				if (test2[i] != tstring[i])
					break;
			}
			return (i == tlen);
		}
		static bool CopyConstructorTest() {
			_debugstream << "BaseDynamicBuffer<T>(const BaseDynamicBuffer<T>& other)";
			char string1[] = "CopyConstructorTest";
			BaseDynamicBuffer<TestType> test1(string1, sizeof(string1));
			BaseDynamicBuffer<TestType> test2(test1);
			char string2[] = "Owning test";
			char* t = test1.data();
			test1._state[BaseDynBuffData] = (size_t)string2;
			bool res = (test1._state[BaseDynBuffData] == test2._state[BaseDynBuffData] && test1._state[BaseDynBuffRCnt] == 2);
			test1._state[BaseDynBuffData] = (size_t)t;
			return res;
		}
		static bool EmptyCopyConstructorTest() {
			_debugstream << "(Empty)BaseDynamicBuffer<T>(BaseDynamicBuffer<T>& other)";
			BaseDynamicBuffer<TestType> tObj1;
			BaseDynamicBuffer<TestType> tObj2(tObj1);
			return(tObj1._state && tObj1._state == tObj2._state && tObj1._state[BaseDynBuffRCnt] == 2);
		}
		static bool ClearAndDestructorTest() {
			_debugstream << "clear()";
			const char tstring[] = "Buffer test";
			size_t tlen = sizeof(tstring);
			size_t
				InstOffset = 0,
				Obj1Offset = 0,
				Obj2Offset = sizeof(BaseDynamicBuffer<char>),
				DataOffset = (sizeof(BaseDynamicBuffer<char>) * 2)
				;
			size_t totalsize = DataOffset + tlen;
			/* Creating Objects in special allocated memory */
			// Getting memory
			byte* allocated = new byte[totalsize];
			size_t* objects = (size_t*)allocated;
			// Creating a string
			char* TestString = new char[tlen];
			// Creating two objects
			BaseDynamicBuffer<char>* tObj1 = new(allocated) BaseDynamicBuffer<char>();
			BaseDynamicBuffer<char>* tObj2 = new(&allocated[Obj2Offset]) BaseDynamicBuffer<char>();
			// Geting addresses of objects
			size_t* tObj1_addres = (size_t*)tObj1;
			size_t* tObj2_addres = (size_t*)tObj2;
			tObj1->takeover(TestString, tlen);
			// Make a copy of tObj1
			*tObj2 = *tObj1;
			size_t* State_addres = tObj1->_state;
			// Reffs test
			if (State_addres[BaseDynBuffRCnt] != 2) {
				return false;
			}
			// Clearing test
			tObj1->clear();
			if (State_addres[BaseDynBuffRCnt] != 2 || State_addres[BaseDynBuffLen] || State_addres[BaseDynBuffData]) {
				return false;
			}
			// destructor test
			tObj1->~BaseDynamicBuffer();
			if (tObj1[0] || State_addres[BaseDynBuffRCnt] != 1) {
				return false;
			}
			tObj2->~BaseDynamicBuffer();
			// final destructor test
			if (tObj1->_state || tObj2->_state) {
				return false;
			}
			return true;
		}
		static bool CopyTest() {
			_debugstream << "copy(const T*, const size_t)";
			char str[] = "CopyTest";
			BaseDynamicBuffer<char> tObj;
			size_t len = sizeof(str);
			tObj.copy(str, len);
			char* t = tObj.data();
			size_t i;
			for (i = 0; i < len; i++)
				if (t[i] != str[i])
					break;
			return (i == len);
		}
		static bool EmptyCopyTest() {
			_debugstream << "(Empty)copy(const T*, const size_t)";
			char str[] = "CopyTest";
			BaseDynamicBuffer<char> tObj1;
			BaseDynamicBuffer<char> tObj2(tObj1);
			size_t len = sizeof(str);
			tObj2.copy(str, len);
			char* t = tObj1.data();
			size_t i;
			for (i = 0; i < len; i++)
				if (t[i] != str[i])
					break;
			return (i == len);
		}
		static bool CloneTest() {
			_debugstream << "clone(const BaseDynamicBuffer<char>&)";
			char str1[] = "CloneTest1";
			char str2[] = "CloneTest2";
			size_t len1 = sizeof(str1);
			size_t len2 = sizeof(str2);
			BaseDynamicBuffer<char> tObj1(str1, len1), tObj2;
			// Make a deep copy of tObj1
			tObj2.clone(tObj1);
			// copy the str2 to tObj1
			tObj1.copy(str2, len2);
			// str1 in tObj2 && str2 in tObj1
			char* t1 = tObj2.data(), *t2 = tObj1.data();
			size_t i1, i2;
			for (i1 = 0; i1 < len1; i1++)
				if (t1[i1] != str1[i1])
					break;
			for (i2 = 0; i2 < len2; i2++)
				if (t2[i2] != str2[i2])
					break;
			return (i1 == len1 && i2 == len2);
		}
		static bool TakeOverTest() {
			bool res = false;
			_debugstream << "TakeOverTest(T* const _Source, size_t const _Size)";
			BaseDynamicBuffer<char> test;
			char tstring[] = "Hello, World!";
			size_t slen = sizeof(tstring);
			char* t = new char[slen + 1];
			memcopy<char, char const>(t, tstring, slen + 1);
			test.takeover(t, slen);
			BaseDynamicBuffer<char> test2(test);
			char* t1 = test2.release();
			delete[] t1;
			test.clear();
			test2.clear();
			return (!test.data() && !test2.data());
		}
		// This macro is used to say the preprocessor that the test is available
	#define BaseDynamicBufferTest
		static bool test() {
			_debugstream << "tpl::BaseDynamicBuffer<T>\n";
			try {
				try {
					result(DefaultConstructorTest());
					result(ParametredConstructorTest());
					result(CopyConstructorTest());
					result(EmptyCopyConstructorTest());
					result(ClearAndDestructorTest());
					result(CopyTest());
					result(EmptyCopyTest());
					result(CloneTest());
					result(TakeOverTest());
				}
				catch (errcodes code) {
					_debugstream << "...Fail!\tErrorCode: " << code << "\n";
				}
			}
			catch (std::exception e) {
				_debugstream << "std::exception [" << e.what() << "]\n\n";
				return false;
			}
			_debugstream << "\n";
			return true;
		}
	#endif // _debug_test_
		/*	End of class BaseDynamicBuffer	*/
	};


}
