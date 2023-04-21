#ifndef TRANSFORMTOFOURIERSPECTRUM_H
#define TRANSFORMTOFOURIERSPECTRUM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph2DData.hpp"

class TransformToFourierSpectrum {
    static void FFTAnalysis(QList<double>& AVal, QList<double>& FTvl, int Nvl, int Nft);
public:

    static std::shared_ptr<Graph2DData> transform(std::shared_ptr<GeneralData> data, long long n);

};

#endif
