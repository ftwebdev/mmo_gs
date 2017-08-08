#define CONFIGLOADERDLL_EXPORTS
#include "ftwd/configloader.h"

namespace ftwd {
	std::map<std::string, std::string> getConfig(const std::string _CfgName) {
		std::map<std::string, std::string> _Result;
		try {
			byte* _buffer;
			size_t _buffSize = fload(_buffer, _CfgName.c_str());
			stripComments(_buffer, _buffSize);
		}
		catch (exception &e) {
			e >> std::cerr;
		}
		return _Result;
	}
}