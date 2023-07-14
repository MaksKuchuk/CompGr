#include "TransformToFourierSpectrum.hpp"
#include "../Utility/SGSmooth.hpp"

void TransformToFourierSpectrum::amplitudeSpectrum(CArray& FTvl, QList<double>& new_data, long long size, double T) {
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = T * abs(FTvl[i]);
    }
}

void TransformToFourierSpectrum::PSDSpectrum(CArray& FTvl, QList<double>& new_data, long long size, double T) {
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = std::pow(T, 2) * std::pow(abs(FTvl[i]), 2);
    }
}

void TransformToFourierSpectrum::first_val_change(CArray& x, FourierModes mode) {
    if (mode == FourierModes::EQUALIZE_WITH_ADJ)
        x[0] = abs(x[1]);
    else if (mode == FourierModes::ZERO_FIRST_VAL)
        x[0] = 0;
}

void TransformToFourierSpectrum::FFTW(CArray& x) {
    const size_t N = x.size();
    if (N <= 1)
        return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray odd = x[std::slice(1, N/2, 2)];

    // conquer
    FFTW(even);
    FFTW(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k) {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}
#include <QDebug>
std::shared_ptr<Graph2DData> TransformToFourierSpectrum::transform
    (
        std::shared_ptr<Graph2DData> data,
        long long L,
        SpectrumModes mode,
        FourierModes first_val_mode,
        qint64 fill_zeros_to
    )
{
    long long new_size = data->rcur - data->lcur;

    QList<double> vals(data->samples);

    vals = vals.sliced(data->lcur, new_size);
    if (fill_zeros_to > new_size)
        new_size = fill_zeros_to;
    vals.resize(new_size);

    Complex* buf = new Complex[new_size];
    for (size_t i = 0; i < new_size; ++i) {
        buf[i] = vals[i];
    }
    CArray FTvl(buf, new_size);
    delete[] buf;

    FFTW(FTvl);
    if (first_val_mode != FourierModes::KEEP_FIRST_VAL)
        first_val_change(FTvl, first_val_mode);

    QList<double> new_data(new_size);


    if (mode == SpectrumModes::AMPLITUDE_SPECTRUM)
        amplitudeSpectrum(FTvl, new_data, new_size, 1 / data->Hz);
    else if (mode == SpectrumModes::PSD)
        PSDSpectrum(FTvl, new_data, new_size, 1 / data->Hz);

    if (L) {
        auto smoothed = std::move( sg_smooth(std::vector<double>(new_data.begin(), new_data.end()), L, 2) );
        new_data = std::move(QList<double>(smoothed.begin(), smoothed.end()));
//        smoothing(new_data, L, new_size);
    }

    new_size = new_size / 2;
    if (new_size == 0) new_size = 1;
    new_data.resize(new_size);

    double min = 0;
    double max = 0;
    for (long long i = 0; i < new_size; ++i) {        
        if (new_data[i] > max) {
            max = new_data[i];
        }
    }

    auto data2d = std::make_shared<Graph2DData>();

    data2d->Hz = new_size/data->Hz; // for correct graph

    data2d->totalSeconds = 1;
    data2d->amountOfSamples = new_size;
    data2d->samples = (new_data);
    data2d->lcur = 0;
    data2d->rcur = (new_size) - 1;
    data2d->name = data->name + QString::fromStdString( " Fourier transform" );
    data2d->minVal = min;
    data2d->maxVal = max;
    data2d->minLoc = min;
    data2d->maxLoc = max;

    return data2d;
}
