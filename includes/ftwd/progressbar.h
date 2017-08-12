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

	PROGRESSBARDLL_API class progressbar {
		// conting offset (total units count)
		size_t total;
		// one percent size in units
		size_t PercentSize;
		size_t BlockSize;
		// curent unit
		size_t CurUnit;
		size_t NextPercent;
		size_t NextBlock;
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
		explicit progressbar(const size_t total, const char* const Msg = nullptr, const BarCfg* const _Cfg = nullptr);
        ~progressbar();
		void now(size_t i);
        void reset(const size_t total, const char* const Msg = nullptr);
        void clearout();
    };
};
