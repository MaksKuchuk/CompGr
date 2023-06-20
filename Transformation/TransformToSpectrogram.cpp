#include "TransformToSpectrogram.hpp"
#include "TransformToFourierSpectrum.hpp"
#include <cmath>
#include <thread>
#include <QDebug>
#include "../Utility/utility.h"

void TransformToSpectrogram::column(std::shared_ptr<Graph3DData> spectrogram, std::shared_ptr<Graph2DData> data,
                   qint64 i, qint64 height, qint64 N, qint64 krat,
                   qint64 start_slice, qint64 end_slice) {
    if (end_slice >= data->amountOfSamples)
        end_slice = data->amountOfSamples - 1;
    auto temp = data->GetSlice(start_slice, end_slice);

    double sum = 0;
    for (qint64 j = 0; j < temp->samples.size(); ++j) {
        sum += temp->samples[j];
    }
    sum /= temp->samples.size();

    for (qint64 j = 0; j < temp->samples.size(); ++j) {
        temp->samples[j] -= sum;
    }

    for (qint64 j = 0; j < temp->samples.size(); ++j) {
        auto w = 0.54 - 0.46 * cos(2 * M_PI * j / (end_slice - start_slice - 1));
        temp->samples[j] = w * temp->samples[j];
    }

    auto dpf = TransformToFourierSpectrum::transform(temp, 0, SpectrumModes::AMPLITUDE_SPECTRUM, FourierModes::EQUALIZE_WITH_ADJ, krat * N);

//    for (size_t i = 0; i < dpf->samples.size() - 1; ++i)
//        if (dpf->samples[i+1] / dpf->maxVal < 1e-2) {
//            auto j = i + 1;
//            while (dpf->samples[j] / dpf->maxVal < 1e-2 && j < dpf->samples.size() - 1)
//                ++j;
//            for (auto p = 1; p < j - i + 1; ++p) {
//                dpf->samples[i + p] = Utility::LinearScale(p, j-i, dpf->samples[i], dpf->samples[j]);
//            }
//            i = j;
//        }

    if (krat == 1) {
        for (qint64 k = 0; k < dpf->samples.size(); ++k)
            spectrogram->samples[i][k] = dpf->samples[k];

        spectrogram->minVal = std::min(spectrogram->minVal, dpf->minVal);
        spectrogram->maxVal = std::max(spectrogram->maxVal, dpf->maxVal);
    } else {
        for (qint64 k = 0; k < height; ++k) {
            double ak = 0;

            for (qint64 j = -(krat - 1)/2 ; j <= krat / 2; ++j  ) {
                qint64 ind = abs(krat * k + j);
                if (ind < dpf->samples.size())
                    ak += dpf->samples[ind];
            }
            ak /= krat;

            spectrogram->minVal = std::min(spectrogram->minVal, ak);
            spectrogram->maxVal = std::max(spectrogram->maxVal, ak);

            spectrogram->samples[i][k] = ak;
        }

    }
}

std::shared_ptr<Graph3DData> TransformToSpectrogram::transform(std::shared_ptr<Graph2DData> data, qint64 width, qint64 height, double coeff)
{
    auto spectrogram = std::make_shared<Graph3DData>(width, height);

    double section_base = (double)data->amountOfSamples / width;
    qint64 section_n = section_base * coeff;

    qint64 N = 2 * height;

    qint64 krat = section_n / N;

    if (krat * N != section_n) {
        ++krat;
    }

    std::vector<std::unique_ptr<std::thread>> threads;

    for (size_t i = 0; i < width; ++i) {
        qint64 n0 = i * section_base;
        qint64 end = n0 + section_n;

        threads.emplace_back(std::make_unique<std::thread>(column, spectrogram, data,
                             i, height, N, krat,
                             n0, end));
    }

    for (size_t i = 0; i < width; ++i) {
        threads[i]->join();
    }

    spectrogram->Hz = data->Hz / section_base;
    spectrogram->yHz = 2*height/data->Hz;
    spectrogram->name = data->name + " Spectrogram";

    return spectrogram;
}
