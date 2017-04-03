#ifndef _ARGS_PARSE_OPTION_H_
#define _ARGS_PARSE_OPTION_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

namespace argsparse {
	struct exception : public std::exception
	{
		std::string msg;
		exception(const std::string& _msg) : msg(_msg) {}
		const char* what() const noexcept { return msg.c_str(); }
	};

	namespace value {
		template<typename T>
			void empty(T& value) { value = T(); }

		template<typename T>
			void empty(std::vector<T>& value) { value.clear(); }

		template<typename T>
			void parse(const std::string& text, T& value) {
				std::istringstream iss(text);
				if(!(iss >> value)) throw(argsparse::exception(
							"parse_value: failed to parse '" + text + "'"));
			}

		template<typename T>
			void parse(const std::string&, bool& value) {
				value = true;
			}

		template<typename T>
			void parse(const std::string& text, std::vector<T>& value) {
				T v;
				value::parse(text, v);
				value.push_back(v);
			}

		template<typename T>
			int32_t size(const T&) { return 1; }

		template<typename T>
			int32_t size(const std::vector<T>& value) {
				return value.size();
			}
	} /* namespace value */


	struct Value
	{
		virtual ~Value() {};
		virtual const std::type_info& type() const = 0;
		virtual void parse(const std::string& text) = 0;
		virtual void empty() = 0;
		virtual Value* copy() = 0;
	};

	template<typename T>
		struct ValueT : public Value
	{
		T value;
		ValueT(const T& t) : value(t) {}
		const std::type_info& type() const { return typeid(T); }
		void parse(const std::string& text) {
			value::parse(text, value); }
		void empty() { value::empty(value); }
		Value* copy() { return new ValueT<T>(value); }
	};

	class AnyValue
	{
		private:
			Value* _val;

		public:
			AnyValue(const AnyValue& av) : _val(av._val->copy()) {}
			template<typename T>
				AnyValue(const T& val) : _val(new ValueT<T>(val)) {}
			~AnyValue() { delete _val; }

			template<typename T>
				const T& as() const {
					T* ptr = (_val->type() == typeid(T))
						? &static_cast<ValueT<T>*>(_val)->value : nullptr;
					if (!ptr) throw(argsparse::exception(
								"AnyValue::as: pointer conversion failed"));
					return *ptr;
				}

			void parse(const std::string& text) const {
				_val->parse(text);
			}
			void empty() const { _val->empty(); }
	};

	struct Option
	{
		int32_t count;
		std::string long_opt;
		const char short_opt;
		const std::string desc;
		int32_t nargs;
		AnyValue value;

		template<typename T>
			Option(const std::string& _long_opt, const char _short_opt, const std::string& _desc, const T& default_val, const int32_t _nargs)
			: count(0), long_opt(_long_opt), short_opt(_short_opt), desc(_desc), nargs(_nargs), value(default_val) {
				if (nargs > 0 && value::size(default_val) != nargs) throw(
						argsparse::exception("Option(): " + long_opt + " default val differs from nargs."));
			}

		template<typename T>
			const T& as() const {
				return value.as<T>();
			}

		void parse(const std::string& text) {
			if (count == 0 && nargs > 1)
				value.empty();
			value.parse(text);
			count++;
		}

		friend std::ostream& operator<<(std::ostream& os, const Option& opt);
	};
}

#endif /* ifndef _ARGS_PARSE_OPTION_H_ */
