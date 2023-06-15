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

    double Hz = 1;

    QString startDate = "01-01-2000 00:00:00.000";
    double totalSeconds;

    QList<double> samples;

    std::shared_ptr<Graph2DData> GetSlice(qint64 l, qint64 r);
};

#endif
