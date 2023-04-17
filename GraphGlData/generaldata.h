#ifndef GENERALDATA_H
#define GENERALDATA_H

#include <QString>
#include <QList>
#include <QPointer>
#include <filesystem>

class GeneralData
{
public:
    // where is it from
    QString source;
    // number of channels
    unsigned long long amountOfChannels;
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
    QList<std::pair<double, double>> extremums;

    size_t lcur = 0;
    size_t rcur;

    GeneralData();

    unsigned long long getAmountOfChannels() const;

    unsigned long long getAmountOfSamples() const;

    double getHz() const;

    QString getStartTime() const;

    QString getStopTime() const;

    double getTotalDuration() const;

    std::tuple<unsigned long long, unsigned long long, unsigned long long, double> getDuration() const;

    QString getSource() const;

    // return n-th channel
    const QList<double>& getChannel(long long n) const;

    // return name of n-th channel
    QString getChannelName(long long n) const;

    //return min value of n-th channel
    double minVal(long long n) const;

    //return max value of n-th channel
    double maxVal(long long n) const;

    void setName(QString fName);

    QString getFileName();
};

#endif // GENERALDATA_H
