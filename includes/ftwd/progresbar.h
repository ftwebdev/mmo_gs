#pragma once
#ifdef _WIN32
	#ifdef PROGRESSBARDLL_EXPORTS
		#define PROGRESSBARDLL_API __declspec(dllexport)
	#else
		#define PROGRESSBARDLL_API __declspec(dllimport)
	#endif //PROGRESSBARDLL_EXPORTS

	#include <ctime>
#else
	#define PROGRESSBARDLL_API
	#include <time.h>
#endif // _WIN32

#include "ftwd/global.h"

namespace ftwd {
	PROGRESSBARDLL_API struct BarCfg {
		conscolor MsgColor;
		conscolor EmptyColor;
		conscolor ProgresColor;
		size_t BlockCount;
		char HeadingScope;
		char TrailingScope;
		char EmptyFiller;
		char ProgresFiller;
		BarCfg();
		~BarCfg();
	};

	class progressbar {
		// conting offset (total units count)
		double total;
		// one percent size in units
		double PercentSize;
		double BlockSize;
		// curent unit
		double CurUnit;
		double NextPercent;
		double NextBlock;
		// message(will be displayed before progressbar)
		char* Msg;
		// char buffer(will be displayed)
		char* bar;
		// bar offsets
		struct _pboffsets {
			char* FirstBlock;
			char* LastBlock;
			char* ActiveBlock;
			char* Prcentage;
			char* TrailingScope;
		} offsets;
		BarCfg barCfg;
		byte percentage;
        progressbar(progressbar &other);
        void operator=(progressbar &other);
        void refresh();
		void init();
		void clear();
    public:
		PROGRESSBARDLL_API explicit progressbar(const size_t total, const char* const Msg = nullptr, const BarCfg* const _Cfg = nullptr);
		PROGRESSBARDLL_API ~progressbar();
		PROGRESSBARDLL_API void now(size_t i);
		PROGRESSBARDLL_API void reset(const size_t total, const char* const Msg = nullptr);
		PROGRESSBARDLL_API void clearout();
    };
};
