#include "ParseData.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <thread>

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

    static void setChannels(ParseDataTxt *data, size_t start_line,
                            const char *buf, size_t start_buf, size_t end) {
        std::string cur_num{};
        size_t i{}, j = start_line;
        for (size_t p = start_buf; p <= end; ++p) {
            if (buf[p] == ' ' || buf[p] == '\n') {
                data->channels[i][j] = std::stod(cur_num);
                cur_num.clear();
                ++i;
                if (i >= data->amountOfChannels) {
                    i = 0;
                    ++j;
                }
            } else {
                cur_num += buf[p];
            }
        }
    }

    void threadsHandle(const char* buf, size_t buf_length) {
        int number_of_threads = 8;

        auto *position_end_line = new size_t[number_of_threads];
        auto *position_end_buf = new size_t[number_of_threads];

        size_t end{};
        for (size_t y = 0; y < number_of_threads; ++y) {
            if (y + 1 != number_of_threads) {
                end = amountOfSamples / number_of_threads * (y + 1);
                while (buf[end] != '\n')
                    end++;
            } else {
                end = amountOfSamples;
            }
            position_end_line[y] = end;
        }

        size_t ii{}, z{}, count{};
        while (ii < buf_length && z + 1 < number_of_threads) {
            if (buf[ii] == '\n') {
                ++count;
                if (count == position_end_line[z]) {
                    position_end_buf[z] = ii;
                    ++z;
                }
            }
            ++ii;
        }
        position_end_buf[number_of_threads - 1] = buf_length;

        size_t s{}, e{}, el{};
        auto **threads = new std::thread *[number_of_threads];
        for (size_t y = 0; y < number_of_threads; ++y) {
            if (y == 0) {
                s = 0;
            } else {
                s = position_end_buf[y - 1] + 1;
                el = position_end_line[y - 1];
            }
            e = position_end_buf[y];
            threads[y] = new std::thread(setChannels, this, el, buf, s, e);
        }

        for (size_t y = 0; y < number_of_threads; ++y) {
            threads[y]->join();
        }
        for (size_t y = 0; y < number_of_threads; ++y) {
            delete threads[y];
        }

        delete[] buf;
        delete[] position_end_buf;
        delete[] position_end_line;
        delete[] threads;
    }

public:
    void parse(std::ifstream &file_to_parse) override {
        file_to_parse.seekg(0, std::ifstream::end);
        size_t file_length = file_to_parse.tellg();
        file_to_parse.seekg(0);

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


        size_t buf_length = file_length - file_to_parse.tellg();
        auto buf = new char[buf_length + 1];
        file_to_parse.read(buf, buf_length);
        buf[buf_length] = '\n';
        threadsHandle(buf, buf_length);

        extremums = new std::pair<double, double>[amountOfChannels];
        for (int i = 0; i < amountOfChannels; ++i) {
            extremums[i].first = extremums[i].second = channels[i][0];
        }
        for (int k = 1; k < amountOfSamples; ++k) {
            for (int i = 0; i < amountOfChannels; ++i) {
                extremums[i].second = std::max(extremums[i].second, channels[i][k]);
                extremums[i].first = std::min(extremums[i].first, channels[i][k]);
            }
        }
    }
};

#endif
