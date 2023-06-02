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
    auto single_range = (std::pow(10, current / total) - 1) / 9;
    return min + (max - min) * single_range;
}
double Utility::LogScale(double current, double total, double min, double max) {
    auto single_range = std::log10(1 + current / total * 9);
    return min + (max - min) * single_range;
}
