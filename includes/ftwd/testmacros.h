#pragma once

#include <iostream>

#ifndef _testmacros_h_

	#define _testmacros_h_
	#define _debugstream std::cerr

	#define FuncDebugMsg(Message) do { _debugstream << Message; } while (false);
	#define FuncDebugSuccess do {  } while (false);
	#define FuncDebugError(Reason) do { _debugstream << "...Fail!\t" << Reason << "\n"; } while (false);

static bool debugError(const char* msg = NULL) {
	_debugstream << "...Fail!";
	if (msg) {
	_debugstream << "\t" << msg;
	return 0;
}
	_debugstream << "\n";
		}
static bool debugSucces() {
	_debugstream << "...Ok!\n";
	return 0;
}
static bool result(const bool res) {
	return (res ? debugSucces() : debugError());
}

	#ifndef _debug_test_
		#define _debug_test_
	#endif // _debug_test_

#endif // _testmacros_h_