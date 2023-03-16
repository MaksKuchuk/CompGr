#include "TransformToFourierSpectrum.hpp"

Graph2DData* TransformToFourierSpectrum::transform(ParseData* data, long long n) {
    long long size = data->getAmountOfSamples();
    double* in = (double*)fftw_malloc(sizeof(double) * size);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (size/ 2 + 1));
    fftw_plan plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);
    in = data->getChannel(n);
    fftw_execute(plan);

    double max = 0;
    double min = std::numeric_limits<double>::max();

    double* amplitudes = new double(size/2+1);
    for (int i = 0; i < size/2+1; ++i) {
        amplitudes[i] = std::sqrt(std::pow(out[i][0], 2) + std::pow(out[i][1], 2));
        if (amplitudes[i] < min)
            min = amplitudes[i];
        if (amplitudes[i] > max)
            max = amplitudes[i];
    }
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    Graph2DData* data2d = new Graph2DData();
    data2d->Hz = 1 / size;
    data2d->totalSeconds = 1;
    data2d->amountOfSamples = size;
    data2d->samples = amplitudes;
    data2d->lcur = 0;
    data2d->rcur = size - 1;
    data2d->source = data->getFileName() + " Fourier transform";
    data2d->minVal = min;
    data2d->maxVal = max;

    return data2d;
}
