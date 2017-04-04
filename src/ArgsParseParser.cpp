#include "ArgsParseParser.hpp"

#include <iostream>

namespace argsparse {

	/* bool parseOption(Option& opt, const std::vector<std::string>& args) */
	/* { */
	/* 	auto it = args.cbegin() + 1; */
	/* 	while(opt.count++ < opt.nargs) { */
	/* 		if ((*it).front() == '-') { */
	/* 			std::cerr << "Parser:parse() " << opt.long_opt << " requires " */
	/* 				<< opt.nargs << " arguments." << std::endl; */
	/* 			return false; */
	/* 		} */
	/* 		opt.parse(*it); */
	/* 		++it; */
	/* 	} */
	/* 	return true; */
	/* } */

	bool parseOption(Option& opt, std::vector<std::string>::const_iterator& args, std::vector<std::string>::const_iterator end)
	{
		// Check if enough args
		if (args + opt.nargs >= end) {
			std::cerr << "Parser::parseOption() option " << opt.long_opt
				<< " requires " << opt.nargs << " arguments." << std::endl;
			return false;
		}

		// Reduce count if past variable args
		while (opt.count++ < opt.vargs) {
			if (args == end || args->front() == '-') break;
			opt.parse(*(args++));
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
		if (_name == "") _name = args.front();
		auto pos_it = _positional.begin();

		for (auto it = args.begin() + 1; it != args.end(); ++it) {
			std::string arg = *it;

			if (arg.substr(0, 2) == "--") { // Parse as long

				auto key = _options.find(arg.substr(2));
				if (key == _options.end()) {
					std::cerr << "Parser::parse() " << arg << " not found." << std::endl;
					return false;
				}

				/* if (it + key->second.nargs == args.end()) { */
				/* 	std::cerr << "Parser:parse() " << arg << " requires " */
				/* 		<< key->second.nargs << " arguments." << std::endl; */
				/* 	return false; */
				/* } */

				if (!parseOption(key->second, it, args.end())) return false;
				/* std::vector<std::string> opt_args(it, it + 1 + key->second.nargs); */
				/* if (!parseOption(key->second, opt_args)) return false; */
				/* it = it + key->second.nargs; */

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
						/* if (it + key->second.nargs >= args.end()) { */
						/* 	std::cerr << "Parser:parse() " << arg << " requires " */
						/* 		<< key->second.nargs << " arguments." << std::endl; */
						/* 	return false; */
						/* } */
						/* std::vector<std::string> opt_args(it, it + 1 + key->second.nargs); */
						if (!parseOption(key->second, it, args.end())) return false;
						/* it = it + key->second.nargs; */
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
		os << "Usage: " << p._name << " [options] ";
		for (auto p : p._positional) {
			os << p.first << " ";
		}
		os << "\n";
		os << "Options:\n";
		for (auto opt : p._options) {
			os << "\t" << opt.second << "\n";
		}
		return os;
	}

} /* namespace argsparse */
