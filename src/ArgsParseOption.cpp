#include "ArgsParseOption.hpp"

#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		if (opt.short_opt != 0) {
			os << "-" << opt.short_opt << ",";
		}
		os << "--" << opt.long_opt << " ";
		if (opt.nargs) {
			std::string upper(opt.long_opt);
			std::transform(upper.begin(), upper.end(), upper.begin(),
					[] (char c) { return std::toupper(c); });
			os << upper;
		}
		opt.nargs > 1 ? os << "... " : os << " ";
		return os << std::setw(30) << std::left << opt.desc;
	}

} /* namespace argsparse */
