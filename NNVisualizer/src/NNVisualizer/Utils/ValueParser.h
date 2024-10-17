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

        static std::wstring ConvertVectorToWString(const std::vector<double>& vec) {
            std::wstringstream wss;
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i != 0) wss << L", ";  // Add separator between values
                wss << std::to_wstring(vec[i]);
            }
            return wss.str();
        }
	};
}