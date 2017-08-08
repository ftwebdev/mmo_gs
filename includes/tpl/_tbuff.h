#pragma once
#include "ftwd/exception.h"

namespace ftwd {
	template<typename T>
	class _tbuff {
		T* _buff;
		size_t _bytes;
		void __zalloc(size_t const _Elements) {
			if (_Elements) {
				_bytes = sizeof(T) * _Elements;
				byte* temp = new byte[_bytes];
				for (size_t i = 0; i < _bytes; temp[i++] = 0);
				_buff = (T*)temp;
			}
		}
		void __copy(void const * const _Array, size_t const _Bytes) {
			if (_Array) {
				byte* src = (byte*)_Array;
				for (size_t i = 0; i < _bytes; i++) _buff[i] = src[i];
			}
		}
	public:
		_tbuff()
			: _buff(NULL)
			, _bytes(0)
		{}
		explicit _tbuff(T const * const _Src, size_t const _Size)
			: _buff(NULL)
			, _bytes(sizeof(T) * _Size)
		{
			__copy(_Src, _bytes);
		}
		explicit _tbuff(size_t const _Size) {
			__zalloc(_Size);
		}
		_tbuff(_tbuff<T> const &src)
			: _buff(NULL)
			, _bytes(src._bytes)
		{
			__copy(src._buff, _bytes);
		}
		template<typename U>
		_tbuff(_tbuff<U> const &_Src)
			: _buff(NULL)
			, _bytes(_Src._bytes)
		{
			__copy(_Src._buff, _bytes);
		}
		~_tbuff() {
			if (_buff)
				delete[]_buff;
		}
		_tbuff<T>& takeover(T* _Array, size_t const _Size) {
			if ((size_t)_Array < (size_t)_buff && (size_t)_Array > ((size_t)_buff + _bytes)) {
				if(_buff)
					delete[] _buff;
				_bytes = _Size * sizeof(T);
				_buff = _Array;
			}
			return *this;
		}
		T* release() {
			T* _temp = _buff;
			_buff = NULL;
			_bytes = 0;
			return _temp;
		}
		_tbuff& operator=(_tbuff<T> const &src) {
			if (this != &src) {
				_bytes = src._bytes;
				__copy(src._buff, _bytes);
			}
		}
		operator T const * const() const {
			return _buff;
		}
		T& operator[](size_t const n) {
			if (n < (_bytes / sizeof(T))) {
				return _buff[n];
			}
			else {
				throw(exception(globErrArrayOutOfRange, nullptr, "operator[]", "_tbuff"));
			}
		}
		T const operator[](size_t const n) const {
			if (n < (_bytes / sizeof(T))) {
				return _buff[n];
			}
		}
	};
};
