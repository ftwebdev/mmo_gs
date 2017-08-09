#pragma once

namespace ftwd {
	void setOutputColor(progressbar::conscolor _TextColor) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		unsigned char txt = (_TextColor < 16 ? _TextColor : 15);
		SetConsoleTextAttribute(handle, txt);
	}
};
