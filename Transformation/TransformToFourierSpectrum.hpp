#ifndef TRANSFORMTOFOURIERSPECTRUM_H
#define TRANSFORMTOFOURIERSPECTRUM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph2DData.hpp"

class TransformToFourierSpectrum {
    static void FFTAnalysis(double *AVal, double *FTvl, int Nvl, int Nft);
public:

    static Graph2DData* transform(ParseData* data, long long n);

};

#endif
