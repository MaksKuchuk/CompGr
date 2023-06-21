#ifndef GRAPH3DDATA_H
#define GRAPH3DDATA_H

#include <QString>
#include <QList>

class Graph3DData {
public:
    QString name;
    QString source;

    double maxVal;
    double minVal;

    double minLoc;
    double maxLoc;
    size_t width;
    size_t height;

    long long lcur, rcur;

    double Hz = 1;
    double yHz = 1;

    QString startDate = "01-01-2000 00:00:00.000";
    double totalSeconds;

    QList<QList<double>> samples;

    Graph3DData(qint64 wid, qint64 heig);

    ~Graph3DData();
};

#endif
