#pragma once
#ifdef _WIN32
	#ifdef PROGRESSBARDLL_EXPORTS
		#define PROGRESSBARDLL_API __declspec(dllexport)
	#else
		#define PROGRESSBARDLL_API __declspec(dllimport)
	#endif //PROGRESSBARDLL_EXPORTS
#else
	#define PROGRESSBARDLL_API
#endif // _WIN32

#include <iostream>

namespace ftwd {
	PROGRESSBARDLL_API class progressbar {
		typedef unsigned char byte;
		// char buffer(will be displayed)
		char* bar;
		// conting offset (total units count)
		size_t total;
		// stepsize in units
		size_t StepSize;
		// current unit
		size_t now;
		// next step in units
		size_t NextSstep;
		//chars used to display progress
		byte blocks;
		// total buffer size
		byte BarSize;
		
        progressbar(progressbar &other);
        void operator=(progressbar &other);
        void refresh() const;
        void init();
    public:
		explicit progressbar(size_t total, size_t blocks, byte progrescolor = 15);
        ~progressbar();
        void now(size_t i);
        void reset(size_t total);
        void clearout();
    };
};