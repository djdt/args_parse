#include "ArgsParseParser.hpp"

#include <algorithm>
#include <iostream>

namespace argsparse {

	bool parseOption(Option& opt, std::vector<std::string>::const_iterator& args, std::vector<std::string>::const_iterator end)
	{
		// Check if enough args
		if (args + opt.nargs >= end) {
			std::cerr << "Parser::parseOption() option " << opt.name
				<< " requires " << opt.nargs << " arguments." << std::endl;
			return false;
		}
		// Parse upto max allowed args
		while (opt.count++ < (opt.nargs + opt.vargs)) {
			if (args + 1 == end) break; // Can we increment
			args++;
			if (args->front() == '-') break; // Is this an option
			opt.parse(*args);
		}
		return true;
	}

	void Parser::addPositional(const std::string& name)
	{
		_positional.push_back({name, ""});
	}

	const std::string& Parser::positional(const std::string& name)
	{
		auto it = std::find_if(_positional.begin(), _positional.end(),
				[&name] (auto positional) { return positional.first == name; });
		if (it == _positional.end()) throw(argsparse::exception(
					"Parser::positional(): " + name + " not found."));

		return it->second;
	}

	bool Parser::parse(const std::vector<std::string>& args)
	{
		if (_program_name == "") _program_name = args.front();
		auto pos_it = _positional.begin();

		for (auto it = args.begin() + 1; it != args.end(); ++it) {
			std::string arg = *it;

			if (arg.substr(0, 2) == "--") { // Parse as long

				auto key = _options.find(arg.substr(2));
				if (key == _options.end()) {
					std::cerr << "Parser::parse() " << arg << " not found." << std::endl;
					return false;
				}

				if (!parseOption(key->second, it, args.end()))
					return false;

			} else if (arg.front() == '-') { // Parse as shorts

				for (auto c_it = arg.begin() + 1; c_it != arg.end(); ++c_it) {

					const auto key = std::find_if(_options.begin(), _options.end(),
							[&c_it] (auto& opt) {	return (opt.second.short_opt == *c_it); });
					if (key == _options.end()) {
						std::cerr << "Parser::parse() -" << *c_it << " not found." << std::endl;
						return false;
					}
					if (c_it != arg.end() - 1) {
						if (key->second.nargs) {
							std::cerr << "Parser::parse() -" << *c_it << " requires "
								<< key->second.nargs << " arguments." << std::endl;
							return false;
						}
						key->second.count++;
					} else { // Parse the last short option

						if (!parseOption(key->second, it, args.end()))
							return false;
					}
				}

			} else { // Parse as positional arg
				if (pos_it == _positional.end()) {
					std::cerr << "Parser::parse() too many positional arguments!" << std::endl;
					return false;
				}
				(*pos_it).second = *it;
				++pos_it;
			}
		}

		if (pos_it != _positional.end()) {
			std::cerr << "Parser::parse() Missing positional arguments: ";
			for (auto p : _positional) {
				if (p.second.empty()) {
					std::cerr << p.first << " ";
				}
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
		const auto key = std::find_if(_options.begin(), _options.end(),
				[&_short_opt] (const auto& opt) {	return (opt.second.short_opt == _short_opt); });
		if (key == _options.end()) throw(
				argsparse::exception("Parser::operator[]: " + std::string(&_short_opt) + " not found."));
		return key->second;
	}

	std::ostream& operator<<(std::ostream& os, const Parser& p)
	{
		os << "Usage: " << p._program_name << " [options] ";
		for (auto p : p._positional) {
			os << p.first << " ";
		}
		os << "\n";
		os << "Options:\n";
		for (auto opt : p._options) {
			os << "\t" << opt.second << "\n";
		}
		return os << p._additional_text;
	}

} /* namespace argsparse */
