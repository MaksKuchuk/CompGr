#include <fstream>
#include <string>
#include <tuple>
#include <cmath>

#ifndef PARSEDATA_H
#define PARSEDATA_H

class ParseData {
protected:
    // number of channels
    unsigned long long amountOfChannels;
    // number of samples
    unsigned long long amountOfSamples;
    // sampling frequency (Hz)
    double Hz;
    // recording start datetime (dd-mm-yyyy hh:mm:ss.sss)
    std::string startTime;
    // recording stop datetime (dd-mm-yyyy hh:mm:ss.sss)
    std::string stopTime;
    // duration
    double totalSeconds;
    unsigned long long days;
    unsigned long long hours;
    unsigned long long minutes;
    double seconds;
    // data
    double **channels = nullptr;
    std::string *channels_names = nullptr;
    std::pair<double, double> *extremums = nullptr;
 
 
    void setDuration(double totalSeconds_) {
        days = floor(totalSeconds_ / (60 * 60 * 24));
        totalSeconds_ -= days * (60 * 60 * 24);
 
        hours = floor(totalSeconds_ / (60 * 60));
        totalSeconds_ -= hours * (60 * 60);
 
        minutes = floor(totalSeconds_ / 60);
        totalSeconds_ -= minutes * 60;
 
        seconds = totalSeconds_;
    }
 
public:
    virtual ~ParseData() {
        delete[] channels_names;
        delete[] extremums;
        if (channels != nullptr) {
            for (int i = 0; i < amountOfChannels; ++i)
                delete[] channels[i];
            delete[] channels;
        }
    }
 
    virtual void parse(std::ifstream &file_to_parse) {}
 
    unsigned long long getAmountOfChannels() const {
        return amountOfChannels;
    };
 
    unsigned long long getAmountOfSamples() const {
        return amountOfSamples;
    };
 
    double getHz() const {
        return Hz;
    }
 
    std::string getStartTime() const {
        return startTime;
    }
 
    std::string getStopTime() const {
        return stopTime;
    }
 
    double getTotalDuration() const {
        return totalSeconds;
    }
 
    std::tuple<unsigned long long, unsigned long long, unsigned long long, double> getDuration() const {
        return {days, hours, minutes, seconds};
    }
 
    // return n-th channel
    double *getChannel(long long n) const {
        if (channels != nullptr)
            return channels[n];
        throw std::runtime_error("Channels nullptr");
    }
 
    // return name of n-th channel
    std::string getChannelName(long long n) const {
        if (channels_names != nullptr)
            return channels_names[n];
        throw std::runtime_error("Channels names nullptr");
    }
 
    //return min value of n-th channel
    double minVal(long long n) const {
        if (extremums != nullptr)
            return extremums[n].first;
        throw std::runtime_error("Extremums nullptr");
    }
 
    //return max value of n-th channel
    double maxVal(long long n) const {
        if (extremums != nullptr)
            return extremums[n].second;
        throw std::runtime_error("Extremums nullptr");
    }
};

#endif
