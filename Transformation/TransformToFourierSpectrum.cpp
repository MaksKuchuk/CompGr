#include "TransformToFourierSpectrum.hpp"

/*void TransformToFourierSpectrum::FFTAnalysis(QList<double>& AVal, QList<double>& FTvl, int Nvl, int Nft) {
     const double TwoPi = 6.283185307179586;
     int i, j, n, m, Mmax, Istp;
     double Tmpr, Tmpi, Wtmp, Theta;
     double Wpr, Wpi, Wr, Wi;
     double *Tmvl;

     n = Nvl * 2; Tmvl = new double[n];

     for (i = 0; i < n; i+=2) {
      Tmvl[i] = 0;
      Tmvl[i+1] = AVal[i/2];
     }

     i = 1; j = 1;
     while (i < n) {
       if (j > i) {
         Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
         Tmpr = Tmvl[i+1]; Tmvl[i+1] = Tmvl[j+1]; Tmvl[j+1] = Tmpr;
       }
       i = i + 2; m = Nvl;
       while ((m >= 2) && (j > m)) {
         j = j - m; m = m >> 1;
       }
       j = j + m;
     }

     Mmax = 2;
     while (n > Mmax) {
       Theta = -TwoPi / Mmax; Wpi = sin(Theta);
       Wtmp = sin(Theta / 2); Wpr = Wtmp * Wtmp * 2;
       Istp = Mmax * 2; Wr = 1; Wi = 0; m = 1;

       while (m < Mmax) {
         i = m; m = m + 2; Tmpr = Wr; Tmpi = Wi;
         Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
         Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

         while (i < n) {
           j = i + Mmax;
           Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
           Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];

           Tmvl[j] = Tmvl[i] - Tmpr; Tmvl[j-1] = Tmvl[i-1] - Tmpi;
           Tmvl[i] = Tmvl[i] + Tmpr; Tmvl[i-1] = Tmvl[i-1] + Tmpi;
           i = i + Istp;
         }
       }

       Mmax = Istp;
     }

     for (i = 0; i < Nft; i++) {
       j = i * 2; FTvl[i] = 2*sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2))/Nvl;
     }

     delete []Tmvl;
}*/

void TransformToFourierSpectrum::smoothing(QList<double> data, long long L, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        long long sum = 0;
        for (size_t j = -L; j <= L; ++j) {
            sum += data[i + j];
        }
        data[i] = sum / (2*L + 1);
    }
}

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

void TransformToFourierSpectrum::first_val_change(CArray& x, int mode) {
    if (mode == EQUALIZE_WITH_ADJ)
        x[0] = abs(x[1]);
    else if (mode == ZERO_FIRST_VAL)
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

std::shared_ptr<Graph2DData> TransformToFourierSpectrum::transform
    (
        std::shared_ptr<Graph2DData> data,
        long long L,
        int mode,
        int first_val_mode
    )
{
    //long long size = data->getAmountOfSamples();
    //long long new_size = std::pow(2, std::ceil(std::log2(size)));
    long long new_size = data->rcur - data->lcur;

    QList<double> vals(data->samples);
    vals.resize(new_size);
    Complex* buf = new Complex[new_size];
    for (size_t i = 0; i < new_size; ++i) {
        buf[i] = vals[i];
    }
    CArray FTvl(buf, new_size);
    delete[] buf;

    FFTW(FTvl);
    if (first_val_mode != KEEP_FIRST_VAL)
        first_val_change(FTvl, first_val_mode);

    QList<double> new_data(new_size);

    if (mode == AMPLITUDE_SPECTRUM)
        amplitudeSpectrum(FTvl, new_data, new_size, 1 / data->Hz);
    else if (mode == PSD)
        PSDSpectrum(FTvl, new_data, new_size, 1 / data->Hz);

    if (L)
        smoothing(new_data, L, new_size);

    double min = 0;
    double max = new_data[0];
    for (long long i = 0; i < new_size; ++i) {        
        if (new_data[i] > max) {
            max = new_data[i];
        }
    }

    new_size = new_size / 2;
    if (new_size == 0) new_size = 1;
    new_data.resize(new_size);

    auto data2d = std::make_shared<Graph2DData>();

    data2d->Hz = 1 / (new_size);
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
