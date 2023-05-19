#ifndef TRANSFORMTOFOURIERSPECTRUM_H
#define TRANSFORMTOFOURIERSPECTRUM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph2DData.hpp"

#include <complex>
#include <valarray>
#include <cmath>
#include <limits>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
const double PI = 3.141592653589793238460;

// First val modes
const int KEEP_FIRST_VAL = 0;
const int ZERO_FIRST_VAL = 1;
const int EQUALIZE_WITH_ADJ = 2;

// Spectrum analysis modes
const int AMPLITUDE_SPECTRUM = 0;
const int PSD = 1;

class TransformToFourierSpectrum {
    static void FFTAnalysis(QList<double>& AVal, QList<double>& FTvl, int Nvl, int Nft);

    static void FFTW(CArray& x);

    static void first_val_change(CArray& x, int mode);

    static void amplitudeSpectrum(CArray&, QList<double>, long long, double);

    static void PSDSpectrum(CArray&, QList<double>, long long, double);

    static void smoothing(QList<double>, const long long, size_t);

public:
    static std::shared_ptr<Graph2DData> transform(std::shared_ptr<GeneralData> data, long long n, long long L, int mode, int, long long, long long);
};

#endif
