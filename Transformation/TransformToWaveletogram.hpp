#ifndef TRANSFORMTOWAVELETOGRAM_H
#define TRANSFORMTOWAVELETOGRAM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph3DData.hpp"

class TransformToFourierSpectrum {

    static Graph3DData* transform(ParseData data, long long n);

};

#endif
