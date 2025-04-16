#include "Candlestick.h"
#include <algorithm>
#include <cmath>
#include <numeric> // For std::accumulate

Candlestick::Candlestick(
    std::vector<double> _opens,
    std::vector<double> _highs,
    std::vector<double> _lows,
    std::vector<double> _closes)
    : opens(_opens),
      highs(_highs),
      lows(_lows),
      closes(_closes)
{
}

std::vector<Candlestick> Candlestick::getCandlestickData(Country country, std::string startYear, std::string endYear)
{
    int startYear_int = std::stoi(startYear);
    int endYear_int = std::stoi(endYear);

    if (startYear_int > endYear_int)
    {
        throw std::runtime_error("Start year cannot be greater than end year.");
    }

    std::vector<DataBookEntry> allData;
    std::vector<Candlestick> candlestick_data;

    for (int year = startYear_int; year <= endYear_int; ++year)
    {
        std::string currentYear = std::to_string(year);
        std::vector<DataBookEntry> yearlyTemps = DataBook::getTemperatures(country, currentYear);

        if (yearlyTemps.empty())
        {
            continue;
        }

        // Append current year entries to allData
        allData.insert(allData.end(), yearlyTemps.begin(), yearlyTemps.end());

        double yearlyOpen;
        if (year == 1980)
        {
            yearlyOpen = std::numeric_limits<double>::quiet_NaN();
        }
        else
        {
            try
            {
                std::string prevYear = std::to_string(year - 1);
                yearlyOpen = DataBook::getClose(allData, prevYear);
            }
            catch (const std::runtime_error &e)
            {
                yearlyOpen = std::numeric_limits<double>::quiet_NaN();
            }
        }

        double yearlyHigh = DataBook::getHighTemp(yearlyTemps, currentYear);
        double yearlyLow = DataBook::getLowTemp(yearlyTemps, currentYear);
        double yearlyClose = DataBook::getClose(yearlyTemps, currentYear);

        std::vector<double> opens{yearlyOpen};
        std::vector<double> highs{yearlyHigh};
        std::vector<double> lows{yearlyLow};
        std::vector<double> closes{yearlyClose};

        if (opens.size() == highs.size() && highs.size() == lows.size() && lows.size() == closes.size())
        {
            Candlestick candlestick_entry{opens, highs, lows, closes};
            candlestick_data.push_back(candlestick_entry);
        }
        else
        {
            std::cerr << "Inconsistent candlestick data for year: " << currentYear << "\n";
        }
    }

    return candlestick_data;
}

void Candlestick::plotChart(Country country, std::string startYear, std::string endYear, std::vector<Candlestick> chart_data)
{
    if (chart_data.empty())
    {
        std::cerr << "No data available to plot for the given range.\n";
        return;
    }

    std::string space = "  ";
    int y_axis_max = std::numeric_limits<int>::min();
    int y_axis_min = std::numeric_limits<int>::max();

    for (const Candlestick &candle : chart_data)
    {
        if (!candle.highs.empty())
        {
            y_axis_max = std::max(y_axis_max, static_cast<int>(*std::max_element(candle.highs.begin(), candle.highs.end())));
        }
        if (!candle.lows.empty())
        {
            y_axis_min = std::min(y_axis_min, static_cast<int>(*std::min_element(candle.lows.begin(), candle.lows.end())));
        }
    }

    if (y_axis_min == std::numeric_limits<int>::max() || y_axis_max == std::numeric_limits<int>::min())
    {
        std::cerr << "No valid high or low values found for plotting.\n";
        return;
    }

    // Y-axis : Printed from max to min (top to bottom)
    for (int i = y_axis_max; i >= y_axis_min; --i)
    {
        // Y-axis : axis (i = temperature)
        // i = 1 char
        if (i >= 0 && i <= 9)
        {
            std::cout << i << "   | ";
            // i = 2 chars
        }
        else if ((i >= -9 && i <= -1) || (i >= 10 && i <= 99))
        {
            std::cout << i << "  | ";
            // i = 3 chars
        }
        else
        {
            std::cout << i << " | ";
        }

        // Print candlesticks
        for (const Candlestick &candle : chart_data)
        {
            if (!candle.highs.empty() && !candle.lows.empty() && !candle.opens.empty() && !candle.closes.empty())
            {
                double high = candle.highs[0];
                double low = candle.lows[0];
                double open = ceil(candle.opens[0]);
                double close = floor(candle.closes[0]);

                // Print Highs and Highs to Opens - " || "
                if (i == static_cast<int>(high) || (i < static_cast<int>(high) && i > static_cast<int>(open)))
                {
                    std::cout << " || " << space << space;
                }
                // Print Opens and Opens to Closes - "----"
                else if (i == static_cast<int>(open) || (i < static_cast<int>(open) && i > static_cast<int>(close)))
                {
                    std::cout << "----" << space << space;
                }
                // Print Closes and Closes to Lows - " || "
                else if (i == static_cast<int>(close) || (i < static_cast<int>(close) && i > static_cast<int>(low)))
                {
                    std::cout << " || " << space << space;
                }
                else
                {
                    std::cout << space << space << space << space;
                }
            }
        }
        std::cout << std::endl;
    }

    // X-axis : axis, lengthened by 1
    for (int i = std::stoi(startYear); i <= (std::stoi(endYear) + 1); ++i)
    {
        std::cout << "--------";
    }

    // X-axis : Space from origin to first letter of startYear
    std::cout << std::endl;
    std::cout << space << space;

    // X-axis : Print startYear to endYear
    for (int year = std::stoi(startYear); year <= std::stoi(endYear); ++year)
    {
        std::cout << space << year << space;
    }
    std::cout << std::endl;
}

std::vector<Candlestick> Candlestick::dataPredict(Country country, std::string referStartYear, std::string referEndYear, std::vector<Candlestick> reference)
{
    int startYear = std::stoi(referStartYear);
    int endYear = std::stoi(referEndYear);
    int numYears = endYear - startYear + 1;

    if (numYears <= 0 || reference.empty())
    {
        throw std::runtime_error("Invalid reference range or empty reference data.");
    }

    // Helper function: Calculate the linear regression coefficients (slope and intercept)
    auto linearRegression = [](const std::vector<int> &years, const std::vector<double> &values)
    {
        int n = years.size();
        double sumX = std::accumulate(years.begin(), years.end(), 0.0);
        double sumY = std::accumulate(values.begin(), values.end(), 0.0);
        double sumXY = 0.0, sumX2 = 0.0;

        for (int i = 0; i < n; ++i)
        {
            sumXY += years[i] * values[i];
            sumX2 += years[i] * years[i];
        }

        double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
        double intercept = (sumY - slope * sumX) / n;
        return std::make_pair(slope, intercept);
    };

    // Step 1: Prepare data for regression
    std::vector<int> years;
    std::vector<double> avgOpens, avgHighs, avgLows, avgCloses;

    for (int i = 0; i < reference.size(); ++i)
    {
        double avgOpen = std::accumulate(reference[i].opens.begin(), reference[i].opens.end(), 0.0) / reference[i].opens.size();
        double avgHigh = std::accumulate(reference[i].highs.begin(), reference[i].highs.end(), 0.0) / reference[i].highs.size();
        double avgLow = std::accumulate(reference[i].lows.begin(), reference[i].lows.end(), 0.0) / reference[i].lows.size();
        double avgClose = std::accumulate(reference[i].closes.begin(), reference[i].closes.end(), 0.0) / reference[i].closes.size();

        if (!std::isnan(avgOpen) && !std::isnan(avgHigh) && !std::isnan(avgLow) && !std::isnan(avgClose))
        {
            years.push_back(startYear + i);
            avgOpens.push_back(avgOpen);
            avgHighs.push_back(avgHigh);
            avgLows.push_back(avgLow);
            avgCloses.push_back(avgClose);
        }
    }

    if (years.empty())
    {
        throw std::runtime_error("No valid data for regression after filtering out NaN values.");
    }

    // Step 2: Perform linear regression for each component
    std::pair<double, double> openResult = linearRegression(years, avgOpens);
    double slopeOpen = openResult.first;
    double interceptOpen = openResult.second;

    std::pair<double, double> highResult = linearRegression(years, avgHighs);
    double slopeHigh = highResult.first;
    double interceptHigh = highResult.second;

    std::pair<double, double> lowResult = linearRegression(years, avgLows);
    double slopeLow = lowResult.first;
    double interceptLow = lowResult.second;

    std::pair<double, double> closeResult = linearRegression(years, avgCloses);
    double slopeClose = closeResult.first;
    double interceptClose = closeResult.second;

    // Step 3: Predict for the next 10 years
    std::vector<Candlestick> predictions;
    for (int i = 1; i <= 10; ++i)
    {
        int futureYear = endYear + i;

        double predictedOpen = slopeOpen * futureYear + interceptOpen;
        double predictedHigh = slopeHigh * futureYear + interceptHigh;
        double predictedLow = slopeLow * futureYear + interceptLow;
        double predictedClose = slopeClose * futureYear + interceptClose;

        predictions.emplace_back(
            std::vector<double>{predictedOpen},
            std::vector<double>{predictedHigh},
            std::vector<double>{predictedLow},
            std::vector<double>{predictedClose});
    }

    return predictions;
}