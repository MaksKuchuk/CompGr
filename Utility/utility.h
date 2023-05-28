#ifndef UTILITY_H
#define UTILITY_H

#include <QList>
#include <QString>

class Utility
{
public:
    Utility() = delete;

    static QList<double> StringToDList(const QString& str, const QString& sep = ",");

    static double LinearScale(double current, double total, double min = -1, double max = 1);
    static double ExpScale(double current, double total, double min = -1, double max = 1);
    static double LogScale(double current, double total, double min = -1, double max = 1);
};

#endif // UTILITY_H
