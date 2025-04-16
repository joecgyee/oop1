#include "MerkelMain.h"
#include <iostream>
#include <map>
#include <algorithm>

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;
    currentYear = databook.getEarliestYear();

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    std::cout << std::endl;
    std::cout << "============== MENU ==============" << std::endl;

    // 1 print help
    std::cout << "1: Print help" << std::endl;
    // 2 print weather stats
    std::cout << "2: Print weather stats" << std::endl;
    // 3 plot of candlestick data
    std::cout << "3: Plot candlestick chart" << std::endl;
    // 4 weather prediction and plotting
    std::cout << "4: Weather predict" << std::endl;
    // 5 continue
    std::cout << "5: Continue" << std::endl;

    std::cout << "----------------------------------" << std::endl;
    std::cout << "Current year: " << currentYear << std::endl;
    std::cout << "==================================" << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - This is a technical analysis toolkit for visualising and predicting weather data using a command line interface. " << std::endl;
    std::cout << "Dataset: weather_data_EU_1980-2019_temp_only" << std::endl;
}

void MerkelMain::printWeatherStats()
{
    std::cout << "Print weather stats - Enter country and year range: country,start year,end year (e.g. AT,1980,1984) " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::printWeatherStats Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            std::cout << std::endl;
            std::cout << "Country: " << tokens[0] << std::endl;
            std::cout << "----------------------------------" << std::endl;
            // Print table header
            std::cout << "Year\tOpen\tHigh\tLow\tClose" << std::endl;

            // Parse input tokens
            Country country = DataBookEntry::stringToCountry(tokens[0]);
            int startYear = std::stoi(tokens[1]);
            int endYear = std::stoi(tokens[2]);

            // Create a Candlestick object and fetch candlestick data
            Candlestick weather_stats({}, {}, {}, {});
            std::vector<Candlestick> weather_stats_data = weather_stats.getCandlestickData(country, tokens[1], tokens[2]);

            // Print the candlestick data
            int year = startYear;
            for (const auto &candle : weather_stats_data)
            {
                if (year > endYear)
                    break; // Stop if we exceed the range

                std::cout << year << "\t"; // Print the current year

                // Print Open values
                if (!candle.opens.empty())
                {
                    std::cout << candle.opens[0] << "\t";
                }
                else
                {
                    std::cout << "NaN\t";
                }

                // Print High values
                if (!candle.highs.empty())
                {
                    std::cout << candle.highs[0] << "\t";
                }
                else
                {
                    std::cout << "NaN\t";
                }

                // Print Low values
                if (!candle.lows.empty())
                {
                    std::cout << candle.lows[0] << "\t";
                }
                else
                {
                    std::cout << "NaN\t";
                }

                // Print Close values
                if (!candle.closes.empty())
                {
                    std::cout << candle.closes[0] << std::endl;
                }
                else
                {
                    std::cout << "NaN" << std::endl;
                }

                ++year;
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}

void MerkelMain::plotCandlestickChart()
{
    std::cout << "Plot candlestick chart - Enter country and year range: country,start year,end year (e.g. AT,1980,1984) " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::plotCandlestickChart Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            std::cout << std::endl;
            Country country = DataBookEntry::stringToCountry(tokens[0]);
            std::string startYear = tokens[1];
            std::string endYear = tokens[2];

            // Header
            std::cout << "Candlestick chart of " << tokens[0] << "'s temperature data from " << startYear << " to " << endYear << std::endl;

            // Create a Candlestick object and fetch candlestick data
            Candlestick chart({}, {}, {}, {});
            std::vector<Candlestick> chart_data = chart.getCandlestickData(country, startYear, endYear);

            // Plot Candlestick chart
            chart.plotChart(country, startYear, endYear, chart_data);
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}

void MerkelMain::weatherPredict()
{
    std::cout << "Data predict - Predict the next 10 years of weather stats for a selected country using its historical data." << std::endl;
    std::cout << "Enter country and reference year range: country,referStartYear,referEndYear (e.g. AT,2000,2010) " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::weatherPredict Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            std::cout << std::endl;
            Country country = DataBookEntry::stringToCountry(tokens[0]);
            std::string referStartYear = tokens[1];
            std::string referEndYear = tokens[2];

            // Header
            std::cout << "Data prediction for " << tokens[0] << " temperature refering to its data of " << referStartYear << " to " << referEndYear << std::endl;
            std::cout << "Next 10 years: " << std::endl;

            // Create Candlestick object and fetch candlestick data of reference years
            Candlestick prediction({}, {}, {}, {});
            std::vector<Candlestick> refer_data = prediction.getCandlestickData(country, referStartYear, referEndYear);

            // Predict the candlestick data with refer_data
            std::vector<Candlestick> predict_data = prediction.dataPredict(country, referStartYear, referEndYear, refer_data);

            // Next 10 years
            std::string futureStartYear = std::to_string(int(std::stoi(referEndYear) + 1));
            std::string futureEndYear = std::to_string(int(std::stoi(referEndYear) + 10));

            // Plot chart of predicted candlestick data for next 10 years
            prediction.plotChart(country, futureStartYear, futureEndYear, predict_data);
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame." << std::endl;
    currentYear = databook.getNextYear(currentYear);
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-5" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << std::endl;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 1)
    {
        printHelp();
    }
    else if (userOption == 2)
    {
        printWeatherStats();
    }
    else if (userOption == 3)
    {
        plotCandlestickChart();
    }
    else if (userOption == 4)
    {
        weatherPredict();
    }
    else if (userOption == 5)
    {
        gotoNextTimeframe();
    }
    else // bad input
    {
        std::cout << "Invalid choice. Choose 1-7" << std::endl;
    }
}
