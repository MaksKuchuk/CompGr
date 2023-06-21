#ifndef TRANSFORMTOFOURIERSPECTRUM_H
#define TRANSFORMTOFOURIERSPECTRUM_H

#include "../GraphGlData/Graph2DData.hpp"

#include <complex>
#include <valarray>
#include <cmath>
#include <limits>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
const double PI = 3.141592653589793238460;

// First val modes
enum class FourierModes {
    KEEP_FIRST_VAL,
    ZERO_FIRST_VAL,
    EQUALIZE_WITH_ADJ
};

// Spectrum analysis modes
enum class SpectrumModes {
    AMPLITUDE_SPECTRUM,
    PSD
};

class TransformToFourierSpectrum {
    static void FFTAnalysis(QList<double>& AVal, QList<double>& FTvl, int Nvl, int Nft);

    static void FFTW(CArray& x);

    static void first_val_change(CArray& x, FourierModes mode);

    static void amplitudeSpectrum(CArray&, QList<double>&, long long, double);

    static void PSDSpectrum(CArray&, QList<double>&, long long, double);

public:
    static std::shared_ptr<Graph2DData> transform(std::shared_ptr<Graph2DData> data, long long smooth,
                                                  SpectrumModes mode, FourierModes fmode, qint64 fill_zeros_to = 0);
};

#endif
