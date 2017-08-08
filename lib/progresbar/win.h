#pragma once
#include <Windows.h>

namespace ftwd {
	void setOutputColor(const unsigned char _Text) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		unsigned char txt = (_Text < 16 ? _Text : 15);
		SetConsoleTextAttribute(handle, txt);
	}
};
