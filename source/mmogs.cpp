#include "ftwd/mmogs.h"
#include "ftwd/progressbar.h"
#include <iostream>

int main(){
	using namespace ftwd;
	const char testMsg[] = { "Hello World!\n" };
	size_t size = 100;
	progressbar bar(size, testMsg);
	for (size_t i = 0; i < size; i++) {
		sleep(1);
		bar.now(i);
	}
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
