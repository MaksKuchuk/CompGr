#ifndef GRAPH3DDATA_H
#define GRAPH3DDATA_H

#include <QString>
#include <QList>
#include <QColor>

class Graph3DData {
public:
    QString name;
    QString source;

    double maxVal;
    double minVal;

    double minLoc;
    double maxLoc;
    size_t amountOfSamples;
    size_t depth;

    long long lcur, rcur;

    double Hz = 1;

    QString startDate = "01-01-2000 00:00:00.000";
    double totalSeconds;

    QList<QList<QColor>> samples;
    QPair<double, double> x_range;
    QPair<double, double> y_range;

    Graph3DData() = default;
};

#endif
