#pragma once

#include <string>
#include <vector>

enum class Country {
    AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI,
    FR, GB, GR, HR, HU, IE, IT, LT, LU, LV,
    NL, NO, PL, PT, RO, SE, SI, SK, UNKNOWN
};

class DataBookEntry
{
    public:
        DataBookEntry(  std::string _timestamp,
                        std::vector<double> _temperatures,
                        Country _country);

        static Country stringToCountry(std::string s);
        
        std::string timestamp;
        std::vector<double> temperatures;
        Country country;
};
