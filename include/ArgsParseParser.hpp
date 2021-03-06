#ifndef _ARGS_PARSE_PARSER_H_
#define _ARGS_PARSE_PARSER_H_

/* args_parse Parser class:
 * 
 * Initialise with an optional program name Parser parser(<PROG_NAME>)
 * 
 * Add any options / args required using addOption and addPositional.
 * .addOption<TYPE>(<LONG_NAME>, <CHAR>, <HELP_DESC>,
 * .addPositional(<NAME>).
 * If not a simple switch (true/false) use:
 * .addOption<TYPE>(<LONG_NAME>, <CHAR>, <HELP_DESC>,
 *                  <DEFAULT_VAL>, <NUM_REQ_ARGS>)
 * 
 * Parse input using .parse(argc, argv).
 * 
 * Options can be accessed using [<NAME>] (returns true if used)
 * or .option<TYPE>(<NAME>) that will return the value casted as <TYPE>.
 * Positional accessed using .positional(<NAME>).
 * 
 * Passing the parser object to a stream will print generated help text
 * Additional text to printed can be added using .addText(<TEXT>).
 */

#include <map>
#include <string>
#include <vector>

#include "ArgsParseOption.hpp"

namespace argsparse
{

	class Parser
	{
		private:
			std::string _program_name;
			std::string _additional_text;
			std::map<std::string, Option> _options;
			std::vector<std::pair<const std::string, std::string>> _positional;

		public:
			Parser(const std::string program_name = "")
				: _program_name(program_name), _additional_text("") {}

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
					_options.insert({long_opt, Option(long_opt, short_opt,
								desc, default_val, required_args < 0 ? value::size(default_val) : required_args)});
				}
			template<typename T>
				const T& option(const std::string& name) {
					return this->operator[](name).as<T>();
				}

			void addPositional(const std::string& name);
			const std::string& positional(const std::string& name);

			void addText(const std::string& text) { _additional_text = text; }

			bool parse(const std::vector<std::string>& args);
			bool parse(int32_t argc, const char* argv[]);

			const Option& operator[](const std::string& long_opt);
			const Option& operator[](const char _short_opt);

			friend std::ostream& operator<<(std::ostream& os, const Parser& p);
	};
};

#endif /* ifndef _ARGS_PARSE_PARSER_H_ */
