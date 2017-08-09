#include "ftwd/mmogs.h"
#include <iostream>

int main(){
	using namespace ftwd;
	const char testMsg[] = { "Hello World!\n" };
	int esc = 1;
	for (size_t i = 0; i < 16; i++) {
		setConsTextColor((conscolor)i);
		std::cout << testMsg;
	}
	setConsTextColor(C_dCayene);
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
