#ifndef TRANSFORMTOOSCILLOGRAM_H
#define TRANSFORMTOOSCILLOGRAM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph2DData.hpp"

class TransformToOscillogram {
public:

    static Graph2DData* transform(ParseData* data, long long n);

};

#endif
