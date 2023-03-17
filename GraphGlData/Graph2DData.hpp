#ifndef GRAPH2DDATA_H
#define GRAPH2DDATA_H

#include <string>

class Graph2DData {
public:
    std::string name;
    std::string source;

    double maxVal;
    double minVal;

    double minLoc;
    double maxLoc;
    unsigned long long amountOfSamples;

    long long lcur, rcur;

    double Hz;

    double totalSeconds;

    double* samples;
};

#endif
