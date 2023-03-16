#ifndef TRANSFORMTOFOURIERSPECTRUM_H
#define TRANSFORMTOFOURIERSPECTRUM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph2DData.hpp"
#include <fftw3.h>
#include <complex.h>

class TransformToFourierSpectrum {
public:

    static Graph2DData* transform(ParseData* data, long long n);

};

#endif
