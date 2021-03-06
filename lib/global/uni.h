#pragma once

namespace ftwd {
	/* sets console text color */
	namespace _prbar {
		const char* const colors[] = {
		    /* black */ "\033[0;30m",
		    /* dBlue */ "\033[0;34m",
		    /* dGreen */ "\033[0;32m",
		    /* dCayene */ "\033[0;36m",
		    /* dRed */ "\033[0;31m",
		    /* dPurple */ "\033[0;35m",
		    /* Brown */ "\033[0;33m",
		    /* Gray */ "\033[0;37m",
		    /* dGray */ "\033[1;30m",
		    /* Blue */ "\033[1;34m",
		    /* Green */ "\033[1;32m",
		    /* Cayene */ "\033[1;36m",
		    /* Red */ "\033[1;31m",
		    /* Purple */ "\033[1;35m",
		    /* Yellow */ "\033[1;33m",
		    /* White */ "\033[1;37m"
		};
	};
	void setConsTextColor(conscolor _TColor) {
		write(1, _prbar::colors[_TColor % 0x10], 7);
	}
};
