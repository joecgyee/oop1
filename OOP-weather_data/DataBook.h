#pragma once
#include "DataBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class DataBook
{
    public:
        /** construct, reading a csv data file */
        DataBook(std::string filename);

        /** return vector of temperature data according to the sent filters (country and year) */
        static std::vector<DataBookEntry> getTemperatures(Country country, std::string timestamp);

        /** returns the earliest year in the databook*/
        std::string getEarliestYear();
        /** returns the next year after the sent year in the databook.
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextYear(std::string timestamp);

        /* highest temperature value seen this (year) time frame. */
        static double getHighTemp(std::vector<DataBookEntry> &datas, std::string timestamp);
        /* lowest temperature value seen this (year) time frame. */
        static double getLowTemp(std::vector<DataBookEntry> &datas, std::string timestamp);
        /* the average mean temperature per unit in this (year) time frame. */
        static double getClose(std::vector<DataBookEntry> &datas, std::string timestamp);

    private:
        static std::vector<DataBookEntry> datas;
};

