#include "ArgsParseOption.hpp"

#include <iomanip>

namespace argsparse {

	std::ostream& operator<<(std::ostream& os, const Option& opt)
	{
		size_t init_w = os.width();
		if (opt.short_opt != 0) {
			os << "-" << opt.short_opt << ",";
		}
		os << "--" << opt.long_opt << " ";
		if (opt.nargs) {
			std::string upper(opt.long_opt);
			std::transform(upper.begin(), upper.end(), upper.begin(),
					[] (char c) { return std::toupper(c); });
			opt.nargs > 1 ? os << upper << "..." : os << upper;
		}
		return os << std::setw(30 - (os.width() - init_w)) << opt.desc;
	}

} /* namespace argsparse */
