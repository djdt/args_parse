#include "ArgsParseOption.hpp"

#include <algorithm>
#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		int32_t w = 0;
		if (opt.short_opt != 0) {
			os << "-" << opt.short_opt << ",";
			w += 3;
		}

		os << "--" << opt.long_opt << " ";
		w += (3 + opt.long_opt.length());

		if (opt.nargs) {
			std::string upper(opt.long_opt);
			std::transform(upper.begin(), upper.end(), upper.begin(),
					[] (char c) { return std::toupper(c); });
			os << upper;
			w += upper.length();
		}
		if (opt.nargs > 1) {
			os << "...";
			w += 3;
		}
		return os << " " << std::setw(29 - w) << opt.desc;
	}

} /* namespace argsparse */
