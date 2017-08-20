#include "ftwd/mmogs.h"
#include <iostream>
#include <io.h>

int main(){
	using namespace ftwd;
	const char testMsg[] = { "Привет, Мир!\n" };
	byte* test = (byte*)dcopy(testMsg);
	size_t len = strlen(test);
	encoding::convert(EncCP1251, EncUTF8, test, len, false);
	FILE* file = fopen("enctest.txt", "wb");
	if (!file) {
		std::cout << "Can't open enctest.txt\n";
	}
	int fd = fileno(file);
	write(fd, test, len);
	fclose(file);
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
