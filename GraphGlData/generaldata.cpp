#include "generaldata.h"

GeneralData::GeneralData() = default;


unsigned long long GeneralData::getAmountOfChannels() const {
    return amountOfChannels;
};

unsigned long long GeneralData::getAmountOfSamples() const {
    return amountOfSamples;
};

double GeneralData::getHz() const {
    return Hz;
}

QString GeneralData::getStartTime() const {
    return startTime;
}

QString GeneralData::getStopTime() const {
    return stopTime;
}

double GeneralData::getTotalDuration() const {
    return totalSeconds;
}

std::tuple<unsigned long long, unsigned long long, unsigned long long, double> GeneralData::getDuration() const {
    return {days, hours, minutes, seconds};
}

QString GeneralData::getSource() const {
    return source;
}

// return n-th channel
const QList<double>& GeneralData::getChannel(long long n) const {
    if (channels.size() > n)
        return channels[n];
    throw std::runtime_error("Channels out of range");
}

// return name of n-th channel
QString GeneralData::getChannelName(long long n) const {
    if (channels_names.size() > n)
        return channels_names[n];
    throw std::runtime_error("Channels names out of range");
}

//return min value of n-th channel
double GeneralData::minVal(long long n) const {
    if (extremums.size() > n)
        return extremums[n].first;
    throw std::runtime_error("Extremums out of range");
}

//return max value of n-th channel
double GeneralData::maxVal(long long n) const {
    if (extremums.size() > n)
        return extremums[n].second;
    throw std::runtime_error("Extremums out of range");
}

void GeneralData::setName(QString fName) {
    source = fName;
}

QString GeneralData::getFileName() {
    return source;
}
