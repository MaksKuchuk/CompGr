#ifndef GRAPH2DDATA_H
#define GRAPH2DDATA_H

#include <string>

class Graph3DData {
    std::string name;
    std::string source;

    double maxValSample;
    double minValSample;
    unsigned long long amountOfSamples;

    double Hz;

    double totalSeconds;

    double* samples;
    double* depth;
};

#endif
