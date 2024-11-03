#pragma once
#include "Exceptions.h"
#include <string>
#include <stdexcept>

namespace Utils {

	class ExceptionHandler {
	public:

		static void ThrowException(const char* className, const std::string& target)
		{
			throw Exceptions::RuntimeException(fmt::format("{}: {}", className, target));
		}

		static void ThrowNullValueException(const std::string& target)
		{
			throw Exceptions::NullValueException(target);
		}

		static void ThrowParsingException(const std::string& target)
		{
			throw Exceptions::ParsingException(target);
		}

		static std::string ClassName(const std::string& functionSignature)
		{
			std::size_t endPos = functionSignature.find("(");

			if (endPos != std::string::npos)
			{
				std::size_t startPos = functionSignature.rfind("::", endPos);
				endPos = startPos;
				if (startPos != std::string::npos)
				{
					startPos = functionSignature.rfind("::", startPos - 1);
					if (startPos != std::string::npos)
					{
						std::string className = functionSignature.substr(startPos + 2, endPos - startPos - 2);
						return className;
					}
				}
			}
			return functionSignature;
		}
	};
}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define NN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define NN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define NN_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define NN_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define NN_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define NN_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define NN_FUNC_SIG __func__
#else
#define NN_FUNC_SIG "WARNING::UNKNOWN::NN_FUNC_SIG"
#endif

#define THROW_ERROR(target) Utils::ExceptionHandler::ThrowException(Utils::ExceptionHandler::ClassName(NN_FUNC_SIG).c_str(), target)
#define THROW_NULL_VALUE_ERROR(target) Utils::ExceptionHandler::ThrowNullValueException(target)
#define THROW_PARSING_ERROR(target) Utils::ExceptionHandler::ThrowParsingException(target)
