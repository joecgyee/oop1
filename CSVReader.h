#pragma once

#include "DataBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
    public:
        CSVReader();

        static std::vector<DataBookEntry> readCSV(const std::string& csvFile);
        static std::vector<std::string> tokenise(const std::string& csvLine, char separator);

    private:
        static std::vector<DataBookEntry> stringsToDBE(const std::vector<std::string>& tokens);
};
