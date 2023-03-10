#ifndef GRAPH2DDATA_H
#define GRAPH2DDATA_H

#include <string>

class Graph2DData {
    std::string name;
    std::string source;

    double maxVal;
    double minVal;
    unsigned long long amountOfSamples;

    double Hz;

    double totalSeconds;

    double* samples;
};

#endif
