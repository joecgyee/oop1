#include "DataBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric> // For std::accumulate
#include <limits>  // For std::numeric_limits

// Define the static member
std::vector<DataBookEntry> DataBook::datas;

/** construct, reading a csv data file */
DataBook::DataBook(std::string filename)
{
    datas = CSVReader::readCSV(filename);
}

std::string DataBook::getEarliestYear()
{
    // Return the earliest year by extracting the year from the first entry
    return datas[0].timestamp.substr(0, 4); // Extract "YYYY" from "YYYY-MM-DDTHH:MM:SSZ"
}

std::string DataBook::getNextYear(std::string timestamp)
{
    // Extract the year from the given timestamp
    int currentYear = std::stoi(timestamp.substr(0, 4));
    std::string nextYear = "";

    // Iterate through the dataset to find the next year
    for (const DataBookEntry &e : datas)
    {
        int entryYear = std::stoi(e.timestamp.substr(0, 4));
        if (entryYear > currentYear)
        {
            nextYear = std::to_string(entryYear); // Update the next year
            break;
        }
    }

    // If no later year is found, wrap around to the earliest year
    if (nextYear.empty())
    {
        nextYear = datas[0].timestamp.substr(0, 4); // Return the earliest year
    }

    return nextYear;
}

std::vector<DataBookEntry> DataBook::getTemperatures(Country country, std::string timestamp)
{
    int year = std::stoi(timestamp.substr(0, 4)); // Extract year from timestamp
    std::vector<DataBookEntry> datas_sub;

    for (const DataBookEntry &e : datas)
    {
        int entryYear = std::stoi(e.timestamp.substr(0, 4)); // Extract year from entry timestamp
        if (e.country == country && entryYear == year)
        {
            datas_sub.push_back(e);
        }
    }

    return datas_sub;
}

double DataBook::getHighTemp(std::vector<DataBookEntry> &datas, std::string timestamp)
{
    double max = std::numeric_limits<double>::lowest(); // Initialize with the lowest possible double value
    std::string entryYear = timestamp.substr(0, 4);     // Extract the year from the timestamp

    for (const DataBookEntry &e : datas)
    {
        if (e.timestamp.substr(0, 4) == entryYear)
        {
            for (double temp : e.temperatures)
            {
                if (temp > max)
                    max = temp;
            }
        }
    }
    return max;
}

double DataBook::getLowTemp(std::vector<DataBookEntry> &datas, std::string timestamp)
{
    double min = std::numeric_limits<double>::max(); // Initialize with the highest possible double value
    std::string entryYear = timestamp.substr(0, 4);  // Extract the year from the timestamp

    for (const DataBookEntry &e : datas)
    {
        if (e.timestamp.substr(0, 4) == entryYear)
        {
            for (double temp : e.temperatures)
            {
                if (temp < min)
                    min = temp;
            }
        }
    }
    return min;
}

double DataBook::getClose(std::vector<DataBookEntry> &datas, std::string timestamp)
{
    // Parse the provided timestamp to extract the year
    int currentYear = std::stoi(timestamp.substr(0, 4));

    // Search for entries (temperatures) corresponding to the current year
    std::vector<DataBookEntry> currentYearEntries;
    
    // Find the earliest year in the `datas` vector
    int earliestYear = std::stoi(datas.front().timestamp.substr(0, 4)); // Assuming `datas` is sorted by timestamp

    for (const DataBookEntry &entry : datas)
    {
        int entryYear = std::stoi(entry.timestamp.substr(0, 4));

        // Skip entries from years earlier than the earliest year in `datas`
        if (entryYear < earliestYear)
        {
            continue;
        }
        else if (entryYear == currentYear)
        {
            currentYearEntries.push_back(entry); // Collect entries for the current year
        }
    }

    // If no data exists for the current year, throw an exception or return an error value
    if (currentYearEntries.empty())
    {
        throw std::runtime_error("No data found for the current year.");
    }

    // Calculate the average of the mean temperatures for the current year
    double totalTemperature = 0.0;
    int temperatureCount = 0;

    for (const DataBookEntry &entry : currentYearEntries)
    {
        // Calculate the mean temperature for the entry
        double meanTemp = std::accumulate(entry.temperatures.begin(), entry.temperatures.end(), 0.0) / entry.temperatures.size();

        totalTemperature += meanTemp;
        temperatureCount++;
    }

    // Compute the overall average
    double close_averageMeanTemperature = totalTemperature / temperatureCount;

    return close_averageMeanTemperature;
}