#pragma once

#include "Candlestick.h"
#include <vector>
#include <string>

class MerkelMain
{
    public:
        MerkelMain();
        void init();

    private:
        void printMenu();
        void printHelp();

        /** TASK 1: Compute candlestick data */
        void printWeatherStats(); 

        /** TASK 2: Create a text-based plot of the candlestick data */
        /** TASK 3: Filter Data and Plotting using text */
        void plotCandlestickChart();

        /** TASK 4: Predicting Data and Plotting */
        void weatherPredict();
        
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);
        
        std::string currentYear;

        DataBook databook{"weather_data_EU_1980-2019_temp_only.csv"};

};