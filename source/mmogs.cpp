//#include "ftwd/mmogs.h"
//#include "tpl/global.h"

#ifdef _WIN32
	#include <Windows.h>
	#include <io.h>
	#include <ctime>
#else
	#include <unistd.h>
	#include <time.h>
#endif //_WIN32
#include <random>

namespace ftwd {
	void setOutputColor(const unsigned char _Text) {
		
	}
};

int main(){
	char test[] = "Hello World!\n";
	const char ret[] = " \r";
	char spin[] = { "\\|/|" };
	size_t len = sizeof(test) - 1;
	size_t slen = sizeof(spin) - 1;
	size_t spos = 0;
	struct timespec ts, tw = { 0, 500 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	for (byte i = 0; i < 16; i++) {
		SetConsoleTextAttribute(handle, i);
		write(1, test, len);
	}
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
