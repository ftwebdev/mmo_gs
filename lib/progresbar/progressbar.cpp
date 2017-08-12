#define PROGRESSBARDLL_EXPORTS

#include "ftwd/progressbar.h"
#ifdef _WIN32
	#include "io.h"
#else
	#include <unistd>
#endif // _WIN32

namespace ftwd {
	/*
	struct BarCfg {
	const char* Msg = nullptr;
	conscolor MsgColor = C_White;
	conscolor EmtyColor = C_White;
	conscolor ProgresColor = C_Green;
	char HeadingScope = '[';
	char TrailingScope = ']';
	char EmtyFiller = '.';
	char ProgresFiller = '#';
	} prbarDefCfg;
	*/
	
	BarCfg::BarCfg()
		: MsgColor(C_White)
		, EmptyColor(C_White)
		, ProgresColor(C_Green)
		, BlockCount(30)
		, HeadingScope('[')
		, TrailingScope(']')
		, EmptyFiller('.')
		, ProgresFiller('#')
	{}
	BarCfg::~BarCfg() {}

	progressbar::progressbar(const size_t total, const char* const Msg, const BarCfg* const _Cfg)
		: total(total)
		, PercentSize(0)
		, BlockSize(0)
		, CurUnit(0)
		, NextPercent(0)
		, NextBlock(0)
		, Msg(dcopy(Msg))
		, bar(nullptr)
		, offsets({ nullptr, nullptr, nullptr, nullptr, nullptr })
		, barCfg(_Cfg ? *_Cfg : BarCfg())
		, percentage(0)
	{
		init();
	}
	progressbar::~progressbar() {
		clear();
	}
	void progressbar::clear() {
		if (Msg) {
			delete[] Msg;
		}
		if (bar) {
			delete[] bar;
		}
	}
	void progressbar::init() {
		PercentSize = total / 100;
		if (bar)
			delete[] bar;
		if (barCfg.BlockCount < 3)
			barCfg.BlockCount = 3;
		else {
			barCfg.BlockCount = ((barCfg.BlockCount << 1) >> 1) + 1;
		}
		BlockSize = total / barCfg.BlockCount;
		bar = new char[barCfg.BlockCount + 4];
		bar[0] = barCfg.HeadingScope;
		offsets.TrailingScope = bar + barCfg.BlockCount + 2;
		offsets.TrailingScope[0] = barCfg.TrailingScope;
		offsets.TrailingScope[1] = '\r';
		offsets.TrailingScope[2] = 0;
		bar[barCfg.BlockCount + 1] = barCfg.TrailingScope;
		offsets.FirstBlock = offsets.ActiveBlock = bar + 1;
		offsets.LastBlock = offsets.TrailingScope - 1;
		offsets.Prcentage = bar + ((barCfg.BlockCount >> 1) + 1);
		
		CurUnit = percentage = 0;
		NextPercent = total / 100;
		NextBlock = total / barCfg.BlockCount;
	}
	void progressbar::refresh() {
		if (CurUnit == NextPercent) {
			percentage = (percentage < 100 ? (++percentage) : 100);
			NextPercent += PercentSize;
		}
		if (CurUnit == NextBlock) {
			if (offsets.ActiveBlock < offsets.LastBlock) {
				offsets.ActiveBlock += 1;
			}
			NextBlock += BlockSize;
		}
		// print Msg
		if (Msg) {
			setConsTextColor(barCfg.MsgColor);
			write(1, Msg, strlen(Msg));
			write(1, " ", 1);
		}
		for (char* temp = (bar + 1); temp < offsets.TrailingScope; temp++) {
			(*temp) = (temp < offsets.ActiveBlock ? barCfg.ProgresFiller : barCfg.EmptyFiller);
		}
		// Set Bar color
		setConsTextColor(barCfg.EmptyColor);
		// write firs scope
		write(1, &barCfg.HeadingScope, 1);
		offsets.Prcentage[0] = ('0' + (percentage / 10));
		offsets.Prcentage[1] = ('0' + (percentage % 10));
		offsets.Prcentage[2] = '%';
		if (offsets.ActiveBlock > offsets.FirstBlock) {
			setConsTextColor(barCfg.ProgresColor);
			write(1, bar + 1, offsets.ActiveBlock - offsets.FirstBlock);
			setConsTextColor(barCfg.EmptyColor);
		}
		write(1, offsets.ActiveBlock, strlen(offsets.ActiveBlock));
	}
	void progressbar::now(const size_t i) {
		CurUnit = i;
		if (i < NextPercent && i < NextBlock);
		else {
			refresh();
		}
	}
	void progressbar::reset(const size_t total, const char* const Msg) {
		if (this->Msg)
			delete[] this->Msg;
		this->Msg = (Msg ? dcopy(Msg) : nullptr);
		this->total = total;
		PercentSize = total / 100;
		CurUnit = percentage = 0;
		init();
	}
	void progressbar::clearout() {
		char* _temp = new char[513];
		_temp[512] = 0;
		for (size_t i = 0; i < 512; i++)
			_temp[i] = ' ';
		std::cout << _temp << '\r';
		delete[] _temp;
	}
};
