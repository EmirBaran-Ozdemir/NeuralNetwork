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
                result.push_back(std::stoi(item));  
            }

            return result;
        }

        static std::vector<double> ParseWStringToDoubleVector(const std::wstring& wstr, wchar_t delimiter = L',')
        {
            std::vector<double> result;
            std::wistringstream wstream(wstr);
            std::wstring item;

            while(std::getline(wstream, item, delimiter)) {
                result.push_back(std::stod(item)); 
            }

            return result;
        }
        static int ParseWStringToInt(const std::wstring& wstr)
        {
            int result = std::stod(wstr);
            return result;
        }

	};
}