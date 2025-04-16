#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>

CSVReader::CSVReader()
{
}

std::vector<DataBookEntry> CSVReader::readCSV(const std::string& csvFilename)
{
    std::vector<DataBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    bool isHeader = true; // Flag to skip the header row

    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            if (isHeader)
            {
                isHeader = false; // Skip the first line
                continue;
            }

            try
            {
                std::vector<DataBookEntry> rowEntries = stringsToDBE(tokenise(line, ','));
                // Add all entries from this row to the main entries vector
                entries.insert(entries.end(), rowEntries.begin(), rowEntries.end());
                
            }
            catch (const std::exception& e)
            {
                std::cerr << "CSVReader::readCSV bad data: " << e.what() << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "CSVReader::readCSV could not open file: " << csvFilename << std::endl;
        throw std::runtime_error("Unable to open CSV file.");
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries." << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(const std::string& csvLine, char separator)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = csvLine.find(separator);

    while (end != std::string::npos)
    {
        tokens.push_back(csvLine.substr(start, end - start));
        start = end + 1;
        end = csvLine.find(separator, start);
    }
    tokens.push_back(csvLine.substr(start));

    return tokens;
}

std::vector<DataBookEntry> CSVReader::stringsToDBE(const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2) // Minimum: timestamp + at least one temperature
    {
        std::cerr << "CSVReader::stringsToDBE: Bad line with insufficient tokens." << std::endl;
        throw std::invalid_argument("Invalid number of tokens.");
    }

    std::vector<DataBookEntry> entries; // Store all entries for this row

    try
    {
        // Parse timestamp (first token)
        std::string timestamp = tokens[0];

        // Iterate over each temperature column and assign it to the correct country
        for (size_t i = 1; i < tokens.size(); ++i)
        {
            if (tokens[i].empty())
                continue; // Skip empty tokens

            // Attempt to convert the temperature value
            double temp = std::stod(tokens[i]);

            // Match the column index to the country
            Country country;
            switch (i)
            {
                case 1:  country = Country::AT; break;
                case 2:  country = Country::BE; break;
                case 3:  country = Country::BG; break;
                case 4:  country = Country::CH; break;
                case 5:  country = Country::CZ; break;
                case 6:  country = Country::DE; break;
                case 7:  country = Country::DK; break;
                case 8:  country = Country::EE; break;
                case 9:  country = Country::ES; break;
                case 10: country = Country::FI; break;
                case 11: country = Country::FR; break;
                case 12: country = Country::GB; break;
                case 13: country = Country::GR; break;
                case 14: country = Country::HR; break;
                case 15: country = Country::HU; break;
                case 16: country = Country::IE; break;
                case 17: country = Country::IT; break;
                case 18: country = Country::LT; break;
                case 19: country = Country::LU; break;
                case 20: country = Country::LV; break;
                case 21: country = Country::NL; break;
                case 22: country = Country::NO; break;
                case 23: country = Country::PL; break;
                case 24: country = Country::PT; break;
                case 25: country = Country::RO; break;
                case 26: country = Country::SE; break;
                case 27: country = Country::SI; break;
                case 28: country = Country::SK; break;
                default: country = Country::UNKNOWN; break;
            }

            if (country != Country::UNKNOWN)
            {
                // Create a DataBookEntry for this country and temperature
                DataBookEntry dbe{timestamp, {temp}, country};
                entries.push_back(dbe); // Add to the vector of entries
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "CSVReader::stringsToDBE: Failed to parse tokens: " << e.what() << std::endl;
        throw;
    }

    return entries; // Return all entries for this row
}
