#pragma once
#ifdef _WIN32
	#ifdef CONFIGLOADERDLL_EXPORTS
		#define CONFIGLOADERDLL_API __declspec(dllexport)
	#elif _WIN32
		#define CONFIGLOADERDLL_API __declspec(dllimport)
	#endif
#else
	#define CONFIGLOADERDLL_API
#endif

#include <string>
#include <map>
#include "ftwd/global.h"
#include "tpl/global.h"

namespace ftwd {
	CONFIGLOADERDLL_API std::map<std::string, std::string> getConfig(const std::string _CfgName);
}
