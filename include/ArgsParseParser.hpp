#ifndef _ARGS_PARSE_PARSER_H_
#define _ARGS_PARSE_PARSER_H_

#include <map>
#include <string>
#include <vector>

#include "ArgsParseOption.hpp"

namespace argsparse
{

	class Parser
	{
		private:
			std::string _name;
			std::map<std::string, Option> _options;
			std::vector<std::pair<const std::string, std::string>> _positional;

		public:
			Parser(const std::string name = "") : _name(name) {}

			void addOption(const std::string& long_opt, const char short_opt = 0,
					const std::string& desc = "")
			{
				if (_options.find(long_opt) != _options.end()) throw(
						argsparse::exception("Parser::AddOption: option already exists."));
				_options.insert({long_opt, Option(long_opt, short_opt, desc)});
			}
			template <typename T>
				void addOption(const std::string& long_opt, const char short_opt = 0,
						const std::string& desc = "", const T& default_val = 0, const int32_t required_args = -1)
				{
					if (_options.find(long_opt) != _options.end()) throw(
							argsparse::exception("Parser::AddOption: option already exists."));
					if (required_args < 0) required_args = value::size(default_val);
					_options.insert({long_opt, Option(long_opt, short_opt,
								desc, default_val, required_args)});
				}
			template<typename T>
				const T& option(const std::string& name) {
					return this->operator[](name).as<T>();
				}

			void addPositional(const std::string& name);
			const std::string& positional(const std::string& name);

			bool parse(const std::vector<std::string>& args);
			bool parse(int32_t argc, const char* argv[]);

			const Option& operator[](const std::string& long_opt);
			const Option& operator[](const char _short_opt);

			friend std::ostream& operator<<(std::ostream& os, const Parser& p);
	};
};

#endif /* ifndef _ARGS_PARSE_PARSER_H_ */
