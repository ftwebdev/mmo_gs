//#include "ftwd/mmogs.h"
//#include "tpl/global.h"
#include <unistd.h>
#include <time.h>
//#include <Windows.h>
#include <random>

namespace ftwd {
	void setOutputColor(const unsigned char _Text) {
		
	}
};

int main(){
	char test[] = "Hello World! ";
	const char ret[] = " \r";
	char spin[] = { "\\|/|" };
	size_t len = sizeof(test) - 1;
	size_t slen = sizeof(spin) - 1;
	size_t spos = 0;
	struct timespec ts, tw = { 0, 500 };
	while (true) {
		if (spos == slen)
			spos = 0;
		write(1, ret + 1, 1);
		write(1, spin + spos, 1);
		spos++;
		write(1, ret, 1);
		write(1, test, len);
		nanosleep(&tw, &ts);
	}
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
