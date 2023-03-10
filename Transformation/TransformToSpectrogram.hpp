#ifndef TRANSFORMTOSPECTROGRAM_H
#define TRANSFORMTOSPECTROGRAM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph3DData.hpp"

class TransformToFourierSpectrum {

    static Graph3DData* transform(ParseData* data, long long n);

};

#endif
