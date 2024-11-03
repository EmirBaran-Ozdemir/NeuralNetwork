#include <sstream>
#include <vector>
#include <string>
#include <Core/Utils/ExceptionHandler.h>

namespace Utils {
	class ValueParser {
	public:
		static std::vector<int> ConvertWStringToIntVector(const std::wstring& wstr, const std::wstring& targetValue, wchar_t delimiter = L',')
		{
			if (wstr.empty())
			{
				THROW_NULL_VALUE_ERROR(ConvertWStringToString(targetValue));
			}
			std::vector<int> result;
			try {

				std::wistringstream wStream(wstr);
				std::wstring item;

				while (std::getline(wStream, item, delimiter)) {
					result.push_back(std::stoi(item));
				}
			}
			catch (std::exception exception)
			{
				THROW_PARSING_ERROR(ConvertWStringToString(targetValue));
			}

			return result;
		}

		static std::vector<double> ConvertWStringToDoubleVector(const std::wstring& wstr, const std::wstring& targetValue, wchar_t delimiter = L',')
		{
			if (wstr.empty())
			{
				THROW_NULL_VALUE_ERROR(ConvertWStringToString(targetValue));
			}
			std::vector<double> result;
			try {

				std::wistringstream wStream(wstr);
				std::wstring item;

				while (std::getline(wStream, item, delimiter)) {
					result.push_back(std::stod(item));
				}
			}
			catch (std::exception exception)
			{
				THROW_PARSING_ERROR(ConvertWStringToString(targetValue));
			}
			return result;
		}

		static int ConvertWStringToInt(const std::wstring& wstr, const std::wstring& targetValue)
		{
			if (wstr.empty())
			{
				THROW_NULL_VALUE_ERROR(ConvertWStringToString(targetValue));
			}
			int result;
			try
			{
				result = std::stod(wstr);
			}
			catch (std::exception exception)
			{
				THROW_PARSING_ERROR(ConvertWStringToString(targetValue));
			}
			return result;
		}

		static std::wstring ConvertStringToWstring(const std::string& str)
		{
			using convert_typeX = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_typeX, wchar_t> converterX;

			return converterX.from_bytes(str);
		}

		static std::string ConvertWStringToString(const std::wstring& wstr)
		{
			using convert_typeX = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_typeX, wchar_t> converterX;

			return converterX.to_bytes(wstr);
		}
	};
}