#include "ArgsParseOption.hpp"

#include <algorithm>
#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		// Print options
		os << "--" << opt.name;
		if (opt.short_opt) os << ",-" << opt.short_opt;

		// Print the argument
		if (opt.nargs || opt.vargs) {
			if (!opt.nargs && opt.vargs) os << "["; // if optional
			std::string upper(opt.name);
			std::transform(upper.begin(), upper.end(), upper.begin(),
					[] (char c) { return std::toupper(c); });
			os << upper;
			if (opt.nargs + opt.vargs > 1) { // more than one arg
				if (opt.nargs && opt.vargs) os << "["; // if mixed
				os << "...";
			}
			if (opt.vargs) os << "]"; // if optional
		}
		// Print the description
		os.fill(' ');
		return os << std::setw(100) << " : " + opt.desc;
	}

} /* namespace argsparse */
