#pragma once

#include "DataBookEntry.h"
#include "DataBook.h"
#include "CSVReader.h"
#include <iostream>
#include <string>

class Candlestick
{
    public:
        Candlestick(
        std::vector <double> _opens,
        std::vector <double> _highs,
        std::vector <double> _lows,
        std::vector <double> _closes
        );

        std::vector <double> opens;
        std::vector <double> highs;
        std::vector <double> lows;
        std::vector <double> closes;

        /* return vector of candlestick data (open,high,low,close) from startYear to endYear of selected country */
        /* [{opens},{highs},{lows},{closes}] */
        std::vector<Candlestick> getCandlestickData(Country country, std::string startYear, std::string endYear);

        /* Text-based plot of the Candlestick data */
        void plotChart(Country country, std::string startYear, std::string endYear, std::vector<Candlestick> chart_data);
        
        /* Predicting Data : pass in Country, referStartYear, referEndYear, and vector<Candlestick> of them */
        /* Return a vector<Candlestick> of next ten years after referEndYear */
        std::vector<Candlestick> dataPredict(Country country, std::string referStartYear, std::string referEndYear, std::vector<Candlestick> reference);

    private:
        std::vector <Candlestick> candlestick_data;
};