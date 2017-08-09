#pragma once
namespace ftwd {
	namespace _prbar {
		const char* const colors = {
			"\033[30m",
		};
	};
	void setOutputColor(progressbar::conscolor _TColor) {
		write(1, _prbar::colors[_TColor]);
	}
};