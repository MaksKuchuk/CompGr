#include "ParseData.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <cmath>
#include <ctime>
#include <iomanip>

#ifndef PARSEDATATXT_H
#define PARSEDATATXT_H

class ParseDataTxt : public ParseData {
    void setChannelsNames(std::string &string_names) {
        std::string cur;
        int i{};
        for (auto c: string_names) {
            if (c != ';')
                cur += c;
            else {
                channels_names[i] = cur;
                i++;
                cur.clear();
            }
        }
        if (!cur.empty())
            channels_names[i] = cur;
    }

    static void getData(std::ifstream &file, auto &val) {
        std::string in_str;
        do {
            std::getline(file, in_str);
        } while (in_str[0] == '#');

        getVal(val, in_str);
    }

    static void getData(std::ifstream &file, std::string &val) {
        std::string in_str;
        do {
            std::getline(file, in_str);
        } while (in_str[0] == '#');

        std::stringstream ss;
        ss << in_str;
        ss >> val;
    }

    static void getVal(unsigned long long &val_to, const std::string &str_from) {
        val_to = std::stoull(str_from);
    }

    static void getVal(double &val_to, const std::string &str_from) {
        val_to = std::stod(str_from);
    }

public:
    void parse(std::ifstream &file_to_parse) override {
        std::string in_str;

        std::string startDate_, startTime_;
        getData(file_to_parse, amountOfChannels);
        getData(file_to_parse, amountOfSamples);
        getData(file_to_parse, Hz);
        getData(file_to_parse, startDate_);
        getData(file_to_parse, startTime_);

        startTime = startDate_ + " " + startTime_;

        totalSeconds = (amountOfSamples - 1) / Hz;
        setDuration(totalSeconds);

        std::tm t{};
        std::istringstream ss(startDate_ + " " + startTime_.substr(0, 8));
        ss.imbue(std::locale(""));
        ss >> std::get_time(&t, "%d-%m-%Y %H:%M:%S");
        auto dotPos = startTime_.find('.');
        int milliseconds = 0;
        if (dotPos != std::string::npos)
            milliseconds = std::stoi(startTime_.substr(dotPos + 1, startTime_.size() - 1));

        t.tm_sec += totalSeconds + milliseconds / 1000.0;
        mktime(&t);

        int millAdd = (totalSeconds - floor(totalSeconds)) * 1000 + milliseconds;
        milliseconds = millAdd >= 1000 ? millAdd % 1000 : millAdd;

        std::ostringstream oss;
        oss << std::put_time(&t, "%d-%m-%Y %H:%M:%S");
        if (milliseconds > 0)
            oss << '.' << milliseconds;

        stopTime = oss.str();

        channels_names = new std::string[amountOfChannels];
        getData(file_to_parse, in_str);
        setChannelsNames(in_str);

        channels = new double *[amountOfChannels];
        for (int i = 0; i < amountOfChannels; ++i)
            channels[i] = new double[amountOfSamples];

        extremums = new std::pair<double, double>[amountOfChannels];
        double val;
        for (int k = 0; k < amountOfSamples; ++k) {
            for (int i = 0; i < amountOfChannels; ++i) {
                file_to_parse >> val;
                if (k == 0) {
                    extremums[i].first = extremums[i].second = val;
                } else {
                    extremums[i].first = std::min(extremums[i].first, val);
                    extremums[i].second = std::max(extremums[i].second, val);
                }
                channels[i][k] = val;
            }
        }
    }
};

#endif
