#include "ArgsParseOption.hpp"

#include <algorithm>
#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		// Set options
		std::string opt_str("--" + opt.name);
		if (opt.short_opt) opt_str.append(",-").push_back(opt.short_opt);
		opt_str.append(" ");
		// Set the argument(s)
		if (opt.nargs || opt.vargs) {
			if (!opt.nargs && opt.vargs) opt_str.append("["); // if optional
			opt_str.append(opt.name.substr(0, 12));
			std::for_each(opt_str.end() - opt.name.size(), opt_str.end(),
					[] (char& c) { c = std::toupper(c); });
			if (opt.nargs + opt.vargs > 1) { // more than one arg
				if (opt.nargs && opt.vargs) opt_str.append("["); // if mixed
				opt_str.append("...");
			}
			if (opt.vargs) opt_str.append("]"); // if optional
		}
		// Print option and arguments
		os << std::left << std::setw(31) << opt_str;
		// Print the description
		return os << " : " + opt.desc;
	}

} /* namespace argsparse */
