//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <string>
#include <type_traits>

namespace System::Support::Utility {

	template<typename T, typename = void>
	struct Formatter;

	template<>
	struct Formatter<std::string> {
		static std::string toString(const std::string& string) {
			return string;
		}

		static std::string toString(std::string&& string) {
			return std::forward<decltype(string)>(string);
		}

	};

	template<>
	struct Formatter<const char*> {
		static std::string toString(const char* string) {
			return string;
		}

	};

	template<size_t S>
	struct Formatter<char[S]> {
		static std::string toString(const char string[S]) {
			return string;
		}

	};

	template<typename T>
	struct Formatter<T, typename std::enable_if<std::is_integral<T>::value>::type> {
		static std::string toString(T i) {
			return std::to_string(i);
		}

	};

}



