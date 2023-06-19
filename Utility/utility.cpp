#include "utility.h"
#include <cmath>

QList<double> Utility::StringToDList(const QString& str, const QString& sep) {
    QList<double> dList;
    foreach(QString d, str.split(sep)) {
        dList.push_back(d.toDouble());
    }
    return dList;
}

double Utility::LinearScale(double current, double total, double min, double max) {
    return min + (max - min) * (current / total);
}
double Utility::ExpScale(double current, double total, double min, double max) {
//    auto single_range = (std::pow(10, current / total) - 1) / 9;
    auto single_range = (std::pow(81, current / total) - 1) / 80;
    return min + (max - min) * single_range;
}
double Utility::LogScale(double current, double total, double min, double max) {
//    auto single_range = std::log10(1 + current / total * 9);
    auto single_range = std::log10(80 * current / total + 1) / std::log10(81);
    return min + (max - min) * single_range;
}

QString Utility::freqToTime(double freq) {
    if (freq <= 0)
        return "-";

    auto time = 1/freq;

    return timeToTime(time);

}

QString Utility::timeToTime(double time) {
    constexpr double secInHour = 3600;
    constexpr double secInDay = secInHour * 24;
    constexpr double secInYear = secInDay * 365;

    if (time > secInYear)
        return QString::number(time / secInYear, 'g', 3)+" y";
    if (time > 7 * secInDay)
        return QString::number(time / secInDay, 'g', 3)+" d";
    if (time > secInHour)
        return QString::number(time / secInHour, 'g', 3)+" h";
    if (time > 3 * 60)
        return QString::number(time / 60, 'g', 3)+" m";

    if (time > 1)
        return QString::number(time, 'g', 3)+" s";
    if (time > 1e-3)
        return QString::number(time * 1e3, 'g', 3)+" ms";

    return QString::number(time * 1e6, 'g', 3)+" mcs";

}
