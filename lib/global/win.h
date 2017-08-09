#pragma once

namespace ftwd {
	/* sets console text color */
	void setConsTextColor(conscolor _TColor) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, _TColor % 16);
	}
};
