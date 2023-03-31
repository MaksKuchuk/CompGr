#ifndef MODELING_H
#define MODELING_H

#include "GraphGlData/Graph2DData.hpp"
class Modeling {
public:
    static Graph2DData* delayedSingleImpulse(const long long N, const double T, const long long n);
};

#endif // MODELING_H
