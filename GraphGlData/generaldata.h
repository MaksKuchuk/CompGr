#ifndef GENERALDATA_H
#define GENERALDATA_H

#include <QString>
#include <QList>
#include <QPointer>
#include <filesystem>
#include "Graph2DData.hpp"

class GeneralData
{
protected:
    void setDuration(double totalSeconds_);
    void setStopTime();

public:
    // number of channels
    unsigned long long amountOfChannels = 0;
    // number of samples
    unsigned long long amountOfSamples;
    // sampling frequency (Hz)
    double Hz;
    // recording start datetime (dd-mm-yyyy hh:mm:ss.sss)
    QString startTime;
    // recording stop datetime (dd-mm-yyyy hh:mm:ss.sss)
    QString stopTime;
    // duration
    double totalSeconds;
    unsigned long long days;
    unsigned long long hours;
    unsigned long long minutes;
    double seconds;
    // data
    QList<QList<double>> channels;
    QList<QString> channels_names;
    QList<QString> sources;
    QList<std::pair<double, double>> extremums;

    QList<int> modellingCounts;

    size_t lcur = 0;
    size_t rcur;

    GeneralData();
    GeneralData(std::shared_ptr<Graph2DData>);
    GeneralData(std::shared_ptr<Graph2DData>, QString name);

    void addNewChannel(std::shared_ptr<Graph2DData> data);
    void addNewChannel(std::shared_ptr<Graph2DData> data, QString name);

    unsigned long long getAmountOfChannels() const;

    unsigned long long getAmountOfSamples() const;

    double getHz() const;

    QString getStartTime() const;

    QString getStopTime() const;

    double getTotalDuration() const;

    std::tuple<unsigned long long, unsigned long long, unsigned long long, double> getDuration() const;

    // return n-th channel
    const QList<double>& getChannel(long long n) const;

    // return name of n-th channel
    QString getChannelName(long long n) const;

    //return min value of n-th channel
    double minVal(long long n) const;

    //return max value of n-th channel
    double maxVal(long long n) const;

    void setName(QString fName);

    QString getChannelSource(qint64 n) const;

    std::shared_ptr<Graph2DData> channelTo2D(long long n) const;
};

#endif // GENERALDATA_H
