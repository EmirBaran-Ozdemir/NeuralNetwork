#pragma once
#include <stdexcept>
#include <string>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Utils::Exceptions {

	static void inline PrintError(const std::string& exception)
	{
		std::cout<<exception<<std::endl;
	}

	class NullValueException : public std::runtime_error {
	public:
		explicit NullValueException(const std::string& exception)
			: std::runtime_error(exception + " cannot be null.")
		{
			Utils::Exceptions::PrintError(exception + " cannot be null.");
		}
	};

	class ParsingException : public std::runtime_error {
	public:
		explicit ParsingException(const std::string& exception)
			: std::runtime_error(exception + " failed to parse.")
		{
			Utils::Exceptions::PrintError(exception + " failed to parse.");
		}
	};

	class RuntimeException : public std::runtime_error {
	public:
		explicit RuntimeException(const std::string& exception)
			: std::runtime_error("Runtime Error occurred " + exception)
		{
			Utils::Exceptions::PrintError("Runtime Error occurred " + exception);
		}
	};
}