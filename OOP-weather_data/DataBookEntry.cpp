#include "DataBookEntry.h"
#include <unordered_map>

DataBookEntry::DataBookEntry(std::string _timestamp,
                            std::vector<double> _temperatures,
                            Country _country) 
: timestamp(_timestamp),
  temperatures(_temperatures),
  country(_country)
{

}

Country DataBookEntry::stringToCountry(std::string s)
{
// Static map to avoid rebuilding the map on each function call
    static const std::unordered_map<std::string, Country> countryMap = {
        {"AT", Country::AT}, {"BE", Country::BE}, {"BG", Country::BG},
        {"CH", Country::CH}, {"CZ", Country::CZ}, {"DE", Country::DE},
        {"DK", Country::DK}, {"EE", Country::EE}, {"ES", Country::ES},
        {"FI", Country::FI}, {"FR", Country::FR}, {"GB", Country::GB},
        {"GR", Country::GR}, {"HR", Country::HR}, {"HU", Country::HU},
        {"IE", Country::IE}, {"IT", Country::IT}, {"LT", Country::LT},
        {"LU", Country::LU}, {"LV", Country::LV}, {"NL", Country::NL},
        {"NO", Country::NO}, {"PL", Country::PL}, {"PT", Country::PT},
        {"RO", Country::RO}, {"SE", Country::SE}, {"SI", Country::SI},
        {"SK", Country::SK}
    };

    // Search in the map for the string
    auto it = countryMap.find(s);
    if (it != countryMap.end()) {
        return it->second; // Return the corresponding Country enum
    }

    return Country::UNKNOWN; // Return UNKNOWN if not found
}