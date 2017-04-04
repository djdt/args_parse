#include "ArgsParseOption.hpp"

#include <algorithm>
#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		if (opt.short_opt != 0) {
			os << "-" << opt.short_opt << ",";
		}

		os << "--" << opt.long_opt << " ";

		if (opt.vargs) {
			if (opt.vargs > opt.nargs) os << "[";
			std::string upper(opt.long_opt);
			std::transform(upper.begin(), upper.end(), upper.begin(),
					[] (char c) { return std::toupper(c); });
			os << upper;
		}
		if (opt.vargs > 1) {
			os << "...";
		}
		if (opt.vargs > opt.nargs) os << "]";
		return os << " : " << opt.desc;
	}

} /* namespace argsparse */
