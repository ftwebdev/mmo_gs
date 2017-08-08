#include "ftwd/mmogs.h"
#include "tpl/global.h"
#include <io.h>
#include <Windows.h>
#include <random>

namespace ftwd {
	void setOutputColor(const unsigned char _Text) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		unsigned char txt = (_Text < 16 ? _Text : 15);
		SetConsoleTextAttribute(handle, txt);
	}
};

int main(){
	char test[] = "Hello World! ";
	const char ret[] = " \r";
	char spin[] = { "\\|/-\\|/-" };
	size_t len = sizeof(test) - 1;
	size_t slen = sizeof(spin - 1);
	size_t spos = 0;
	while (true) {
		if (spos == slen)
			spos = 0;
		write(1, ret + 1, 1);
		write(1, spin + spos, 1);
		spos++;
		write(1, ret, 1);
		write(1, test, len);
		Sleep(200);
	}
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
