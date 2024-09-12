#include <sstream>
#include <vector>
#include <string>

namespace Utils {
	class ValueParser {
    public:
        static std::vector<int> ParseWStringToIntVector(const std::wstring& wstr, wchar_t delimiter = L',')
        {
            std::vector<int> result;
            std::wistringstream wstream(wstr);
            std::wstring item;

            while(std::getline(wstream, item, delimiter)) {
                result.push_back(std::stoi(item));  // Convert each split part to int
            }

            return result;
        }

        // Utility to split wstring by a delimiter and convert to a vector of doubles
        static std::vector<double> ParseWStringToDoubleVector(const std::wstring& wstr, wchar_t delimiter = L',')
        {
            std::vector<double> result;
            std::wistringstream wstream(wstr);
            std::wstring item;

            while(std::getline(wstream, item, delimiter)) {
                result.push_back(std::stod(item));  // Convert each split part to double
            }

            return result;
        }

	};
}