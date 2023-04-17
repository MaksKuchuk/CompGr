#ifndef GRAPH2DDATA_H
#define GRAPH2DDATA_H

#include <QString>
#include <QList>

class Graph2DData {
public:
    QString name;
    QString source;

    double maxVal;
    double minVal;

    double minLoc;
    double maxLoc;
    unsigned long long amountOfSamples;

    long long lcur, rcur;

    double Hz;

    QString startDate{};
    double totalSeconds;

    QList<double> samples;
};

#endif
