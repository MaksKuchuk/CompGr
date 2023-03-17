#include "TransformToFourierSpectrum.hpp"

#include <cmath>
#include <limits>
#include <iostream>

void TransformToFourierSpectrum::FFTAnalysis(double *AVal, double *FTvl, int Nvl, int Nft) {
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
   }

Graph2DData* TransformToFourierSpectrum::transform(ParseData* data, long long n) {
    long long size = data->getAmountOfSamples();
    double* _AVal = data->getChannel(n);
    long long new_size = std::pow(2, std::ceil(std::log2(size)));
    double* AVal = new double[new_size]();
    double* FTvl = new double[new_size];
    for (long long i = 0; i < size; ++i) {
        AVal[i] = _AVal[i];
    }
    FFTAnalysis(AVal, FTvl, new_size, new_size);
    delete[]AVal;

    double min = std::numeric_limits<double>::max();
    double max = 0;
    for (long long i = 0; i < new_size; ++i) {
        if (FTvl[i] < min) {
            min = FTvl[i];
            std::cout << "min: " << min << std::endl;
        }
        if (FTvl[i] > max) {
            max = FTvl[i];
            std::cout << "max: " << max << std::endl;
        }
    }

    Graph2DData* data2d = new Graph2DData();
    data2d->Hz = 1 / (new_size);
    data2d->totalSeconds = 1;
    data2d->amountOfSamples = new_size;
    data2d->samples = FTvl;
    data2d->lcur = 0;
    data2d->rcur = (new_size) - 1;
    data2d->source = data->getFileName() + " Fourier transform";
    data2d->minVal = min;
    data2d->maxVal = max;

    return data2d;
}
