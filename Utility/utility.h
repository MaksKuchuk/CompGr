#ifndef UTILITY_H
#define UTILITY_H

#include <QList>
#include <QString>

class Utility
{
public:
    Utility() = delete;

    static QList<double> StringToDList(const QString& str, const QString& sep = ",");
};

#endif // UTILITY_H
