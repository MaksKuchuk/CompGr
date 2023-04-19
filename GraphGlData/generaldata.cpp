#include "generaldata.h"

GeneralData::GeneralData() = default;

void GeneralData::addNewChannel(std::shared_ptr<Graph2DData> data) {
    if (amountOfSamples != data->amountOfSamples)
        throw std::runtime_error("Different numbers of samples: " + std::to_string(amountOfSamples)
                                 + " != "+ std::to_string(data->amountOfSamples));

    channels.push_back(data->samples);
    channels_names.push_back(data->name);
    extremums.push_back({data->minVal, data->maxVal});
    ++amountOfChannels;
}

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

std::shared_ptr<Graph2DData> GeneralData::channelTo2D(long long n) const {
    auto gr = std::make_shared<Graph2DData>();

    gr->name = getChannelName(n);
    gr->source = getSource();

    gr->maxVal = maxVal(n);
    gr->minVal = minVal(n);

    gr->maxLoc = gr->maxVal;
    gr->minLoc = gr->minVal;

    gr->amountOfSamples = getAmountOfSamples();
    gr->Hz = getHz();
    gr->totalSeconds = getTotalDuration();
    gr->samples = getChannel(n);

    gr->lcur = 0;
    gr->rcur = gr->amountOfSamples - 1;

    return gr;
}

GeneralData::GeneralData(std::shared_ptr<Graph2DData> data) :
    amountOfSamples(data->amountOfSamples),
    startTime("01-01-2000 00:00:00"),
    totalSeconds(data->totalSeconds),
    Hz(data->Hz)
{
    addNewChannel(data);
}
