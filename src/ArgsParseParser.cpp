#include "ArgsParseParser.hpp"

#include <iostream>

namespace argsparse {

	bool parseOption(Option& opt, std::vector<std::string>::const_iterator& arg_it, std::vector<std::string>::const_iterator end)
	{
		// Increment even if requires no args
		if (opt.nargs == 0) opt.nargs++;

		int32_t i = 0;
		while (i < opt.nargs) {
			if (arg_it == end || (*arg_it).front() == '-') {
				std::cerr << "Parser::ParseAsLong: " << opt.long_opt
					<< " requires " << opt.nargs << " arguments."
					<< std::endl;
				return false;
			}
			opt.parse(*(arg_it++));
			++i;
		}
		return true;
	}

	void Parser::addPositional(const std::string& name)
	{
		_positional.push_back({name, ""});
	}

	bool Parser::parse(const std::vector<std::string>& args)
	{
		if (_name == "") _name = args.front();
		auto pos_it = _positional.begin();

		for (auto it = args.begin() + 1; it != args.end(); ++it) {
			std::string arg = *it;

			if (arg.substr(0, 2) == "--") { // Parse as long

				auto key = _options.find(arg.substr(2));
				if (key == _options.end()) {
					std::cerr << "Parser::Parse: " << arg << "not found." << std::endl;
					return false;
				}
				if (!parseOption(key->second, ++it, args.end())) return false;

			} else if (arg.front() == '-') { // Parse as shorts

				for (auto c_it = arg.begin() + 1; c_it != arg.end(); ++c_it) {

					/* const std::map<std::string, Option>::iterator key = std::find_if( */
					/* 		_options.begin(), _options.end(), [c_it] */
					/* 		(std::pair<std::string, Option>& opt) */
					/* 		{	return (opt.second.short_opt == *c_it); }); */
					auto key = _options.end();
					if (key == _options.end()) {
						std::cerr << "Parser::Parse: -" << *c_it << "not found." << std::endl;
						return false;
					}
					if (key->second.nargs && c_it != arg.end() - 1) {
						std::cerr << "Parser::Parse: -" << *c_it << " requires "
							<< key->second.nargs << " arguments." << std::endl;
						return false;
					}
					if (!parseOption(key->second, ++it, args.end())) return false;
				}

			} else { // Parse as positional arg
				if (pos_it == _positional.end()) {
					std::cerr << "Parser::Parse: too many positional arguments!" << std::endl;
					return false;
				}
				(*pos_it).second = *it;
				++pos_it;
			}
		}

		if (pos_it != _positional.end()) {
			std::cerr << "Parser::Parse: Missing positional arguments: " << std::endl;
			while (pos_it++ != _positional.end()) {
				std::cerr << (*pos_it).first;
			}
			std::cerr << std::endl;
			return false;
		}

		return true;
	}

	bool Parser::parse(int32_t argc, const char* argv[])
	{
		return this->parse(std::vector<std::string>(argv, argv + argc));
	}

	const Option& Parser::operator[](const std::string& long_opt)
	{
		auto key = _options.find(long_opt);
		if (key == _options.end()) throw(
				argsparse::exception("Parser::operator[]: " + long_opt + " not found."));
		return key->second;
	}

	const Option& Parser::operator[](const char _short_opt)
	{
		const std::map<std::string, Option>::const_iterator key = std::find_if(
				_options.begin(), _options.end(), [_short_opt]
				(const std::pair<std::string, Option>& opt)
				{	return (opt.second.short_opt == _short_opt); });
		if (key == _options.end()) throw(
				argsparse::exception("Parser::operator[]: " + std::string(&_short_opt) + " not found."));
		return key->second;
	}

	std::ostream& operator<<(std::ostream& os, const Parser& p)
	{
		os << "Usage: " << p._name << " [options] ";
		for (auto p : p._positional) {
			os << p.first << " ";
		}
		os << "\n";
		os << "Options:\n";
		for (auto opt : p._options) {
			os << "\t\t" << opt.second << "\n";
		}
		return os << std::endl;
	}

} /* namespace argsparse */
